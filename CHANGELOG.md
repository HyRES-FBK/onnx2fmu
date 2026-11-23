# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Fixed

- **A memory leak on every network run**
  ([#62](https://github.com/HyRES-FBK/onnx2fmu/issues/62)). The generated
  FMU never released the output tensors that ONNX Runtime allocates on each
  run: it released copies of their pointers, which were still `NULL`. It
  also never released a tensor shape object per output, which it did not
  use. An FMU lost about 0.7 KB per output and feedback state on every run,
  about 2.3 KB per step for example 4, so long simulations ran out of
  memory. The FMU now releases both.

- **Co-Simulation FMUs with feedback states advance once per step, in
  `doStep` only** ([#59](https://github.com/HyRES-FBK/onnx2fmu/issues/59)).
  The network used to run, and feedback states to advance, when the importer
  read an output or left initialization, so results depended on the order
  of the importer's calls. Under `fmpy.simulate_fmu` a stateful FMU ran one
  step ahead. Depending on the importer, the state could also advance twice
  in one step, or not at all.

### Changed

- In Co-Simulation the network runs only inside `doStep`, once per internal
  step, on the time and inputs at the start of the step. Setting inputs,
  reading outputs and initialization never run it, also in FMUs without
  feedback states. An output read after a step holds what that step
  computed, and before the first step it holds its `start` value. Model
  Exchange still evaluates the network on demand.
- An FMU that offers only Co-Simulation declares `dependencies=""` on its
  outputs and initial unknowns, telling importers that its outputs have no
  direct feedthrough from the inputs.
- `start` values are checked against the shape of their ONNX node before
  any code is generated.
- A description that combines `"feedbackStates"` with `"derivativeOf"` is
  rejected for every `"FMUType"`, not only for Model Exchange.
- **Breaking:** feedback states are internal by default
  ([#60](https://github.com/HyRES-FBK/onnx2fmu/issues/60)). They are
  emitted with `initial="calculated"` and no `start` attribute, and the FMU
  refuses to set them. FMI 2.0 FMUs used to emit them with
  `initial="exact"`, so an importer could overwrite them before
  initialization. To keep that, add `"initial": "exact"` to the
  feedback-state entry.
- No feedback state can be set after initialization, also with
  `"initial": "exact"`. The FMU used to accept such a set at any time. FMI
  allows a `local` variable to be set only before the end of
  initialization, and since #59 only `doStep` moves a feedback state forward.

### Added

- `"defaultExperiment"` in the model description, mirroring the FMI
  `DefaultExperiment` element (`startTime`, `stopTime`, `stepSize`). Its
  `stepSize` sets the internal step of a Co-Simulation FMU, which was fixed
  at 1.
- `"fixedInternalStepSize"` for FMI 3.0 descriptions. It sets the internal
  step on its own and is written to the `<CoSimulation>` attribute of the
  same name. FMI 2.0 has no such attribute, so FMI 2.0 descriptions that use
  it are rejected.
- `"initial"` on feedback states
  ([#60](https://github.com/HyRES-FBK/onnx2fmu/issues/60)), the FMI
  attribute of the same name. With `"exact"`, an importer may set the state
  before the end of initialization, so one FMU can start each run from a
  different state. With `"calculated"`, the default, the state is internal
  and always starts from its `start` value. Use the default for states
  without a physical meaning, such as an RNN context vector. FMI 3.0 FMUs now
  emit `initial` and, for `"exact"` states, `start` on feedback states.

### Deprecated

- Co-Simulation outputs without a `start` value. They now report `start`
  until the first step, default to `0.0` and raise a `FutureWarning`. A
  future release will require `start` on every output.

## [0.4.0] - 2026-11-11

This release adds Model Exchange FMUs, a documentation site, and a clearer
name for feedback states. Python 3.10 is no longer supported.

The same code was published by mistake as 1.0.0 on the same day. That
release is withdrawn: use 0.4.0 instead.

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

[0.4.0]: https://github.com/HyRES-FBK/onnx2fmu/compare/v0.3.3...v0.4.0

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
