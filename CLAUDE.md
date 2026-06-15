# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Workflow rule

Any new feature or debugging issue must be developed on a new branch (the `master` branch is the PR target).

## Commands

Dependencies are managed with `uv`. Install everything (including dev deps: `torch`, `fmpy`, `pandas`, etc.) with:

```bash
uv sync --all-extras   # or: pip install ".[extra]"
```

Tests use the stdlib `unittest` runner. Run the full suite or a single test:

```bash
python -m unittest tests.test_app                          # one module
python -m unittest tests.test_app.TestExample1             # one test class
python -m unittest tests.test_app.TestExample1.test_compile  # one test
```

`test_variables`, `test_model`, and `test_model_description` are pure-Python unit tests (fast, no compiler).
`test_app` is heavier: its `test_compile*` / `test_compile_and_simulate` cases invoke CMake + a C compiler to build a real FMU and validate/simulate it with `fmpy`, so they require a working C toolchain and download ONNX Runtime on first run.

Build an FMU from the CLI (the package installs an `onnx2fmu` entry point):

```bash
onnx2fmu build <model.onnx> <modelDescription.json> <target_folder> [--fmi-platform ...] [--cmake-config Debug|Release]
onnx2fmu generate <model.onnx> <modelDescription.json> [target_folder]   # code only, no compile
onnx2fmu compile <target_folder> <modelDescription.json> [--fmi-platform ...]
onnx2fmu version
```

## Architecture

ONNX2FMU turns an ONNX model + a JSON model description into a co-simulation FMU. The pipeline has a **Python code-generation half** and a **C runtime half**.

### Python: parse → context → render (`onnx2fmu/`)

The whole Python side exists to produce a single `context` dict that gets rendered into templates. The data flow:

1. **`app.py`** — Typer CLI. `generate` loads the ONNX model and JSON description, builds the context, scaffolds the FMU folder, and renders every template in `template/` through Jinja2. `compile` shells out to `cmake` (configure + build) and zips the result into a `.fmu`. `build` = `generate` then `compile`. `build()` can also be imported and called directly from a training script.
2. **`model_description.py`** — `ModelDescription` validates the JSON against the actual ONNX graph: every declared input/output/local `name` must match a graph node, and `local` `nameIn`/`nameOut` node shapes must agree. It reads node shapes from the ONNX graph and instantiates `Input`/`Output`/`Local` variable objects, injecting the shape and FMI version into each.
3. **`variables.py`** — `VariableFactory` and its `Input`/`Output`/`Local` subclasses. Each variable expands its tensor `shape` into a flat list of FMI **scalar** variables (`scalarValues`), one per tensor element, with per-element names, optional `labels`, and broadcast `start` values. `causality` is derived from the class name. `Local` is special: it represents a feedback variable that wires an output node back into an input node on the next step, and carries both `nameIn`/`nameOut`.
4. **`model.py`** — `Model` aggregates the variable contexts, assigns sequential FMI `valueReference`s to every scalar, generates the GUID/timestamps, and emits the final context dict consumed by the templates.
5. **`config.py`** — Maps `onnx.TensorProto` dtypes to FMI 2.0 (`Real`/`Integer`/...) and FMI 3.0 (`Float32`/`Int32`/...) type names plus C types. Note: in FMI 2.0, ONNX `FLOAT`/`DOUBLE` both map to `Real`/`double`; FMI 3.0 distinguishes `Float32`/`Float64`. ONNX runtime requires float32, so doubles are downcast at the C boundary.

### Code generation templates (`onnx2fmu/template/`)

Jinja2 templates rendered with the model context: `model.c` (the generated model logic), `config.h`, `buildDescription.xml`, and one of `FMI2.xml`/`FMI3.xml` (selected by `FMIVersion`). The `FMI{2,3}.xml` becomes the FMU's `modelDescription.xml`.

### C runtime (`onnx2fmu/src/`, `onnx2fmu/include/`, `onnx2fmu/CMakeLists.txt`)

These are static C sources (derived from the Modelica Reference-FMUs project) copied verbatim into each generated project. `ortUtils.c` drives ONNX Runtime inference; `cosimulation.c` and the `fmi{1,2,3}Functions.c` files implement the FMI co-simulation interface. `CMakeLists.txt` downloads/caches a pinned ONNX Runtime release (`LATEST_RELEASE_TAG`), compiles the generated `model.c` against it, and packages the `.fmu` zip. The generated `model.onnx` is bundled in the FMU's `resources/` folder and loaded at runtime — the network is *not* compiled into C.

### Model description JSON

Top-level keys: `name` (also the FMU name), `description`, `FMIVersion` (`"2.0"` or `"3.0"`), `inputs`, `outputs`, and optional `locals`. Each input/output entry needs a `name` matching an ONNX graph node, plus optional `labels`, `description`, `variability` (`continuous` default, or `discrete`), and `start`. `locals` entries use `nameIn`/`nameOut` to define output→input feedback (used for RNN-style stateful models). See `README.md` and `examples/example1`–`example4` for worked cases, including PyTorch model declarations in the `generate-example-model.ipynb` notebooks.
