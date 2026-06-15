# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
