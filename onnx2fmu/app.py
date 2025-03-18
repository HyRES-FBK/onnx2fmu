import re
import uuid
import json
import typer
import shutil
import subprocess
import numpy as np
from pathlib import Path
from loguru import logger
from datetime import datetime
from typing_extensions import Annotated
from onnx import load, TensorProto, ModelProto
from jinja2 import Environment, FileSystemLoader


app = typer.Typer()


TEMPLATE_DIR = Path("template")


class ScalarVariable:
    """
    A 'ScalarVariable' entry of the model description for FMI 2.0.
    """

    VARIABILITY = ["constant", "fixed", "tunable", "discrete", "continuous"]
    CAUSALITY = ["parameter", "calculatedParameter", "input", "output",
                 "local", "independent"]
    TYPES = {
        TensorProto.FLOAT: "Real",
        TensorProto.INT4: "Integer",
        TensorProto.INT8: "Integer",
        TensorProto.INT16: "Integer",
        TensorProto.INT32: "Integer",
        TensorProto.INT64: "Integer",
        TensorProto.UINT8: "Integer",
        TensorProto.UINT16: "Integer",
        TensorProto.UINT32: "Integer",
        TensorProto.UINT64: "Integer",
        TensorProto.BOOL: "Boolean",
        TensorProto.STRING: "String"
    }

    def __init__(self,
                 name: str,
                 description: str,
                 variability: str,
                 causality: str,
                 valueReference: int,
                 vType: TensorProto.DataType,
                 start: str = None):

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
        elif variability not in self.VARIABILITY:
            raise ValueError(f"Variability {variability} is not valid.")
        else:
            self.variability = variability

        if not causality:
            raise ValueError("Causality is a required argument.")
        elif causality not in self.CAUSALITY:
            raise ValueError(f"Causality {causality} is not valid.")
        else:
            self.causality = causality

        if not valueReference:
            raise ValueError("Value reference is a required argument.")
        else:
            self.valueReference = valueReference

        if not vType:
            self.vType = 'Real'
        else:
            self.vType = self.TYPES[vType]

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

    FMI_VERSIONS = ["2.0", "3.0"]
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
        self.vr = (i for i in range(1, 10000))

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

        # FMI 2.0 and 3.0 have different ways of handling model variables
        if self.FMIVersion == "2.0":
            self.fmi2GetVariables()
        elif self.FMIVersion == "3.0":
            self.fmi3GetVariables()

    def fmi2GetVariables(self):
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
                    ) for j in range(len(array_names))
                ]
                # Store indexes for easy access when generating templates
                setattr(self, entry, getattr(self, entry) + [array])

    def fmi3GetVariables(self):
        pass

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
    if not Path('version.txt').exists():
        typer.echo("versiont.txt file not found.")
        raise typer.Exit(code=1)
    # Parse version from the config.py file using regex
    typer.echo(f"ONNX2FMU {find_version('version.txt')}")


def complete_platform():
    return ['x86-windows', 'x86_64-windows', 'x86_64-linux', 'aarch64-linux',
            'x86_64-darwin', 'aarch64-darwin']


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
    fmi_version: Annotated[
        int,
        typer.Option(help="The FMI version, only 2 and 3 are supported. Default is 2.")
    ] = 2,
    fmi_platform: Annotated[
        str,
        typer.Option(
            help="Platform to build for, e.g. x86_64-windows.",
            autocompletion=complete_platform)
    ] = "x86_64-windows"
):
    """
    Build the FMU.

    Parameters:
    -----------

    - ``model_path`` (str): The path to the model to be encapsulated in an FMU.

    - ``model_description_path`` (str): The path to the model description file.

    - ``fmi_version`` (str): The FMI version, only 2.0 and 3.0 are supported.
    """
    # Cast to Path
    model_path = Path(model_path)
    # Check if the model file exists
    if not model_path.exists():
        logger.error(f"Model file {model_path} does not exist.")
        raise typer.Exit(code=1)

    ##############################
    # Retrieve model information #
    ##############################
    # Read the model file
    onnx_model = load(model_path)
    # Read model description
    model_description = json.loads(Path(model_description_path).read_text())
    # Initialize model handler
    model = Model(onnx_model, model_description)
    # Generate context for the template
    context = model.generate_context()

    ############################
    # Populate the templates
    ############################
    # Set target directory to the model name
    target_path = Path(f"{model_path.stem}")
    # Remove the target directory if it exists
    if target_path.exists():
        shutil.rmtree(target_path)
    # Create the target directory
    target_path.mkdir(exist_ok=True)
    # Create a Jinja2 environment and set the current directory as the search
    # path
    env = Environment(loader=FileSystemLoader("."))
    # Iterate over all the remaining templates
    for template_name in TEMPLATE_DIR.iterdir():
        # Skip directories and FMI files
        if not template_name.is_file():
            continue
        # Load the template
        template = env.get_template("template/" + str(template_name.name))
        # Render the template with the context
        out = template.render(context)
        # Write the rendered template to the target directory
        with open(target_path / f"{template_name.name}", "w") as f:
            f.write(out)

    # Copy the model to the resources directory, do not change
    model_target_path = target_path / "resources/model.onnx"
    model_target_path.parent.mkdir(exist_ok=True)
    # Copy the model to the target directory
    shutil.copy(model_path, model_target_path)

    ############################
    # Generate the FMU
    ############################

    fmi_architecture, fmi_system = fmi_platform.split("-")

    fmi_version = str(fmi_version)

    # Declare CMake arguments
    cmake_args = [
        "-S", ".",
        "-B", "build",
        "-DMODEL_NAME=" + model_path.stem,
        "-DFMI_VERSION=" + fmi_version,
        "-DFMI_ARCHITECTURE=" + fmi_architecture
    ]

    if fmi_system == "windows":

        cmake_args += ["-G", "'Visual Studio 17 2022'"]

        if fmi_architecture == "x86":
            cmake_args += ["-A", "Win32"]
        elif fmi_architecture == "x86_64":
            cmake_args += ["-A", "x64"]

    # elif fmi_platform == "aarch64-linux":

    #     toolchain_file = parent_dir / "aarch64-linux-toolchain.cmake"
    #     cmake_args += ["-D", f"CMAKE_TOOLCHAIN_FILE={ toolchain_file }"]

    elif fmi_platform == "x86_64-darwin":

        cmake_args += ["-D", "CMAKE_OSX_ARCHITECTURES=x86_64"]

    elif fmi_platform == "aarch64-darwin":

        cmake_args += ["-D", "CMAKE_OSX_ARCHITECTURES=arm64"]


    # Declare CMake build arguments
    build_command = [
        "cmake",
        "--build", "build",
        "--config", "Release"
    ]
    # Run cmake to generate the FMU
    logger.info(f"Call cmake {' '.join(cmake_args)}")
    subprocess.check_call(["cmake"] + cmake_args)
    subprocess.check_call(build_command)

    ############################
    # Clean up
    ############################
    # Remove the target directory
    shutil.rmtree(target_path)


if __name__ == "__main__":
    app()
