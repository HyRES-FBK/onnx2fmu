import re
import typer
import shutil
import subprocess
import numpy as np
from onnx import load, TensorProto
from pathlib import Path
from loguru import logger
from jinja2 import Environment, FileSystemLoader
from itertools import product


app = typer.Typer()


TEMPLATE_DIR = "template"
template_path = Path(TEMPLATE_DIR)


# C types are strings because they are simply copied in the template
onnx_to_c_types = {
    "FLOAT": {"FMI2": "Real", "FMI3": "Float32", "C": "float"},  # This is the most common
    "DOUBLE": {"FMI2": "Real", "FMI3": "Float64", "C": "double"},
}


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


def cleanName(name: str) -> str:
    """
    Clean the name of the ONNX node.

    Parameters:
    -----------
    - ``name`` (str): The name of the ONNX node or element

    Returns:
    --------
    - ``str``: The cleaned name.
    """
    return name.replace("/", "_").replace(":", "")


def FMI2generateIOEntries(model):
    """
    Parse the ONNX model and generate input/output entries for the FMU.

    Parameters:
    -----------
    - ``model`` (onnx.ModelProto): The ONNX model.
    - ``context`` (dict): The context dictionary for templating.
    """
    context = {}
    # Add all input nodes as multi-dimensional variables
    # Node names must be cleaned from C-unsupported characters
    input_name = cleanName(model.graph.input[0].name)
    # Retrieve input tensor shape
    shape = tuple(dim.dim_value for dim in
                    model.graph.input[0].type.tensor_type.shape.dim)
    # Print the input node name
    logger.debug(f"Input node {input_name} with shape {shape}.")
    # A unique index is used for all model variables. It starts at 1 because
    # the first index is reserved for the time variable.
    i = 1
    # FMI2 does not support multi-dimensional inputs, so we need to flatten
    # the input tensor.
    # Initialize the input entries list
    context['input_entries'] = []
    # Iterate over all dimensions except the first one
    for idx in np.ndindex(shape):
        element_type = TensorProto.DataType.Name(
            model.graph.input[0].type.tensor_type.elem_type
        )
        context['input_entries'].append({
            'name': f"{input_name}_" + "_".join(str(x) for x in idx),
            'index': i,
            'type': onnx_to_c_types[element_type],
            'description': "Input name identifies tensor element.",
            'causality': "independent",
            'variability': "continuous",
        })
        i += 1
    # Print the output node name
    output_name = model.graph.output[0].name
    # Retrieve output tensor shape
    shape = tuple(dim.dim_value for dim in
                model.graph.output[0].type.tensor_type.shape.dim)
    logger.debug(f"Output node: {output_name} with shape {shape}.")
    # Initialize the output entries list
    context['output_entries'] = []
    # Iterate over all dimensions except the first one
    for idx in np.ndindex(tuple(shape)):
        context['output_entries'].append({
            'name': f"out{output_name}_" + "_".join(str(x) for x in idx),
            'index': i,
            'type': onnx_to_c_types[element_type],
            'description': "Output name identifies tensor element.",
            'causality': "output",
            'variability': "continuous",
        })
        i += 1
    return context


def FMI3generateIOEntries(model):
    """
    Parse the ONNX model and generate input/output entries for the FMU.

    Parameters:
    -----------
    - ``model`` (onnx.ModelProto): The ONNX model.
    - ``context`` (dict): The context dictionary for templating.
    """
    # A unique index is used for all model variables. It starts at 1 because
    # the first index is reserved for the time variable.
    i = 1
    context = {}
    # Initialize the input entries list
    context['input_entries'] = []
    # Add all input nodes as multi-dimensional variables
    for node in model.graph.input:
        # Node names must be cleaned from C-unsupported characters
        input_name = cleanName(model.graph.input[0].name)
        # Retrieve input tensor shape
        shape = tuple(dim.dim_value for dim in
                      model.graph.input[0].type.tensor_type.shape.dim)
        # Print the input node name
        logger.debug(f"Input node {input_name} with shape {shape}.")
        # FMI3 supports multi-dimensional variables.
        # Iterate over all input nodes
        element_type = TensorProto.DataType.Name(
            node.type.tensor_type.elem_type
        )
        context['input_entries'].append({
            'name': f"{input_name}",
            'index': i,
            'type': onnx_to_c_types[element_type],
            'description': "Input name identifies tensor element.",
            'causality': "independent",
            'variability': "continuous",
            'dimensions': shape,
        })
        i += 1
    # Initialize the output entries list
    context['output_entries'] = []
    for node in model.graph.output:
        output_name = model.graph.output[0].name
        # Retrieve output tensor shape
        shape = tuple(dim.dim_value for dim in
                      model.graph.output[0].type.tensor_type.shape.dim)
        logger.debug(f"Output node: {output_name} with shape {shape}.")
        # Iterate over all dimensions except the first one
        context['output_entries'].append({
            'name': f"{output_name}",
            'index': i,
            'type': onnx_to_c_types[element_type],
            'description': "Output name identifies tensor element.",
            'causality': "output",
            'variability': "continuous",
            'dimensions': shape,
        })
        i += 1
    return context


