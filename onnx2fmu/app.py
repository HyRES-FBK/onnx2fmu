import json
import typer
import shutil
import platform
import subprocess
import importlib.metadata
from pathlib import Path
from loguru import logger
from typing import Union
import importlib.resources as resources
from typing_extensions import Annotated
from onnx import load
from jinja2 import Environment, BaseLoader

from onnx2fmu.model_description import ModelDescription


app = typer.Typer()


PARENT_DIR = resources.files("onnx2fmu")
TEMPLATE_DIR = resources.files("onnx2fmu.template")


@app.command()
def version():
    version = importlib.metadata.version('onnx2fmu')
    typer.echo(f"ONNX2FMU version {version}")
    raise typer.Exit()


def _createFMUFolderStructure(destination: Path, model_path: Path) -> None:
    model_name = model_path.stem
    # Remove the target directory if it exists
    if destination.exists():
        shutil.rmtree(destination)
    # Create the target directories
    destination.mkdir(exist_ok=True)
    fmu_folder = destination / model_name
    fmu_folder.mkdir(exist_ok=True)
    resources_folder = fmu_folder / "resources"
    resources_folder.mkdir(exist_ok=True)
    shutil.copy(model_path, resources_folder)
    # The model must name `model.onnx`
    onnx_model = resources_folder / model_path.name
    if onnx_model.name != "model.onnx":
        shutil.move(onnx_model, resources_folder / "model.onnx")
    # Copy CMakeLists.txt to the target path
    resources.files('onnx2fmu')
    cmakelists_path = resources.files('onnx2fmu').joinpath('CMakeLists.txt')
    with resources.as_file(cmakelists_path) as path:
        shutil.copy(path, destination)
    # Copy src folder
    src_folder = resources.files('onnx2fmu').joinpath('src')
    with resources.as_file(src_folder) as path:
        shutil.copytree(path, destination / path.name, dirs_exist_ok=True)
    # Copy include folder
    include_folder = resources.files('onnx2fmu').joinpath('include')
    with resources.as_file(include_folder) as path:
        shutil.copytree(path, destination / path.name, dirs_exist_ok=True)


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
    target_folder: Annotated[
        str,
        typer.Argument(help="The folder where the FMU files are generated.")
    ] = "target",
) -> None:
    """Generate the FMU project folder structure in `target_folder`."""
    model_path, model_description_path, target_folder = _set_paths(
        model_path, model_description_path, target_folder
    ) # type: ignore

    onnx_model = load(model_path)

    model_description = json.loads(Path(model_description_path).read_text())

    model = ModelDescription(onnx_model, model_description)

    context = model.generateContext()

    _createFMUFolderStructure(destination=Path(target_folder),
                              model_path=Path(model_path))

    # Initialize Jinja2 environment
    env = Environment(loader=BaseLoader())

    for template_name in TEMPLATE_DIR.iterdir():
        # Skip directories and FMI files
        if not template_name.is_file():
            continue
        # Skip unmatching FMI model description
        if (template_name.stem.startswith("FMI")) and \
            (template_name.name !=
             f"FMI{int(float(context['FMIVersion']))}.xml"):
            continue
        # Read the template content from the package resource
        with resources.as_file(template_name) as path:
            template_source = path.read_text()
        # Create a Jinja2 template from the source
        template = env.from_string(template_source)
        # Render the template with the context
        rendered = template.render(context)
        # Write the rendered template to the target directory
        core_dir = Path(target_folder) / f"{model_path.stem}" / template_name.name
        with open(core_dir, "w") as f:
            f.write(rendered)


def _set_paths(
        model_path: Union[str, Path],
        model_description_path: Union[str, Path],
        destination: Union[str, Path]):
    if type(model_path) is str:
        model_path = Path(model_path)
        if not model_path.exists():
            raise ValueError(f"Cannot find model at {model_path}.")
    if type(model_description_path) is str:
        model_description_path = Path(model_description_path)
        if not model_description_path.exists():
            raise ValueError(
                f"Cannot find model description at {model_description_path}.")
    if type(destination) is str:
        destination = Path(destination)
    return model_path,model_description_path,destination


def complete_platform():
    return ['x86-windows', 'x86_64-windows', 'x86_64-linux', 'aarch64-linux',
            'x86_64-darwin', 'aarch64-darwin']

def cmake_configurations():
    return ['Debug', 'Release']


@app.command()
def compile(
    target_folder: Annotated[
        str,
        typer.Argument(help="The folder containing the FMU files to be compiled.")
    ],
    model_description_path: Annotated[
        str,
        typer.Argument(help="The path to the model description file.")
    ],
    destination: Annotated[
        str,
        typer.Option(help="The destination folder where to copy the FMU.")
    ] = ".",
    fmi_platform: Annotated[
        str,
        typer.Option(
            help="The target platform to build for. If empty, the program " +
            "set the target to the platform where it is compiled. See --help" +
            " for further options.",
            autocompletion=complete_platform
        )
    ] = "",
    cmake_config: Annotated[
        str,
        typer.Option(help="The CMake build config.",
                     autocompletion=cmake_configurations)
    ] = "Release"
) -> None:
    """Compile the project defined in `target_folder`."""
    if type(target_folder) is str:
        target_folder = Path(target_folder)
    if type(model_description_path) is str:
        model_description_path = Path(model_description_path)
        if not model_description_path.exists():
            raise ValueError(f"{model_description_path} does not exist.")

    with open(model_description_path, "r") as f:
        model_description = json.load(f)

    model_name = model_description["name"]

    if fmi_platform in complete_platform():
        fmi_architecture, fmi_system = fmi_platform.split("-")
    else:
        fmi_system = platform.system().lower()
        # Left empty, CMake will detect it
        fmi_architecture = None

    # Create build dir
    build_dir = target_folder / Path("build")

    if not build_dir.exists():
        build_dir.mkdir(exist_ok=True)

    # Declare CMake arguments
    cmake_args = [
        '-S', str(target_folder),
        '-B', str(build_dir),
        '-D', f'MODEL_NAME={model_name}',
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

        # Add /Wv:18 flag for MSVC
        cmake_args += [
            '-DCMAKE_C_FLAGS=/Wv:18',
            '-DCMAKE_CXX_FLAGS=/Wv:18'
        ]

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
    shutil.copy(build_dir / f"fmus/{model_name}.fmu", destination)
    # Remove the build folder
    shutil.rmtree(build_dir)
    # Remove the target directory
    shutil.rmtree(target_folder)


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
    target_folder: Annotated[
        str,
        typer.Argument(help="The folder where the FMU files are generated and from which the FMU is compiled.")
    ],
    destination: Annotated[
        str,
        typer.Option(help="The destination folder where to copy the FMU.")
    ] = ".",
    fmi_platform: Annotated[
        str,
        typer.Option(
            help="The target platform to build for. If empty, the program " +
            "set the target to the platform where it is compiled. See --help" +
            " for further options.",
            autocompletion=complete_platform
        )
    ] = "",
    cmake_config: Annotated[
        str,
        typer.Option(help="The CMake build config.",
                     autocompletion=cmake_configurations)
    ] = "Release"
) -> None:
    """Build the FMU."""
    # Generate the FMU
    generate(
        model_path=model_path,
        model_description_path=model_description_path,
        target_folder=target_folder,
    )

    # Compile the FMU
    compile(
        target_folder=target_folder,
        model_description_path=model_description_path,
        destination=destination,
        fmi_platform=fmi_platform,
        cmake_config=cmake_config
    )


if __name__ == "__main__":
    app()
