---
title: Concepts
description: How ONNX tensors map to FMI variables, and what feedback states and continuous states are.
---

This page explains the handful of ideas you need to write a model description
with confidence. The exact syntax of every key is in the
[model description reference](reference/model-description.md).

## From tensors to scalar variables

An ONNX model works with tensors. FMI tools mostly work with named scalar
signals. ONNX2FMU bridges the two by exposing **one FMI variable per tensor
element**.

The variable name is the node name followed by the element index along each
dimension, joined by underscores:

| ONNX node | Shape | FMI variables |
|---|---|---|
| `u` | `(3,)` | `u_0`, `u_1`, `u_2` |
| `x` | `(1, 2)` | `x_0_0`, `x_0_1` |
| `input` | `(1, 200, 2)` | `input_0_0_0`, `input_0_0_1`, `input_0_1_0`, … |

Elements are listed in row-major order, the same order NumPy uses when it
flattens an array. These are the names you connect in your simulation tool
and the column names you use in an input file.

A few details are worth knowing:

- **Characters outside letters, digits and underscore are dropped** from node
  names. A node called `hidden.state` becomes `hiddenstate_0`, ….
- **Every node needs at least one dimension.** A zero-dimensional scalar is
  rejected, so export scalars with shape `(1,)`. In PyTorch that means
  `torch.tensor([0.0])` instead of `torch.tensor(0.0)`.
- **The model must be a single file.** ONNX2FMU copies only the `.onnx` file
  into the FMU. If your exporter writes the weights to a separate file, as
  PyTorch can do, ask it to keep them inline. With `torch.onnx.export` that
  is `external_data=False`.
- **Shapes must be fixed.** ONNX2FMU reads the shape from the graph when it
  generates the FMU. A dynamic dimension is treated as having size 1.

### Labels

Index-based names are precise but not very telling. With `labels` you attach a
human-readable text to each element. Labels are written into the
`description` of the FMI variable, so they show up next to the variable in
your tool's variable browser. The variable *name* stays index-based.

```json
{
    "name": "x",
    "labels": ["position", "velocity"]
}
```

(start-values)=
### Start values

Every variable needs a start value. You can give a single number, which is
applied to every element, or a list that NumPy can broadcast to the node
shape. ONNX2FMU checks the shape before it generates any code.

```json
{"name": "u", "start": 0.0}
{"name": "x", "start": [2.0, 0.0]}
```

