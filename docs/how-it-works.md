---
title: How it works
description: A walk through the ONNX2FMU pipeline, from the model description to the compiled FMU.
---

You do not need this page to use ONNX2FMU. It is here for when you want to
understand an error message, customise the generated code, or contribute.

ONNX2FMU has two halves. A **Python code generator** reads your files and
writes a small C project. A **C runtime**, compiled by CMake, becomes the
binary inside the FMU and runs the network with ONNX Runtime.

```{figure} ../onnx2fmu_workflow.png
:alt: The ONNX model and the model description are validated, expanded into scalar variables, rendered into C and XML templates, then compiled and packaged into an FMU.
:width: 500px

The ONNX2FMU pipeline.
```

## The Python half: from two files to a C project

Everything on the Python side works towards one goal: a dictionary, called
the *context*, that holds all the facts the templates need. This is what
`onnx2fmu generate` does.

### 1. Validate

The model description is checked against the ONNX graph before anything is
written. ONNX2FMU verifies that:

- the required keys are present and there is at least one input and one
  output;
- every `name`, `nameIn` and `nameOut` is a real node of the graph, on the
  correct side (input or output);
- the two nodes of a feedback state have the same shape, and so do a state
  and its derivative;
- `FMUType`, time inputs and continuous states respect the rules described
  in [](concepts.md).

A mistake here stops the build with a message that names the offending entry,
which is much easier to act on than a compiler error later.

This step lives in `ModelDescription` (`onnx2fmu/model_description.py`).

### 2. Expand tensors into scalars

Each entry is turned into a variable object: `Input`, `Output` or
`FeedbackState` (`onnx2fmu/variables.py`). The object reads the node shape
and produces the flat list of scalar variables, one per tensor element, each
with its name, its label and its start value.

### 3. Build the context

`Model` (`onnx2fmu/model.py`) collects the variables and gives every scalar a
unique FMI *value reference*, the integer an importing tool uses to address
it. It pairs each continuous state with its derivative, generates a fresh
identifier for the FMU, and returns the context.

### 4. Render the templates

The context is rendered through the [Jinja2](https://jinja.palletsprojects.com/)
templates in `onnx2fmu/template/`:

| Template | Becomes |
|---|---|
| `model.c` | The model-specific C code: buffers, start values, the call to ONNX Runtime, getters and setters. |
| `config.h` | Compile-time settings such as the model identifier and which FMI interfaces are enabled. |
| `FMI2.xml` or `FMI3.xml` | The FMU's `modelDescription.xml`. |
| `buildDescription.xml` | Instructions for tools that compile the FMU from source. |

The rendered files are written to the target folder, next to a copy of the
static C sources, the CMake project and your ONNX model, renamed to
`resources/model.onnx`.

## The C half: from a C project to an FMU

This is what `onnx2fmu compile` does. It calls CMake twice, once to configure
and once to build.

### 5. Fetch ONNX Runtime

During configuration, CMake downloads a pinned release of
[ONNX Runtime](https://onnxruntime.ai/) for the target platform. The archive
is cached (in `/tmp/onnxruntime` on Linux and macOS, in
`%LOCALAPPDATA%\onnxruntime` on Windows), so only the first build needs the
network.

### 6. Compile and package

CMake compiles the generated `model.c` together with the static runtime:

- `ortUtils.c` opens an ONNX Runtime session and runs inference;
- `cosimulation.c` implements time stepping for Co-Simulation;
- `fmi2Functions.c` and `fmi3Functions.c` implement the functions of the FMI
  standard on top of the two.

The runtime is derived from the Modelica
[Reference FMUs](https://github.com/modelica/Reference-FMUs/).

The result is zipped into the `.fmu` archive, which contains:

```
modelDescription.xml      what the importing tool reads
binaries/<platform>/      the compiled model and the ONNX Runtime library
resources/model.onnx      your network
sources/                  the C sources, for tools that recompile
```

## What happens at simulation time

Two design choices are worth knowing about.

**The network is not translated to C.** The ONNX file travels inside the FMU
and is loaded by ONNX Runtime when the FMU is instantiated. Any operator
supported by the pinned ONNX Runtime release works, and the FMU computes
exactly what the ONNX model computes.

**Each FMU brings its own ONNX Runtime.** The library is shipped in the
`binaries` folder and loaded privately, so two FMUs built with different
ONNX Runtime versions can run in the same simulation without clashing.

Every time the model is evaluated, the FMU:

1. copies the current values of the FMI input variables, and of the feedback
   states, into 32-bit float buffers;
2. fills the time input, if there is one, with the current simulation time;
3. runs the ONNX Runtime session;
4. copies the results into the FMI output variables, and stores the feedback
   outputs for the next step.

In Co-Simulation this happens once per internal step. In Model Exchange it
happens whenever the solver asks for the derivatives.

## Customising the generated code

Because generation and compilation are separate commands, you can stop in
between:

```bash
onnx2fmu generate model.onnx modelDescription.json my_project
# edit my_project/<name>/model.c, add resources, ...
onnx2fmu compile my_project modelDescription.json
```

See the [CLI reference](reference/cli.md) for the details of each command.