def generateIOEntries(model):
    """
    Parse the ONNX model and generate input/output entries for the FMU.

    Parameters:
    -----------
    - ``model`` (onnx.ModelProto): The ONNX model.
    - ``context`` (dict): The context dictionary for templating.
    """
    # A unique index is used for all model variables. It starts at 1 because
    # the first index is reserved for the time variable.
    i = 1
    context = {}
    # Initialize the input entries list
    context['input_entries'] = []
    # Add all input nodes as multi-dimensional variables
    for node in model.graph.input:
        # Node names must be cleaned from C-unsupported characters
        # input_name = cleanName(model.graph.input[0].name)
        # Retrieve input tensor shape
        shape = tuple(dim.dim_value for dim in
                      model.graph.input[0].type.tensor_type.shape.dim)
        # Print the input node name
        logger.debug(f"Input node {node.name} with shape {shape}.")
        # FMI3 supports multi-dimensional variables.
        # Iterate over all input nodes
        element_type = TensorProto.DataType.Name(
            node.type.tensor_type.elem_type
        )
        context['input_entries'].append({
            'name': node.name,
            'type': onnx_to_c_types[element_type],
            'description': "Input name identifies tensor element.",
            'causality': "input",
            'variability': "continuous",
            'dimensions': shape,
            'indexes': {i + j: idx for j, idx in
                        enumerate(list(np.ndindex(shape)))},
        })
        i += len(list(np.ndindex(shape)))
        # Print the output node name
    for node in model.graph.output:
        # Retrieve output tensor shape
        shape = tuple(dim.dim_value for dim in
                      model.graph.output[0].type.tensor_type.shape.dim)
        logger.debug(f"Output node: {model.graph.output[0].name} with shape {shape}.")
        # Initialize the output entries list
        context['output_entries'] = []
        # Iterate over all dimensions except the first one
        context['output_entries'].append({
            'name': f"{model.graph.output[0].name}",
            'type': onnx_to_c_types[element_type],
            'description': "Output name identifies tensor element.",
            'causality': "output",
            'variability': "continuous",
            'dimensions': shape,
            'indexes': {i + j: idx for j, idx in
                        enumerate(list(np.ndindex(shape)))},
        })
        i += len(list(np.ndindex(shape)))
    return context


@app.command()
def build(
    model_path: str = typer.Argument(help="The path to the ONNX model file."),
):
    """
    Build the FMU.

    Parameters:
    -----------
    - ``model_path`` (str): The path to the model to be encapsulated in an FMU.
    """
    # Cast to Path
    model_path = Path(model_path)
    # Check if the model file exists
    if not model_path.exists():
        logger.error(f"Model file {model_path} does not exist.")
        raise typer.Exit(code=1)

    ############################
    # Retrieve model information
    ############################
    # Empty context dictionary for templating
    context = {
        "model_description": "This FMU loads an ONNX model and runs inference on it.",
        "model_name": model_path.stem,
        "model_version": find_version('pyproject.toml'),
        "model_GUID": "{7b9c2114-2ce5-4076-a138-2cbc69e069e5}",
        "canGetAndSetFMUstate": "true",
        "canSerializeFMUstate": "true",
        "canNotUseMemoryManagementFunctions": "true",
        "canHandleVariableCommunicationStepSize": "true",
        "providesIntermediateUpdate": "true",
        "canReturnEarlyAfterIntermediateUpdate": "true",
        "fixedInternalStepSize": "1",
        "startTime": "0",
        "stopTime": "1",
    }
    # Read the model file
    model = load(model_path)
    # Generate input entries for all dimensions except the first one

    context.update(generateIOEntries(model))

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
    for template_name in template_path.iterdir():
        # Skip directories and FMI files
        if not template_name.is_file():
            continue
        # Load the template
        template = env.get_template(str(template_name))
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
    # Copy onnxruntime to the resources directory, do not change
    onnx_folder = "onnxruntime-linux-x64-1.20.0"
    onnxruntime_target_path = target_path / f"resources/{onnx_folder}"
    shutil.copytree(f"lib/{onnx_folder}",
                    onnxruntime_target_path)

    ############################
    # Generate the FMU
    ############################

    # Declare CMake arguments
    cmake_command = [
        "cmake",
        "-S",
        ".",
        "-B",
        "build",
        "-DMODEL_NAME=" + model_path.stem,
    ]
    # Declare CMake build arguments
    build_command = [
        "cmake",
        "--build",
        "build",
    ]
    # Run cmake to generate the FMU
    subprocess.run(cmake_command, check=True)
    subprocess.run(build_command, check=True)

    ############################
    # Clean up
    ############################
    # Remove the target directory
    shutil.rmtree(target_path)


if __name__ == "__main__":
    app()
