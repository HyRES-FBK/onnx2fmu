---
title: Python API
description: Build FMUs from a Python script or a notebook.
---

The commands of the [command line interface](cli.md) are ordinary Python
functions. Importing them lets you train a model, export it and build the FMU
in a single script or notebook, which is what the tutorials do.

```python
from onnx2fmu.app import build, generate, compile
```

## build

```python
build(
    model_path,
    model_description_path,
    target_folder,
    destination=".",
    fmi_platform="",
    cmake_config="Release",
)
```

Generates the C project and compiles it into an FMU. Equivalent to calling
[`generate`](#api-generate) and then [`compile`](#api-compile).

| Parameter | Meaning |
|---|---|
| `model_path` | Path to the ONNX model. |
| `model_description_path` | Path to the [model description](model-description.md) JSON file. |
| `target_folder` | Scratch folder for the generated project. **It is deleted before and after the build.** |
| `destination` | Folder, or file path, where the FMU is copied. Defaults to the current folder. |
| `fmi_platform` | Target platform, for example `"x86_64-linux"`. Empty means the current machine. See [Target platforms](#target-platforms). |
| `cmake_config` | `"Release"` or `"Debug"`. |

Paths can be strings or `pathlib.Path` objects. The function returns nothing:
when it finishes, the FMU is at `destination`.

A typical end of a training script looks like this:

```python
import json
import torch
from onnx2fmu.app import build

# 1. Export the trained model
torch.onnx.export(
    model,
    example_inputs,
    "controller.onnx",
    input_names=["u"],
    output_names=["y"],
)

# 2. Write the model description
description = {
    "name": "controller",
    "description": "Learned controller.",
    "FMIVersion": "3.0",
    "inputs": [{"name": "u"}],
    "outputs": [{"name": "y"}],
}
with open("controllerDescription.json", "w") as f:
    json.dump(description, f, indent=4)

# 3. Build the FMU
build(
    model_path="controller.onnx",
    model_description_path="controllerDescription.json",
    target_folder="build_tmp",
)
```

(api-generate)=
## generate

```python
generate(model_path, model_description_path, target_folder="target")
```

Validates the model description against the ONNX graph and writes the C
project to `target_folder`, without compiling. An existing `target_folder`
is deleted first.

It is a quick way to check a model description: if `generate` returns
without raising, the description is consistent with the model.

(api-compile)=
## compile

```python
compile(
    target_folder,
    model_description_path,
    destination=".",
    fmi_platform="",
    cmake_config="Release",
)
```

Compiles a project written by `generate` and copies the FMU to
`destination`. `target_folder` is deleted after a successful build.

:::{note}
Importing `compile` by name hides Python's built-in function of the same
name in your module. If that bothers you, use `from onnx2fmu import app` and
call `app.compile(...)`.
:::

## Errors

| Situation | What you get |
|---|---|
| The ONNX file or the model description does not exist | `ValueError` |
| A required key is missing, or a rule about `FMUType`, time inputs or continuous states is broken | `ValueError` with a message naming the entry |
| A `name`, `nameIn` or `nameOut` is not a node of the graph, or feedback shapes differ | `AssertionError` listing the available node names |
| An entry contains a key that ONNX2FMU does not know | `TypeError` |
| CMake or the C compiler fails | `subprocess.CalledProcessError`. The compiler output is written to the log. |
| CMake takes too long (10 minutes to configure, 30 to build) | `subprocess.TimeoutExpired` |

## Logging

ONNX2FMU logs through [loguru](https://loguru.readthedocs.io/). The CMake
commands are logged at `INFO` level and compiler output is logged only when
a build fails. To silence the messages in your script:

```python
from loguru import logger

logger.disable("onnx2fmu")
```

## Internal classes

`ModelDescription`, `Model`, `Input`, `Output` and `FeedbackState` do the
work behind `generate`. They are implementation details and may change
between releases. [](../how-it-works.md) describes what each one does.
