---
title: Model description
description: Reference for the JSON file that maps an ONNX model to an FMU.
---

The model description is a JSON file that tells ONNX2FMU how to present your
ONNX model as an FMU. This page lists every key. For the ideas behind them,
read [](../concepts.md) first.

## A complete example

```json
{
    "name": "heater",
    "description": "Predicts the temperature of a heated plate.",
    "FMIVersion": "3.0",
    "FMUType": ["CoSimulation"],
    "defaultExperiment": {"stopTime": 3600, "stepSize": 60},
    "inputs": [
        {
            "name": "u",
            "description": "Heating power.",
            "labels": ["left heater", "right heater"],
            "start": 0.0
        }
    ],
    "outputs": [
        {
            "name": "T",
            "description": "Plate temperature.",
            "start": 20.0
        }
    ],
    "feedbackStates": [
        {
            "nameIn": "h",
            "nameOut": "h_next",
            "description": "Hidden state of the network.",
            "start": 0.0
        }
    ]
}
```

## Top-level keys

| Key | Required | Meaning |
|---|---|---|
| `name` | yes | Name of the model and of the `.fmu` file. Characters other than letters, digits and underscore are removed. |
| `description` | yes | Free text stored in the FMU. |
| `FMIVersion` | yes | `"2.0"` or `"3.0"`, as a string. |
| `inputs` | yes | List of [input entries](#md-inputs-and-outputs). At least one. |
| `outputs` | yes | List of [output entries](#md-inputs-and-outputs). At least one. |
| `feedbackStates` | no | List of [feedback state entries](#md-feedback-states). |
| `FMUType` | no | List with `"CoSimulation"`, `"ModelExchange"`, or both. Defaults to `["CoSimulation"]`. |
| `defaultExperiment` | no | The FMI `DefaultExperiment`: start time, stop time and step size. See [Step size](#md-step-size). |
| `fixedInternalStepSize` | no | FMI 3.0 only. The internal step of a Co-Simulation FMU. See [Step size](#md-step-size). |
| `onnxRuntimeVersion` | no | ONNX Runtime release to bundle, for example `"1.21.0"`. Defaults to the release pinned by ONNX2FMU. |

`locals` is a deprecated alias of `feedbackStates`. It still works and prints
a warning. Using both keys in the same file is an error.

(md-inputs-and-outputs)=
## Inputs and outputs

Each entry of `inputs` and `outputs` describes one node of the ONNX graph.

| Key | Required | Applies to | Meaning |
|---|---|---|---|
| `name` | yes | both | Name of the node in the ONNX graph. Inputs must be graph inputs, outputs must be graph outputs. |
| `description` | no | both | Free text. Used as the description of every scalar variable that has no label. |
| `labels` | no | both | List of texts, one per tensor element in row-major order. Each replaces the description of its scalar variable. |
| `variability` | no | both | `"continuous"` (default) or `"discrete"`. |
| `start` | no | both | Start value: a number, or a list that can be broadcast to the node shape. Inputs default to `1.0`. For an output, it is the value a Co-Simulation FMU reports before its first step. Outputs default to `0.0`, and leaving `start` out of a Co-Simulation output is deprecated. |
| `time` | no | inputs | `true` to feed this input with the simulation time. See [Time inputs](#md-time-inputs). |
| `derivativeOf` | no | outputs | Name of the input this output is the time derivative of. See [Continuous states](#md-continuous-states). |

You do not have to list every node of the graph, but every graph input must
receive a value from somewhere: it has to appear in `inputs` or as the
`nameIn` of a feedback state.

Keys that are not in this table are rejected.

(md-feedback-states)=
## Feedback states

Each entry of `feedbackStates` connects an output node back to an input node.

| Key | Required | Meaning |
|---|---|---|
| `nameIn` | yes | Graph input that receives the value at the next step. |
| `nameOut` | yes | Graph output that produces the value. Must have the same shape as `nameIn`. |
| `description` | no | Free text. |
| `labels` | no | List of texts, one per element. |
| `variability` | no | `"continuous"` (default) or `"discrete"`. |
| `start` | no | Value of the state at the first step. Defaults to `1.0`. |

Nodes used in a feedback state must not be repeated under `inputs` or
`outputs`.

(md-time-inputs)=
## Time inputs

```json
{"name": "t", "time": true}
```

- The node must contain exactly one element.
- At most one input can be a time input.
- A time input cannot also be a continuous state.
- No FMI variable is generated for it.

(md-continuous-states)=
## Continuous states

```json
"inputs":  [{"name": "x", "start": [2.0, 0.0]}],
"outputs": [{"name": "dx", "derivativeOf": "x"}]
```

- `derivativeOf` must name an entry of `inputs`.
- The state and the derivative nodes must have the same shape.
- The state must declare `start`.
- Continuous states cannot be combined with `feedbackStates`.
- Use it together with `"FMUType": ["ModelExchange"]`, or with both types.

(md-step-size)=
## Step size

A Co-Simulation FMU runs the network once per internal step, inside
`doStep` only (see [](#co-simulation-steps)). Two keys set the
internal step. Both mirror FMI attributes, so each is accepted only where the
FMI version defines it.

```json
"defaultExperiment": {"startTime": 0, "stopTime": 3600, "stepSize": 60}
```

| Key | Meaning | Default |
|---|---|---|
| `startTime` | Default start time of a simulation. | `0` |
| `stopTime` | Default stop time. Must be greater than `startTime`. | `1` |
| `stepSize` | Preferred communication step. Also the internal step, unless `fixedInternalStepSize` is given. Must be positive. | the internal step |

These keys become the FMI `DefaultExperiment` element, which exists in FMI 2.0
and 3.0. Importing tools use it as their default settings. FMPy, for example,
takes `stepSize` as its output interval when you give none.

`fixedInternalStepSize` (a positive number) is an attribute of the FMI 3.0
`<CoSimulation>` element, which FMI 2.0 does not have. In an FMI 3.0
description it sets the internal step on its own, so the preferred
communication step can be a multiple of it:

```json
"FMIVersion": "3.0",
"fixedInternalStepSize": 1,
"defaultExperiment": {"stepSize": 10}
```

The internal step is `fixedInternalStepSize` if given, otherwise
`defaultExperiment.stepSize`, otherwise `1`. An FMI 3.0 FMU always declares it
in `fixedInternalStepSize`. An FMI 2.0 description that uses
`fixedInternalStepSize` is rejected: use `defaultExperiment.stepSize`.

## FMUType

| Value | Result |
|---|---|
| `["CoSimulation"]` | A Co-Simulation FMU. The default. |
| `["ModelExchange"]` | A Model Exchange FMU. |
| `["ModelExchange", "CoSimulation"]` | One FMU offering both interfaces. |

`"ModelExchange"` cannot be combined with `feedbackStates`.

## What the generated variables look like

| Declared as | Variable names | FMI causality |
|---|---|---|
| input | `<name>_<index>` | `input` |
| output | `<name>_<index>` | `output` |
| feedback state | `<nameIn>_<nameOut>_<index>` | `local` |
| continuous state | `<name>_<index>` | `output` |
| derivative of a state | `<name>_<index>` | `local` |
| time input | none | none |

`<index>` is the position of the element along each dimension, joined by
underscores: `_0`, `_1` for a vector, `_0_0`, `_0_1` for a matrix.

## Validation

ONNX2FMU checks the model description against the ONNX graph before it
generates any code. The most common messages are:

| Message | Cause |
|---|---|
| `Missing … in model description.` | A required top-level key is absent. |
| `'…' is not one of the model input nodes […]` | A `name` or `nameIn` does not match a graph input. The message lists the valid names. |
| `'…' is not one of the model output nodes […]` | Same, for outputs and `nameOut`. |
| `Feedback state … shape … and … shape … are not consistent.` | `nameIn` and `nameOut` have different shapes. |
| `Model Exchange is not compatible with 'feedbackStates'.` | Remove one of the two. |
| `Output '…' declares derivativeOf, which cannot be combined with 'feedbackStates'.` | Remove the feedback states or the continuous states. |
| `State '…' must declare a 'start' value` | A continuous state has no initial condition. |
| `Start values of '…' have shape …, which does not broadcast to the node shape …` | A `start` list does not fit its node. |
| `'fixedInternalStepSize' is an FMI 3.0 attribute …` | Use `defaultExperiment.stepSize` in an FMI 2.0 description. |
| `'defaultExperiment.…' must be …` | A `defaultExperiment` value is not a number, the step is not positive, or `stopTime` is not after `startTime`. |
| `Shape is empty.` | A node has no dimensions. Export it with shape `(1,)`. |

One message is a warning, not an error: `Outputs […] declare no 'start'
value.` It appears for a Co-Simulation FMU whose outputs have no `start`.
The FMU still builds, with outputs starting at `0.0`, but a future release
will reject such a description.

A quick way to list the node names of a model:

```python
import onnx

model = onnx.load("model.onnx")
print("inputs: ", [node.name for node in model.graph.input])
print("outputs:", [node.name for node in model.graph.output])
```
