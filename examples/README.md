# ONNX2FMU examples

ONNX2FMU can export co-simulation FMUs for both FMI 2.0 and 3.0 standards. Starting from an ONNX model, the user must specify a mapping between the model's inputs and outputs and the FMU's I/O in a JSON model description file.

To illustrate the ability of ONNX2FMU to encapsulate any ONNX model in an FMU, we
provide the following examples:

- [**Example 1**](example1): Demonstrates basic vector operations. It shows how to handle scalar and vector inputs and combine them to produce an output.
- [**Example 2**](example2): A simple example showcasing a model with multiple inputs and outputs.
- [**Example 3**](example3): A more complex example featuring a Recurrent Neural Network (RNN) to predict the temperature on a metallic plate. This highlights how to handle models with internal states and normalization parameters.
- [**Example 4**](example4): An example of an autoregressive model, which uses its own output as a future input. This is achieved using FMI `local` variables to create a feedback loop.

## Get started with the examples

To run the examples, you first need to clone the repository and install the necessary development dependencies. Each example is contained in a dedicated Jupyter notebook.

If you are using uv, you can set up the environment by running the following command in your terminal:

```bash
uv sync --all-extras
```

If you prefer using `pip`, you can install the dependencies with:

```bash
pip install ".[extra]"
```

After setting up the environment, you can launch Jupyter Lab or Jupyter Notebook and navigate to the `examples` directory to open and run the notebooks for each example.

## FMU export compatibility information

ONNX2FMU is compatible with the FMI 2.0 and 3.0 standards.

FMU generation is regularly tested for Windows, Linux, and MacOS; tested architectures are x86 and ARM.

Only co-simulation FMUs can be exported; export of model exchange FMUs is under development.

### Tested import tools

The import and execution of ONNX2FMU-generated FMUs has been tested with:

- [**FMPy**](https://github.com/CATIA-Systems/FMPy) is part of the testing pipeline of ONNX2FMU. FMUs were successfully
imported, validated, compiled using the C soruce code, and simulated.
- [**Modelon Impact**](https://modelon.com/modelon-impact/) can import Linux-based co-simulation FMUs. The FMU
must be compatible with Ubuntu 20.04. The FMUs generated with ONNX2FMU could be
imported, compiled, and simulated using Modelon Impact.
