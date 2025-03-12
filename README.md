# ONNX2FMU: Encapsulate ONNX models in Functional Mock-up Units (FMUs)

## 🚀 Get started

### ONNX runtime

- Python 3.10+
- CMake 3.28+
- The default Windows CMake Generator is Visual Studio 2022.

## 📝 ONNX model declaration

Use `onnx.helper.set_model_props()` to set metadata.

Set input names and initial values as metadata properties of node.

### Causality of model variables

Variables of type `local` can be declared but they do not change during a
simulation. Indeed, the value of `local` variables must be defined *ad hoc* in
the model and cannot be defined automatically by ONNX2FMU.

## ONNX model generation

**Only co-simulation** FMUs can be generated.
