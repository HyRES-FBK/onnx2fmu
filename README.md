# ONNX2FMU: Encapsulate ONNX models in Functional Mock-up Units (FMUs)

**What ONNX2FMU does?** It wraps ONNX models into FMUs by generating the C code
and subsequently compiling it.

## 🚀 Get started

- Python 3.10+
- CMake 3.22+
- A code compiler for the host platform, which could be one of linux, windows
and macos.

The default Windows CMake Generator is Visual Studio 2022.

## 📝 ONNX model declaration

ONNX2FMU can handle models with multiple inputs and outputs as far as
1. the model description lists all inputs and outputs, and node names are
compliant with the node names in the ONNX model;
2. Input and output nodes have no 0 dimension, i.e., they have shapes like
`(N, )`, `(1, N)`, etc., with `N` being any tensor dimension.

### Model description file

A model description is declared in a JSON file and its schema includes the
following global items:

- `"name"` is the model name, which will also be the FMU archive name,
- `"description"` provides a generic description of the model,
- `"FMIVersion"` is the FMI standard version for generatign the FMU code and
the FMU binaries,
- `"inputs"` and `"outputs"` are the lists of inputs and output nodes in the
ONNX model.

Each entry of the the inputs and output lists is characterized by the following
schema:

- `"name"` must match the name of one of the model nodes, whereas
- `"names"` is the list of user-provided names for each of the node elements.
The number of names must match the number of elements in a given entry.
- `"description"` allows the user to attach a description to each of the
arrays.

The following is an example of a model description for a model with three
input nodes and one output node.

### Causality of model variables

Variables of type `local` can be declared but they do not change during a
simulation. Indeed, the value of `local` variables must be defined *ad hoc* in
the model and cannot be defined automatically by ONNX2FMU.

### State variables

State variables, which may usually be declare using variables with `local`
causality, are not handled by ONNX2FMU. This is because there is no standard
behaviour to rely on for their definition in the source code.

To handle state variables, the original ONNX model should be wrapped in model
that keep track of the state. The following example could help understanding
how to manage the presence of a state.

```python
class CompleteModel(nn.Module):

    def __init__(self):
        super(CompleteModel).__init__()
        self.fc1 = ...
        self.relu = ...

    def forward(self, X):
        X = self.fc1(X)
        X = self.relu(X)
        return X


class WrapperModel(nn.Module):

    def __init__(self, complete_model: CompleteModel, initial_state):
        super(WrapperModel).__init__()
        self.model = complete_model
        self.state = initial_state

    def forward(self, u):
        # Concatenate the control u with the state
        X = torch.stack((u, self.state))
        # Predic the state of the system at time t
        Y = self.model(X)
        # Part of the output Y is the state of the system at time t, which is
        # stored in self.state
        self.state = Y[:(N_state_elements)]
        # Return the complete output to be used in the rest of the simulation
        return Y

```

## ONNX model generation

**Only co-simulation** FMUs can be generated.
