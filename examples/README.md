# ONNX2FMU examples

Each folder contains a notebook that builds an FMU from scratch, together with
the files it produces (the ONNX model, the model description and the FMU).

The notebooks are published, with their outputs, as the tutorials of the
[ONNX2FMU documentation](https://hyres-fbk.github.io/onnx2fmu/).

| Example | Notebook | What it shows |
|---|---|---|
| 1 | [example1.ipynb](example1/example1.ipynb) | Scalar and vector inputs, labels, building and simulating an FMU. |
| 2 | [example2.ipynb](example2/example2.ipynb) | The smallest complete workflow: several scalar inputs, one output. |
| 3 | [example3.ipynb](example3/example3.ipynb) | A trained recurrent network with normalisation baked into the model. |
| 4 | [example4.ipynb](example4/example4.ipynb) | Feedback states: a model that remembers its own past. |
| 5 | [example5.ipynb](example5/example5.ipynb) | A neural ODE exported as a Model Exchange FMU. |

## Run the examples

Clone the repository and install the extra dependencies:

```bash
uv sync --all-extras      # with uv
pip install ".[extra]"    # or with pip
```

Then open a notebook with Jupyter Lab, Jupyter Notebook or VS Code. You also
need a C compiler; see the
[installation notes](https://hyres-fbk.github.io/onnx2fmu/#install).

Running a notebook regenerates the model, the model description, the FMU and
the CSV files in its folder. The test suite reads some of these files, so
think twice before committing them.
