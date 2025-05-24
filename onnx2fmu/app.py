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
from typing import Union
import importlib.resources as resources
from typing_extensions import Annotated
from onnx import load, TensorProto, ModelProto
from jinja2 import Environment, BaseLoader


app = typer.Typer()


PARENT_DIR = resources.files("onnx2fmu")
TEMPLATE_DIR = resources.files("onnx2fmu.template")

VARIABILITY = ["discrete", "continuous"]
CAUSALITY = ["input", "output"]

def _find_version(file_path: Union[str, Path]) -> str:
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
        typer.echo("pyproject.toml file not found.")
        raise typer.Exit(code=1)
    # Parse version from the config.py file using regex
    typer.echo(f"ONNX2FMU {_find_version('pyproject.toml')}")


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
    print(model_path)
    shutil.copy(model_path, resources_folder)
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
