---
title: Command line
description: Reference for the onnx2fmu command line interface.
---

Installing the package adds the `onnx2fmu` command to your shell. It has four
subcommands:

| Command | What it does |
|---|---|
| [`build`](#cli-build) | Generate the code and compile it. This is the one you need most of the time. |
| [`generate`](#cli-generate) | Only write the C project, without compiling. |
| [`compile`](#cli-compile) | Compile a project written by `generate`. |
| [`version`](#cli-version) | Print the installed version. |

Every command accepts `--help`.

(cli-build)=
## build

```bash
onnx2fmu build MODEL_PATH MODEL_DESCRIPTION_PATH TARGET_FOLDER [OPTIONS]
```

Builds an FMU from an ONNX model and a model description. It runs `generate`
followed by `compile`.

| Argument | Meaning |
|---|---|
| `MODEL_PATH` | Path to the ONNX model. |
| `MODEL_DESCRIPTION_PATH` | Path to the [model description](model-description.md) JSON file. |
| `TARGET_FOLDER` | Scratch folder where the C project is generated and compiled. |

| Option | Default | Meaning |
|---|---|---|
| `--destination` | `.` | Folder where the finished FMU is copied. You can also give a file path to choose the file name. |
| `--fmi-platform` | the current machine | Platform to build for. See [Target platforms](#target-platforms). |
| `--cmake-config` | `Release` | CMake build configuration, `Release` or `Debug`. |

The FMU is named after the `name` in the model description, not after the
ONNX file.

```bash
onnx2fmu build example1.onnx example1Description.json build_tmp --destination fmus/
```

:::{warning}
`TARGET_FOLDER` is deleted if it already exists, and deleted again once the
FMU has been copied to its destination. Use a folder that is not used for
anything else.
:::

(cli-generate)=
## generate

```bash
onnx2fmu generate MODEL_PATH MODEL_DESCRIPTION_PATH [TARGET_FOLDER]
```

Validates the model description against the ONNX graph and writes a complete
C project to `TARGET_FOLDER` (default: `target`). Nothing is compiled.

The folder contains the CMake project, the static runtime sources, and a
subfolder named after the model with the generated `model.c`, `config.h`, the
FMI model description and a copy of the ONNX model.

Use it when you want to inspect or modify the generated code before
compiling, or to check that a model description is valid without waiting for
a compiler.

:::{warning}
An existing `TARGET_FOLDER` is deleted first.
:::

(cli-compile)=
## compile

```bash
onnx2fmu compile TARGET_FOLDER MODEL_DESCRIPTION_PATH [OPTIONS]
```

Compiles a project created by `generate` and packages the FMU.

| Argument | Meaning |
|---|---|
| `TARGET_FOLDER` | The folder written by `generate`. |
| `MODEL_DESCRIPTION_PATH` | The same model description used for `generate`. |

The options `--destination`, `--fmi-platform` and `--cmake-config` are the
same as for [`build`](#cli-build).

`TARGET_FOLDER` is deleted after a successful build. If you edited the
generated sources and want to keep them, work on a copy.

The first build downloads ONNX Runtime, so it needs a network connection and
takes longer than the following ones.

(cli-version)=
## version

```bash
onnx2fmu version
```

Prints the installed ONNX2FMU version.

(target-platforms)=
## Target platforms

By default the FMU is built for the machine you are working on. To build for
another architecture, pass one of these values to `--fmi-platform`:

| Value | Notes |
|---|---|
| `x86_64-linux` | |
| `aarch64-linux` | Cross-compiled with the bundled toolchain file. Needs an `aarch64` cross compiler. |
| `x86_64-windows` | Visual Studio 2022. |
| `x86-windows` | 32-bit, Visual Studio 2022. |
| `x86_64-darwin` | Intel Macs. |
| `aarch64-darwin` | Apple Silicon. |

The operating system cannot be crossed: a Windows FMU has to be built on
Windows, a macOS FMU on macOS. Any other value is ignored and the FMU is
built for the current machine.

## Requirements

- A C compiler: gcc or clang on Linux, Xcode command line tools on macOS,
  Visual Studio 2022 on Windows.
- CMake, which is installed as a dependency of the Python package.
- A network connection for the first build, to download ONNX Runtime.
