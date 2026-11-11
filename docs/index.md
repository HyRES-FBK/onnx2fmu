---
title: ONNX2FMU
description: Wrap ONNX models in Functional Mock-up Units.
---

ONNX2FMU takes a model saved in the [ONNX](https://onnx.ai/) format and wraps
it in a [Functional Mock-up Unit](https://fmi-standard.org/) (FMU), so that a
neural network trained in PyTorch, TensorFlow or scikit-learn can run inside
any simulation tool that speaks FMI.

You give it two files:

- the **ONNX model**, and
- a short **JSON model description** that says which graph nodes are inputs
  and outputs of the FMU.

You get back a single `.fmu` archive for FMI 2.0 or 3.0, as a Co-Simulation
FMU, a Model Exchange FMU, or both.

## Install

```bash
pip install onnx2fmu
```

You also need:

- **Python 3.11 or newer**;
- **a C compiler** for your platform: gcc or clang on Linux, Xcode command
  line tools on macOS, Visual Studio 2022 on Windows.

CMake is installed together with the Python package, and ONNX Runtime is
downloaded automatically the first time you build an FMU.

## A first FMU in three steps

**1. Export a model to ONNX** and give its inputs and outputs names you will
recognise later.

```python
import torch
from torch import nn


class Sum(nn.Module):
    def forward(self, a, b):
        return a + b


torch.onnx.export(
    Sum(),
    (torch.zeros(3), torch.zeros(3)),
    "sum.onnx",
    input_names=["a", "b"],
    output_names=["y"],
)
```

**2. Describe it** in `sumDescription.json`. The `name` of each entry is the
name of a node in the ONNX graph.

```json
{
    "name": "sum",
    "description": "Adds two vectors of three elements.",
    "FMIVersion": "2.0",
    "inputs": [
        {"name": "a", "description": "First vector."},
        {"name": "b", "description": "Second vector."}
    ],
    "outputs": [
        {"name": "y", "description": "Element-wise sum."}
    ]
}
```

**3. Build the FMU.**

```bash
onnx2fmu build sum.onnx sumDescription.json build_tmp
```

This writes `sum.fmu` in the current folder. Each tensor is exposed
one element at a time, so the FMU has inputs `a_0`, `a_1`, `a_2`, `b_0`,
`b_1`, `b_2` and outputs `y_0`, `y_1`, `y_2`.

:::{warning}
The third argument is a scratch folder for the generated C project.
ONNX2FMU **deletes it** before generating and again after compiling, so never
point it at a folder that holds something you want to keep.
:::

## Where to go next

The **tutorials** are runnable notebooks, ordered from simple to advanced:

| Tutorial | What it shows |
|---|---|
| [Example 1](../examples/example1/example1.ipynb) | Scalar and vector inputs, labels, building and simulating an FMU. |
| [Example 2](../examples/example2/example2.ipynb) | The smallest complete workflow: several scalar inputs, one output. |
| [Example 3](../examples/example3/example3.ipynb) | A trained recurrent network with normalisation baked into the model. |
| [Example 4](../examples/example4/example4.ipynb) | Feedback states: a model that remembers its own past. |
| [Example 5](../examples/example5/example5.ipynb) | A neural ODE exported as a Model Exchange FMU. |

The **guide** explains the ideas behind the tool:

- [](concepts.md) covers how tensors become FMI variables, feedback states,
  and the difference between Co-Simulation and Model Exchange.
- [](how-it-works.md) follows a model through the code generator and the C
  runtime.

The **reference** pages are for looking things up:

- [](reference/cli.md)
- [](reference/python-api.md)
- [](reference/model-description.md)

## Compatibility

FMU generation is tested on Windows, Linux and macOS, on x86 and ARM
processors, for FMI 2.0 and FMI 3.0.

Generated FMUs have been imported and simulated with:

- [FMPy](https://github.com/CATIA-Systems/FMPy), which is part of the
  ONNX2FMU test suite;
- [Modelon Impact](https://modelon.com/modelon-impact/), with Linux
  Co-Simulation FMUs.

## Citing

If ONNX2FMU is useful in your research, please cite:

```bibtex
@inproceedings{urbani2025tool,
  title={A Tool for the Implementation of Open Neural Network Exchange Models in Functional Mockup Units},
  author={Urbani, Michele and Bolognese, Michele and Prattic{\`o}, Luca and Testi, Matteo},
  booktitle={Modelica Conferences},
  pages={645--651},
  year={2025},
  doi={https://doi.org/10.3384/ecp218645}
}
```

ONNX2FMU builds on the
[Reference FMUs](https://github.com/modelica/Reference-FMUs/) project of the
Modelica Association.
