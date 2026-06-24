# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Fixed

- FMU size reduced by ~3× (from ~18 MB to ~6–7 MB) by shipping the ONNX
  Runtime shared library exactly once instead of three times. The previous
  CMake packaging dereferenced the version symlinks in the official ORT
  release (`libonnxruntime.so` and `libonnxruntime.so.1` both point to
  the real `libonnxruntime.so.1.x.y`), writing three full copies into the
  FMU archive. The model binary only needs `libonnxruntime.so.MAJOR`, so
  the build now copies that single file. On Windows, import libraries
  (`.lib`) and debug symbols (`.pdb`) are also dropped.

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