If an input or a feedback state has no start value, ONNX2FMU uses `1.0`.
Start values matter most for [feedback states](#feedback-states), where they
are the content of the model's memory at the first step.

Outputs need a start value too. A Co-Simulation FMU runs the network only
inside a step (see [](#co-simulation-steps)), so from the initialisation until
its first step it reports the start values of its outputs. An output without
`start` gets `0.0` and a deprecation warning: a future release will require
the key.

### Data types

All inputs, outputs and feedback states are exposed as floating-point
variables: `Real` in FMI 2.0 and `Float32` in FMI 3.0. Continuous states and
their derivatives are `Float64` in FMI 3.0, because the FMI 3.0 state
functions require it.

Inside the FMU, values are converted to 32-bit floats before they are handed
to ONNX Runtime. In practice this means results carry about seven significant
digits. Keep it in mind when you compare an FMU against a double-precision
reference, or when you choose solver tolerances for a Model Exchange FMU.

(feedback-states)=
## Feedback states

An ONNX model is a pure function: it keeps nothing between two calls. Models
with memory, such as recurrent networks or autoregressive predictors, need
some of their output handed back as input at the next step.

A **feedback state** does exactly that. You export the model with the memory
as an explicit input *and* an explicit output, and you tell ONNX2FMU that the
two belong together:

```json
"feedbackStates": [
    {"nameIn": "h", "nameOut": "h_next", "start": 0.0}
]
```

At every internal step the FMU runs the network, takes the value of `h_next`
and stores it. At the following step, that stored value is what the network
receives as `h`. The user of the FMU never has to wire the loop by hand, and
the memory advances exactly once per step, whatever the importing tool does
between two steps (see [](#co-simulation-steps)).

```{mermaid}
flowchart LR
    u([input u]) --> net[ONNX model]
    net --> y([output y])
    net -- h_next --> mem[(stored by the FMU)]
    mem -- h, next step --> net
```

Some rules follow from this:

- `nameIn` must be an input node and `nameOut` an output node of the graph.
- Both nodes must have the same shape.
- The two names must differ, because ONNX does not allow two nodes with the
  same name.
- Feedback nodes are **not** listed under `inputs` or `outputs`.

In the FMU, a feedback state appears as variables named
`<nameIn>_<nameOut>_<index>`, for example `h_h_next_0`. They are visible, so
you can plot them, but they are not meant to be connected to other models.

[Example 4](../examples/example4/example4.ipynb) builds a model with two
feedback states step by step.

(co-simulation-and-model-exchange)=
## Co-Simulation and Model Exchange

FMI defines two ways for a tool to run an FMU, and ONNX2FMU can generate
either one or both in the same archive. You choose with the `FMUType` key.

**Co-Simulation** is the default. The FMU advances in time on its own: the
importing tool sets the inputs, asks the FMU to take a step, and reads the
outputs. This fits networks that map inputs directly to outputs, with or
without feedback states. Examples 1 to 4 are Co-Simulation FMUs.

**Model Exchange** is for networks that compute a *time derivative*. The FMU
only answers the question "given this state and this time, what is
`dx/dt`?", and the importing tool integrates the answer with its own solver.
This is the natural home for a neural ODE.
[Example 5](../examples/example5/example5.ipynb) is a Model Exchange FMU.

(co-simulation-steps)=
### How a Co-Simulation FMU steps

A Co-Simulation FMU keeps its own time grid with a fixed **internal step**
`h`, and runs the network **once per internal step, inside `doStep`, and
nowhere else**. Setting inputs, reading outputs and initialising the FMU never
run the network. So the results do not depend on the order in which the
importing tool calls the FMU, and a feedback state advances exactly once per
step.

Each internal step `[t, t + h]` runs the network on:

- the time `t` at the start of the step, for a [time input](#time-as-an-input);
- the inputs as they were at `t`;
- the feedback states.

It then stores the outputs and the next feedback states. An output read at
`t + h` therefore holds the network's answer for the inputs at `t`. Before
the first step, outputs hold their [start values](#start-values).

Design the network so that one call corresponds to one internal step, and
declare the step in the model description:

```json
"defaultExperiment": {"stopTime": 3600, "stepSize": 60}
```

`defaultExperiment` mirrors the FMI `DefaultExperiment` element. Its
`stepSize` is the FMU's internal step, 1 by default. It is written into the
FMU as the preferred communication step, and FMPy uses it when you give no
output interval. FMI 3.0 descriptions can also set the internal step on its
own, with `"fixedInternalStepSize"`. That attribute exists only in FMI 3.0,
so an FMI 2.0 description that uses it is rejected.

The importing tool does not have to step with `h`. A `doStep` runs every
internal step that ends within it. A shorter communication step leaves the
outputs unchanged until the internal step it falls in is complete. For
example, FMPy shortens a step when an input changes in the middle of it.
With `h = 1`, a step from 2 to 2.5 runs nothing. The next step, from 2.5 to
3, completes the internal step `[2, 3]`, still on the inputs at 2.

Because a Co-Simulation output does not change when an input is set at a
communication point, an FMU that offers only Co-Simulation declares
`dependencies=""` on its outputs and initial unknowns in the FMI
`ModelStructure`. FMI defines the empty list this way, in FMI 2.0.4
(section 2.2.8, Definition of the Model Structure):

> If not present, it must be assumed that the `Unknown` depends on all
> `Knowns`. If present as empty list, the `Unknown` depends on none of the
> `Knowns`.

and in the same words in FMI 3.0.2 (section 2.4.8, Model Structure): "If
dependencies is present as empty list, the unknown depends on none of the
knowns." At a communication point, the knowns of an output include its
inputs, and in Initialization Mode the knowns of an initial unknown include
the inputs and the start values. FMI 2.0.4 recommends the empty list for
the latter case: "For standard Co-Simulation FMUs, it is common that no
algebraic loops over the input/output variables nor over start-values is
supported. In such a case, all `dependencies` lists for output variables
under the `InitialUnknowns` element should be defined as empty lists
defining that the setting of inputs and/or of start values does not
influence the outputs."

So the importing tool knows that the outputs have no direct feedthrough from
the inputs, and connecting FMUs in a loop does not create an algebraic loop.
An FMU that also offers Model Exchange keeps the default, with no attribute
(every output depends on all knowns), because its Model Exchange outputs do
follow the inputs.

(continuous-states)=
### Continuous states

A **continuous state** is a quantity the solver integrates. In the model
description you declare it by pointing an output at the input it is the
derivative of:

```json
"inputs":  [{"name": "x", "start": [2.0, 0.0]}],
"outputs": [{"name": "dx", "derivativeOf": "x"}]
```

This tells ONNX2FMU that `x` is the state and `dx` is `dx/dt`. Three things
follow:

- The state **must have a `start` value**. It is the initial condition.
- The state and its derivative must have the same shape.
- In the FMU the state is no longer an input. It is exposed as an output that
  the solver updates, and the derivative becomes an internal variable linked
  to it.

(time-as-an-input)=
### Time as an input

Some dynamics depend on time explicitly, for example a system with a periodic
forcing term. Mark the corresponding input with `"time": true`:

```json
{"name": "t", "time": true}
```

ONNX2FMU then feeds the simulation clock into that node at every evaluation:
the solver's current time in Model Exchange, the start of the internal step
in Co-Simulation. No FMI variable is created for it. A model can have one
time input, and it must hold a single element.

### What you cannot combine

Model Exchange cannot be combined with feedback states. A solver calls the
model many times per step, at trial points it may later discard, so a memory
that advances at every call would lose its meaning. ONNX2FMU refuses such a
description with an error. For the same reason, feedback states cannot be
combined with continuous states in any FMU type.

You *can* ask for both interfaces with
`"FMUType": ["ModelExchange", "CoSimulation"]`. In that case the
Co-Simulation side integrates the continuous states itself, with a simple
forward Euler scheme on the [internal step](#co-simulation-steps). That is
much coarser than an adaptive solver, so prefer the Model Exchange interface
when accuracy matters.

## Three words that sound alike

| Term | What it is | How you declare it |
|---|---|---|
| **Feedback state** | An output that is fed back into an input at the next step. An ONNX2FMU concept. | An entry of `feedbackStates`. |
| **Continuous state** | A quantity integrated by the importing tool's solver. An FMI Model Exchange concept. | An input that an output points to with `derivativeOf`. |
| **`local` causality** | The FMI label for a variable that is internal to the FMU. | You do not declare it. ONNX2FMU assigns it to feedback states and to state derivatives. |

Older model descriptions use the key `locals` for feedback states. It still
works, but it prints a deprecation warning. Rename it to `feedbackStates`.
