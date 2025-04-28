import re
import uuid
import json
import typer
import shutil
import platform
import subprocess
import numpy as np
from pathlib import Path
from loguru import logger
from datetime import datetime
import importlib.resources as resources
from typing_extensions import Annotated
from onnx import load, TensorProto, ModelProto
from jinja2 import Environment, BaseLoader


app = typer.Typer()


PARENT_DIR = resources.files("onnx2fmu")
TEMPLATE_DIR = resources.files("onnx2fmu.template")

VARIABILITY = ["discrete", "continuous"]
CAUSALITY = ["input", "output"]

FMI2TYPES = {
    TensorProto.FLOAT:  {"FMIType": "Real",    "CType": "double"},
    TensorProto.DOUBLE: {"FMIType": "Real",    "CType": "double"},
    TensorProto.INT4:   {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT8:   {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT16:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT32:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.INT64:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT8:  {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT16: {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT32: {"FMIType": "Integer", "CType": "int"},
    TensorProto.UINT64: {"FMIType": "Integer", "CType": "int"},
    TensorProto.BOOL:   {"FMIType": "Boolean", "CType": "bool"},
    TensorProto.STRING: {"FMIType": "String", "CType": "char"},
}

FMI3TYPES = {
    TensorProto.FLOAT:  {"FMIType": "Float32", "CType": "float"},
    TensorProto.DOUBLE: {"FMIType": "Float64", "CType": "double"},
    TensorProto.INT4:   {"FMIType": "Int8",    "CType": "int"},
    TensorProto.INT8:   {"FMIType": "Int8",    "CType": "int"},
    TensorProto.INT16:  {"FMIType": "Int16",   "CType": "int"},
    TensorProto.INT32:  {"FMIType": "Int32",   "CType": "int"},
    TensorProto.INT64:  {"FMIType": "Int64",   "CType": "int"},
    TensorProto.UINT8:  {"FMIType": "UInt8",   "CType": "int"},
    TensorProto.UINT16: {"FMIType": "UInt16",  "CType": "int"},
    TensorProto.UINT32: {"FMIType": "UInt32",  "CType": "int"},
    TensorProto.UINT64: {"FMIType": "UInt64",  "CType": "int"},
    TensorProto.BOOL:   {"FMIType": "Boolean", "CType": "bool"},
    TensorProto.STRING: {"FMIType": "String",  "CType": "char"},
}


class ScalarVariable:
    """
    A 'ScalarVariable' entry of the model description.
    """

    def __init__(self,
                 name: str,
                 description: str,
                 variability: str,
                 causality: str,
                 valueReference: int,
                 vType: TensorProto.DataType,
                 start: str = None,
                 fmi_version: str = "2.0"):

        # Mandatory arguments
        if not name:
            raise ValueError("Name is a required argument.")
        else:
            self.name = re.sub(r'[^\w]', '', name)

        # Optional arguments
        if not description:
            self.description = "Description of the array was not provided."
        else:
            self.description = description

        if not variability:
            self.variability = 'discrete'
        elif variability not in VARIABILITY:
            raise ValueError(f"Variability {variability} is not valid.")
        else:
            self.variability = variability

        if not causality:
            raise ValueError("Causality is a required argument.")
        elif causality not in CAUSALITY:
            raise ValueError(f"Causality {causality} is not valid.")
        else:
            self.causality = causality

        if not valueReference:
            raise ValueError("Value reference is a required argument.")
        else:
            self.valueReference = valueReference

        if fmi_version == "2.0":
            if vType not in FMI2TYPES:
                raise ValueError("vType not in FMI 2.0 allowed types.")
            else:
                self.vType = FMI2TYPES[vType]
        elif fmi_version == "3.0":
            if vType not in FMI3TYPES:
                raise ValueError("vType not in FMI 3.0 allowed types.")
            else: self.vType = FMI3TYPES[vType]
        else:
            raise ValueError("Wrong FMI version, must be one of 2.0 or 3.0.")

        if self.causality == 'input' and self.variability == 'continuous':
            if start:
                self.start = start
            else:
                self.start = 1.0

    def generate_context(self):
        context = {}
        context['name'] = self.name
        context['description'] = self.description
        context['variability'] = self.variability
        context['causality'] = self.causality
        context['type'] = self.vType
        if self.start is not None:
            context['start'] = self.start
        return context


class Model:
    """
    The model factory class.
    """

    canGetAndSetFMUstate = True
    canSerializeFMUstate = True
    canNotUseMemoryManagementFunctions = True
    canHandleVariableCommunicationStepSize = True
    providesIntermediateUpdate = True
    canReturnEarlyAfterIntermediateUpdate = True
    fixedInternalStepSize = 1
    startTime = 0
    stopTime = 1

    def __init__(self, onnx_model: ModelProto, model_description: dict):
        """
        Initialize the model factory.

        Parameters:
        -----------
        - ``onnx_model`` (onnx.ModelProto): The ONNX model.
        - ``model_description`` (dict): The model description.
        """

        #####################
        # Model description #
        #####################

        self.model_description = model_description

        if 'name' not in model_description:
            self.name = "Model"
        # Check if special characters are present in the model name
        else:
            self.name = model_description['name'].replace(" ", "_")\
                .replace("-", "_").replace(".", "_").replace(":", "_")

        if 'description' not in model_description:
            self.description = "Description of the model was not provided."
        else:
            self.description = model_description['description']

        if 'FMIVersion' not in model_description:
            self.FMIVersion = "2.0"
        else:
            self.FMIVersion = model_description['FMIVersion']

        # Generate model GUID
        self.GUID = str(uuid.uuid4())

        # Initialize value reference index for model description variables
        self.vr = (i for i in range(1, 100000))

        ############################
        # ONNX model health checks #
        ############################

        self.onnx_model = onnx_model

        # Check that the number of inputs in the model description matches the
        # number of inputs in the ONNX model
        assert \
            (len(model_description.get('input', [])) ==
             len(onnx_model.graph.input)), \
            "The number of inputs in the model description does not match " + \
            "the ONNX model."

        # Check that the list of inputs in the model description is not empty
        assert len(model_description.get('input', [])) > 0, \
            "At least one input must be provided."

        # Check that onnx node names and description names match
        for i, node in enumerate(onnx_model.graph.input):
            assert node.name == model_description['input'][i]['name']

        # Check that the number of outputs in the model description matches the
        # number of outputs in the ONNX model
        assert \
            (len(model_description.get('output', [])) ==
             len(onnx_model.graph.output)), \
            "The number of outputs in the model description does not match" + \
            " the ONNX model."

        # Check that the list of outputs in the model description is not empty
        assert len(model_description.get('output', [])) > 0, \
            "At least one output must be provided."

        # Check that onnx node names and description names match
        for i, node in enumerate(onnx_model.graph.output):
            assert node.name == model_description['output'][i]['name']

        ############################################
        # Variables extraction from the ONNX model #
        ############################################
        entries = ['input', 'output']
        for entry in entries:
            setattr(self, entry, [])
            nodes = getattr(self.onnx_model.graph, entry)
            for i, node in enumerate(nodes):
                description = self.model_description[entry][i]
                array = {}
                array["name"] = description.get('name', node.name)
                # Retrieve tensor shape
                array["shape"] = tuple(
                    dim.dim_value for dim in node.type.tensor_type.shape.dim
                )
                # If tensor shape is empty, set it to 1
                if not array["shape"]:
                    array["shape"] = (1,)
                # Define array names
                array_names = [
                    array['name'] + "_" + "_".join([str(k) for k in idx])
                    for idx in np.ndindex(array['shape'])
                ]
                # Use names provided by the user if available
                if 'names' in description:
                    array["names"] = description["names"]
                else:
                    array["names"] = array_names
                # Store the scalar variables
                array["scalarValues"] = [
                    ScalarVariable(
                        name=array_names[j],
                        description=array["names"][j],
                        variability=description.get('variability',
                                                    'continuous'),
                        causality=description.get('causality', entry),
                        valueReference=next(self.vr),
                        vType=node.type.tensor_type.elem_type,
                        fmi_version=self.FMIVersion,
                    ) for j in range(len(array_names))
                ]
                # Store indexes for easy access when generating templates
                setattr(self, entry, getattr(self, entry) + [array])

    def generate_context(self):
        # Initialize the context dictionary
        context = {}
        # Iterate over attributes and add model information
        context['name'] = self.name
        context['description'] = self.description
        context['GUID'] = self.GUID
        context['FMIVersion'] = self.FMIVersion
        context['generationDateAndTime'] = datetime.now().isoformat()
        context['canGetAndSetFMUstate'] = self.canGetAndSetFMUstate
        context['canSerializeFMUstate'] = self.canSerializeFMUstate
        context['canNotUseMemoryManagementFunctions'] = \
            self.canNotUseMemoryManagementFunctions
        context['canHandleVariableCommunicationStepSize'] = \
            self.canHandleVariableCommunicationStepSize
        context['providesIntermediateUpdate'] = self.providesIntermediateUpdate
        context['canReturnEarlyAfterIntermediateUpdate'] = \
            self.canReturnEarlyAfterIntermediateUpdate
        context['fixedInternalStepSize'] = self.fixedInternalStepSize
        context['startTime'] = self.startTime
        context['stopTime'] = self.stopTime
        # Add variables to the context
        context['inputs'] = self.input
        context['outputs'] = self.output
        # Return the context
        return context


def find_version(file_path: str) -> str:
    version_pattern = re.compile(r'^version\s*=\s*[\'"]([^\'"]+)[\'"]',
                                 re.MULTILINE)
    try:
        with open(file_path, 'r') as file:
            content = file.read()
            match = version_pattern.search(content)
            if match:
                return match.group(1)
            else:
                return "Version not found"
    except FileNotFoundError:
        return "File not found"


@app.command()
def version():
    """Parse ONNX version from version.txt file."""
    # Check if pyproject.toml file exists
    if not Path('pyproject.toml').exists():
        typer.echo("versiont.txt file not found.")
        raise typer.Exit(code=1)
    # Parse version from the config.py file using regex
    typer.echo(f"ONNX2FMU {find_version('pyproject.toml')}")


def complete_platform():
    return ['x86-windows', 'x86_64-windows', 'x86_64-linux', 'aarch64-linux',
            'x86_64-darwin', 'aarch64-darwin']

def cmake_configurations():
    return ['Debug', 'Release']


def model_information(model_path: str, model_description_path: str,
                      destination: str, fmi_version: str):
    ##############################
    # Retrieve model information #
    ##############################
    # Cast to Path
    model_path = Path(model_path)
    destination = Path(destination)
    # Check if the model file exists
    if not model_path.exists():
        logger.error(f"Model file {model_path} does not exist.")
        raise typer.Exit(code=1)
    # Read the model file
    onnx_model = load(model_path)
    # Read model description
    model_description = json.loads(Path(model_description_path).read_text())
    # CLI-provided FMI version takes over model description version
    if fmi_version is not None:
        model_description['FMIVersion'] = fmi_version

    return model_path, destination, onnx_model, model_description


@app.command()
def generate(
    model_path: Annotated[
        str,
        typer.Argument(help="The path to the ONNX model file.")
    ],
    model_description_path: Annotated[
        str,
        typer.Argument(help="The path to the model description file.")
    ],
    destination: Annotated[
        str,
        typer.Option(help="The destination path.")
    ] = ".",
    fmi_version: Annotated[
        str,
        typer.Option(
            help="The FMI version, only 2 and 3 are supported. Default is 2."
        )
    ] = None,
):
    ##############################
    # Retrieve model information #
    ##############################
    model_path, _, onnx_model, model_description = \
        model_information(
            model_path=model_path,
            model_description_path=model_description_path,
            destination=destination,
            fmi_version=fmi_version
        )
    # Initialize model handler
    model = Model(onnx_model, model_description)
    # Generate context for the template
    context = model.generate_context()

    #############################
    # Generate the FMU template #
    #############################
    # Set target directory to the model name
    target_path = Path(f"{model_path.stem}")
    # Remove the target directory if it exists
    if target_path.exists():
        shutil.rmtree(target_path)
    # Create the target directories
    target_path.mkdir(exist_ok=True)
    (target_path / f'{model_path.stem}').mkdir(exist_ok=True)
    # Create a Jinja2 environment and set the current directory as the search
    # path
    env = Environment(loader=BaseLoader())
    # Iterate over all the remaining templates
    for template_name in TEMPLATE_DIR.iterdir():
        # Skip directories and FMI files
        if not template_name.is_file():
            continue
        # Read the template content from the package resource
        with resources.as_file(template_name) as path:
            template_source = path.read_text()
        # Create a Jinja2 template from the source
        template = env.from_string(template_source)
        # Render the template with the context
        rendered = template.render(context)
        # Write the rendered template to the target directory
        core_dir = target_path / f"{model_path.stem}/{template_name.name}"
        with open(core_dir, "w") as f:
            f.write(rendered)

    # Copy the model to the resources directory, do not change
    model_target_path = target_path / f"{model_path.stem}/resources/model.onnx"
    model_target_path.parent.mkdir(exist_ok=True)
    # Copy the model to the target directory
    shutil.copy(model_path, model_target_path)
    # Copy CMakeLists.txt to the target path
    shutil.copy(resources.files('onnx2fmu').joinpath('CMakeLists.txt'),
                target_path)
    # Copy src folder
    src_folder = resources.files('onnx2fmu').joinpath('src')
    with resources.as_file(src_folder) as path:
        shutil.copytree(path, target_path / path.name, dirs_exist_ok=True)
    # Copy include folder
    include_folder = resources.files('onnx2fmu').joinpath('include')
    with resources.as_file(include_folder) as path:
        shutil.copytree(path, target_path / path.name, dirs_exist_ok=True)


@app.command()
def compile(
    model_path: Annotated[
        str,
        typer.Argument(help="The path to the ONNX model file.")
    ],
    model_description_path: Annotated[
        str,
        typer.Argument(help="The path to the model description file.")
    ],
    destination: Annotated[
        str,
        typer.Option(help="The destination path.")
    ] = ".",
    fmi_version: Annotated[
        str,
        typer.Option(
            help="The FMI version, only 2 and 3 are supported. Default is 2."
        )
    ] = None,
    fmi_platform: Annotated[
        str,
        typer.Option(
            help="The target platform to build for. If empty, the program" +
            "set the target to the platform where it is compiled.",
            autocompletion=complete_platform
        )
    ] = "",
    cmake_config: Annotated[
        str,
        typer.Option(help="The CMake build config.",
                     autocompletion=cmake_configurations)
    ] = "Release"
):
    ##############################
    # Retrieve model information #
    ##############################
    model_path, destination, _, model_description = \
        model_information(
            model_path=model_path,
            model_description_path=model_description_path,
            destination=destination,
            fmi_version=fmi_version
        )
    # Set target directory to the model name
    target_path = Path(f"{model_path.stem}")
    ####################
    # Generate the FMU #
    ####################
    if fmi_platform in complete_platform():
        fmi_architecture, fmi_system = fmi_platform.split("-")
    else:
        fmi_system = platform.system().lower()
        # Left empty, CMake will detect it
        fmi_architecture = None

    # Create build dir
    build_dir = target_path / "build"

    if not build_dir.exists():
        build_dir.mkdir(exist_ok=True)

    # Declare CMake arguments
    cmake_args = [
        '-S', str(target_path),
        '-B', str(build_dir),
        '-D', f'MODEL_NAME={model_path.stem}',
        '-D', f'FMI_VERSION={int(float(model_description["FMIVersion"]))}',
    ]

    if fmi_architecture:
        cmake_args += ['-D', f'FMI_ARCHITECTURE={fmi_architecture}']

    if fmi_system == 'windows':

        cmake_args += ['-G', 'Visual Studio 17 2022']

        if fmi_architecture == 'x86':
            cmake_args += ['-A', 'Win32']
        elif fmi_architecture == 'x86_64':
            cmake_args += ['-A', 'x64']

    elif fmi_platform == 'aarch64-linux':

        toolchain_file = PARENT_DIR / 'aarch64-linux-toolchain.cmake'
        cmake_args += ['-D', f'CMAKE_TOOLCHAIN_FILE={ toolchain_file }']

    elif fmi_platform == 'x86_64-darwin':

        cmake_args += ['-D', 'CMAKE_OSX_ARCHITECTURES=x86_64']

    elif fmi_platform == 'aarch64-darwin':

        cmake_args += ['-D', 'CMAKE_OSX_ARCHITECTURES=arm64']

    # Declare CMake build arguments
    cmake_build_args = [
        '--build', str(build_dir),
        '--config', cmake_config
    ]

    # Run cmake to generate the FMU
    logger.info(f'Call cmake {" ".join(cmake_args)}')
    subprocess.run(['cmake'] + cmake_args, check=True)
    logger.info(f'CMake build cmake {" ".join(cmake_build_args)}')
    subprocess.run(['cmake'] + cmake_build_args, check=True)

    ############################
    # Clean up
    ############################
    # Copy the FMU
    shutil.copy(build_dir / f"fmus/{model_path.stem}.fmu", destination)
    # Remove the build folder
    shutil.rmtree(build_dir)
    # Remove the target directory
    shutil.rmtree(target_path)


@app.command()
def build(
    model_path: Annotated[
        str,
        typer.Argument(help="The path to the ONNX model file.")
    ],
    model_description_path: Annotated[
        str,
        typer.Argument(help="The path to the model description file.")
    ],
    destination: Annotated[
        str,
        typer.Option(help="The destination path.")
    ] = ".",
    fmi_version: Annotated[
        str,
        typer.Option(
            help="The FMI version, only 2 and 3 are supported. Default is 2."
        )
    ] = None,
    fmi_platform: Annotated[
        str,
        typer.Option(
            help="The target platform to build for. If empty, the program" +
            "set the target to the platform where it is compiled.",
            autocompletion=complete_platform
        )
    ] = "",
    cmake_config: Annotated[
        str,
        typer.Option(help="The CMake build config.",
                     autocompletion=cmake_configurations)
    ] = "Release"
):
    """
    Build the FMU.

    Parameters:
    -----------

    - ``model_path`` (str): The path to the model to be encapsulated in an FMU.

    - ``model_description_path`` (str): The path to the model description file.

    - ``destination`` (str): The destination path where to copy the FMU.

    - ``fmi_version`` (int): The FMI version, only 2.0 and 3.0 are supported.

    - ``fmi_platform`` (str): One of 'x86-windows', 'x86_64-windows',
    'x86_64-linux', 'aarch64-linux', 'x86_64-darwin', 'aarch64-darwin'. If left
    blank, it builds for the current platform.

    - ``cmake_config`` (str): The CMake build config.
    """
    # Generate the FMU
    generate(
        model_path=model_path,
        model_description_path=model_description_path,
        destination=destination,
        fmi_version=fmi_version
    )

    # Compile the FMU
    compile(
        model_path=model_path,
        model_description_path=model_description_path,
        destination=destination,
        fmi_version=fmi_version,
        fmi_platform=fmi_platform,
        cmake_config=cmake_config
    )


if __name__ == "__main__":
    app()
