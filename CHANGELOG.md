# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.0.0] - 2026-11-11

First stable release. It adds Model Exchange FMUs, a documentation site, and
a clearer name for feedback states. Python 3.10 is no longer supported.

### Added

- **Model Exchange FMUs.** A network whose outputs are time derivatives
  (a "neural ODE") can be exported as a Model Exchange FMU, so that the
  importing tool integrates it with its own solver. Three model description
  keys support this: the top-level `"FMUType"` (a list with
  `"CoSimulation"`, `"ModelExchange"`, or both; defaults to
  `["CoSimulation"]`, so existing descriptions produce the same FMU),
  `"derivativeOf"` on an output to declare it the derivative of an input
  (which becomes a continuous state and must declare `"start"`), and
  `"time": true` on an input to feed it the simulation time. Model Exchange
  cannot be combined with `"feedbackStates"`.
- `examples/example5`: a neural ODE trained with torchdiffeq on a forced
  Van der Pol oscillator and exported as a Model Exchange FMU.
- **Documentation site** at <https://hyres-fbk.github.io/onnx2fmu/>, built
  with Jupyter Book 2 from `docs/` and published to GitHub Pages by the new
  `docs.yml` workflow. It contains a quickstart, the five examples as
  tutorials, a guide to the concepts and the pipeline, and reference pages
  for the command line, the Python API and the model description.
- A `docs` optional dependency group with the tools needed to build the
  documentation.
- Python 3.13 support.

### Changed

- The model description section declaring output→input feedback (used for
  RNN-style stateful models) is renamed from `"locals"` to
  `"feedbackStates"`, and the `Local` variable class to `FeedbackState`.
  "Local" is an FMI causality, not a kind of model description entry, and
  the old name had become ambiguous. Generated FMUs are unchanged: feedback
  states are still emitted with `causality="local"` and the same variable
  names. The README gains a terminology section distinguishing feedback
  states, continuous states, and FMI `local` causality.
- The example notebooks are renamed from `generate-example-model.ipynb` to
  `exampleN.ipynb` and rewritten as step-by-step tutorials. Each one now
  builds the FMU, simulates it with FMPy and compares the result with the
  PyTorch model. Example 3 loads its trained weights by default instead of
  retraining.
- cmake output is captured instead of printed, and shown only when a build
  fails. The configure and build steps are bounded by timeouts (10 and 30
  minutes), so a stalled download or compiler fails instead of hanging.
- The ONNX Runtime download is bounded by a timeout and safe to run from
  several builds at once.
- Development dependencies updated to fix security alerts (among them
  torch, onnx, protobuf, tornado, filelock and setuptools). The minimum
  `onnxscript` in the `extra` group is raised to 0.7.2, which the ONNX
  exporter of the pinned torch release requires.

### Deprecated

- The `"locals"` model description key. It is still accepted as an alias of
  `"feedbackStates"` but emits a `FutureWarning`; declaring both keys is an
  error.

### Removed

- Python 3.10 support. ONNX2FMU now requires Python 3.11 or newer.

### Fixed

- The ONNX Runtime library bundled in each FMU is now renamed with the model
  name (e.g. `libMyModel_onnxruntime.so`) and loaded explicitly at runtime
  (`dlopen`/`LoadLibraryEx`) instead of being linked against by its generic
  name (`onnxruntime`). Previously, two FMUs built by `onnx2fmu` — or an FMU
  and an FMI importer that itself embeds ONNX Runtime — could each ship a
  different ONNX Runtime version under the same library filename; loading
  both into the same process risked the OS module loader resolving to the
  wrong copy, causing a version mismatch. The model binary's own dependency
  metadata no longer references ONNX Runtime at all (verified with
  `ldd`/`otool -L`/`dumpbin /dependents`), so this collision can no longer
  happen. Fixes #53.
- FMU size reduced by ~3× (from ~18 MB to ~6–7 MB) by shipping the ONNX
  Runtime shared library exactly once instead of three times. The previous
  CMake packaging dereferenced the version symlinks in the official ORT
  release (`libonnxruntime.so` and `libonnxruntime.so.1` both point to
  the real `libonnxruntime.so.1.x.y`), writing three full copies into the
  FMU archive. The model binary only needs `libonnxruntime.so.MAJOR`, so
  the build now copies that single file. On Windows, import libraries
  (`.lib`) and debug symbols (`.pdb`) are also dropped.
- An intermittent deadlock when an FMU instance was freed. The ONNX Runtime
  objects were released in the wrong order; they are now released session
  first and environment last.
- CMake architecture detection on Apple Silicon. A build without
  `--fmi-platform` failed with "Unknown System Architecture: arm64".
- An empty ONNX Runtime archive, left behind when a firewall intercepts the
  download, is now detected and removed, and the error message gives the
  URL for a manual download and the expected cache path. Fixes #35.

[1.0.0]: https://github.com/HyRES-FBK/onnx2fmu/compare/v0.3.3...v1.0.0

## [0.3.3] - 2026-06-18

### Fixed

- Excluded large binary artifacts (prebuilt FMUs, ONNX Runtime shared
  libraries, ONNX models, logos) from the PyPI sdist via
  `[tool.flit.sdist]` exclude patterns. The sdist previously exceeded
  PyPI's 60 MB upload limit, causing the release to fail.

[0.3.3]: https://github.com/HyRES-FBK/onnx2fmu/compare/v0.3.2...v0.3.3

## [0.3.2] - 2026-06-18

### Changed

- ONNX Runtime pinned version updated to 1.21.0.
- macOS CI runner migrated from the deprecated `macos-13` (Intel) runner to
  `macos-15` (Apple Silicon / ARM64).

### Fixed

- `test_compile_and_simulate` updated to call the correctly renamed method.

[0.3.2]: https://github.com/HyRES-FBK/onnx2fmu/compare/v0.3.1...v0.3.2

## [0.3.1] - 2026-06-18

### Fixed

- Model names containing underscores now correctly produce a matching
  folder, `modelIdentifier`, and DLL name. Previously `Model._setName`
  stripped underscores (regex `[^a-zA-Z0-9]`), causing `generate()` to
  create a sanitized folder (e.g. `mymodel/`) while `compile()` passed
  the raw JSON name to CMake (e.g. `my_model`). CMake could not find the
  source files and, when a build did succeed, the DLL name diverged from
  `modelIdentifier` so the FMU simulator could not load it at runtime.
  Underscores are now preserved in both `Model._setName` and `compile()`.

[0.3.1]: https://github.com/HyRES-FBK/onnx2fmu/compare/v0.3.0...v0.3.1

## [0.3.0] - 2026-06-15

### Changed

- Refactored the generated FMU's tensor handling: ONNX input and local
  variable tensors are now allocated once into a persistent `ModelBuffers`
  struct instead of being allocated and freed on every `calculateValues`
  step. `calculateValues` now only updates the buffer contents and runs
  inference, reducing per-step allocation overhead.
- Renamed the `dev` dependency group to an `extra` optional-dependencies
  group, installable via `pip install ".[extra]"` or `uv sync --all-extras`.

### Added

- New `initModel`/`freeModel` lifecycle functions that allocate and release
  the persistent input/local tensors and their ONNX memory info. They are
  wired into `createModelInstance` and `freeModelInstance`.
- `ModelBuffers` struct in the generated `config.h` holding the persistent
  `float*` buffers, `OrtValue*` tensors, and `OrtMemoryInfo*`.
- `CLAUDE.md` with project guidance for working in this repository.

[0.3.0]: https://github.com/HyRES-FBK/onnx2fmu/releases/tag/v0.3.0
