import math
import warnings

import numpy as np
from onnx import ModelProto, ValueInfoProto

from onnx2fmu.model import Model, DEFAULT_START_TIME, DEFAULT_STOP_TIME
from onnx2fmu.variables import Input, Output, FeedbackState


INPUTS = "inputs"
OUTPUTS = "outputs"
FEEDBACK_STATES = "feedbackStates"
# Deprecated alias of FEEDBACK_STATES, kept for backward compatibility.
DEPRECATED_LOCALS = "locals"
FMU_TYPE = "FMUType"
MODEL_EXCHANGE = "ModelExchange"
CO_SIMULATION = "CoSimulation"
FMU_TYPES = [MODEL_EXCHANGE, CO_SIMULATION]
DEFAULT_FMU_TYPES = [CO_SIMULATION]
DEFAULT_EXPERIMENT = "defaultExperiment"
DEFAULT_EXPERIMENT_KEYS = ["startTime", "stopTime", "stepSize"]
# FMI 3.0 only: FMI 2.0 has no such attribute.
FIXED_INTERNAL_STEP_SIZE = "fixedInternalStepSize"


def _isNumber(value) -> bool:
    return isinstance(value, (int, float)) and not isinstance(value, bool) \
        and math.isfinite(value)


class ModelDescription:

    def __init__(self,
                 onnx_model: ModelProto,
                 model_description: dict,
                 ) -> None:
        self.onnx_model = onnx_model
        self.model_description = model_description
        self._renameDeprecatedLocals()
        self._checkModelDescription()
        self._checkFMUType()
        self._checkOutputStartValues()
        self._checkFeedbackStateNodesShape()
        self._checkStartValueShapes()
        self._checkTimeInputs()
        self._checkContinuousStateDeclarations()
        self._checkStepSize()

    def _renameDeprecatedLocals(self) -> None:
        """Accept the deprecated `locals` key as an alias of `feedbackStates`.

        The key was renamed because "local" is an FMI causality, which is
        not what these entries declare.
        """
        if DEPRECATED_LOCALS not in self.model_description:
            return
        if FEEDBACK_STATES in self.model_description:
            raise ValueError(
                f"Model description declares both '{FEEDBACK_STATES}' and "
                f"the deprecated '{DEPRECATED_LOCALS}'. Move every entry "
                f"to '{FEEDBACK_STATES}'."
            )
        warnings.warn(
            f"The '{DEPRECATED_LOCALS}' model description key is "
            f"deprecated and will be removed in a future release; rename "
            f"it to '{FEEDBACK_STATES}'.",
            FutureWarning,
            stacklevel=3,
        )
        self.model_description[FEEDBACK_STATES] = \
            self.model_description.pop(DEPRECATED_LOCALS)

    def _checkModelDescription(self) -> None:
        """Check that model description names and size match model nodes."""
        values = ["name", "FMIVersion", INPUTS, OUTPUTS]
        for v in values:
            if self.model_description.get(v) is None:
                raise ValueError(f"Missing {v} in model description.")
        if len(self.model_description[INPUTS]) == 0:
            raise ValueError("Inputs list is empty.")
        if len(self.model_description[OUTPUTS]) == 0:
            raise ValueError("Output list is empty.")
        input_nodes_names = [node.name for node in self.onnx_model.graph.input]
        output_nodes_names = [node.name for node
                              in self.onnx_model.graph.output]
        for input in self.model_description[INPUTS]:
            assert input["name"] in input_nodes_names, \
                f"'{input['name']}' is not one of the model input nodes {input_nodes_names}"
        for output in self.model_description[OUTPUTS]:
            assert output["name"] in output_nodes_names, \
                f"'{output['name']}' is not one of the model output nodes {output_nodes_names}"
        for feedback_state in self.model_description.get(FEEDBACK_STATES, []):
            assert feedback_state["nameIn"] in input_nodes_names, \
                f"'{feedback_state['nameIn']}' is not one of the model input nodes {input_nodes_names}"
            assert feedback_state["nameOut"] in output_nodes_names, \
                f"'{feedback_state['nameOut']}' is not one of the model output nodes {output_nodes_names}"

    def _checkFeedbackStateNodesShape(self):
        if self.model_description.get(FEEDBACK_STATES) is None:
            return
        for var in self.model_description[FEEDBACK_STATES]:
            node_in = self._findNode(var["nameIn"])
            node_out = self._findNode(var["nameOut"])
            node_shape_in = self._readNodeShape(node_in)
            node_shape_out = self._readNodeShape(node_out)
            assert node_shape_in == node_shape_out, \
                f"Feedback state {var['nameIn']} shape {node_shape_in} and {var['nameOut']} shape {node_shape_out} are not consistent."

    def _checkFMUType(self) -> None:
        """Validate/normalize the top-level `FMUType` key.

        Optional; defaults to Co-Simulation only so descriptions written
        before Model Exchange support was added keep producing identical
        FMUs. Model Exchange is currently incompatible with
        `feedbackStates` (they would advance at solver-internal evaluation
        rate, not once per communication step).
        """
        fmu_type = self.model_description.get(FMU_TYPE, DEFAULT_FMU_TYPES)
        if not isinstance(fmu_type, list) or len(fmu_type) == 0:
            raise ValueError(
                f"'{FMU_TYPE}' must be a nonempty list, a subset of "
                f"{FMU_TYPES}."
            )
        for t in fmu_type:
            if t not in FMU_TYPES:
                raise ValueError(
                    f"'{t}' is not an admissible FMU type. Admissible "
                    f"types are {FMU_TYPES}."
                )
        if MODEL_EXCHANGE in fmu_type and \
                len(self.model_description.get(FEEDBACK_STATES, [])) > 0:
            raise ValueError(
                "Model Exchange is not compatible with 'feedbackStates'. "
                "Remove 'feedbackStates' or drop 'ModelExchange' from "
                "FMUType."
            )
        self.model_description[FMU_TYPE] = fmu_type

    def _checkOutputStartValues(self) -> None:
        """Warn about Co-Simulation outputs that declare no `start` value.

        Co-Simulation runs the network only inside doStep, so an output
        reports its start value from the initialization until the first
        step. Model Exchange computes outputs at initialization, and state
        derivatives (`derivativeOf`) are not FMI outputs.
        """
        if CO_SIMULATION not in self.model_description[FMU_TYPE]:
            return
        missing = [
            out["name"] for out in self.model_description[OUTPUTS]
            if "start" not in out and not out.get("derivativeOf")
        ]
        if missing:
            warnings.warn(
                f"Outputs {missing} declare no 'start' value. A "
                "Co-Simulation FMU reports an output's start value from "
                "the initialization until its first step, and the default "
                "is 0.0. Declaring 'start' for every output will be "
                "required in a future release.",
                FutureWarning,
                stacklevel=3,
            )

    def _checkTimeInputs(self) -> None:
        """Validate the optional `"time": true` flag on `inputs` entries.

        A time input receives the FMI independent variable and is not
        exposed as an FMI variable, so it must be a single scalar, unique,
        and cannot also be a continuous state (`derivativeOf` target).
        """
        time_inputs = [
            inp for inp in self.model_description[INPUTS]
            if inp.get("time", False)
        ]
        if len(time_inputs) == 0:
            return
        if len(time_inputs) > 1:
            raise ValueError(
                "At most one input can be flagged as 'time': true."
            )
        time_input = time_inputs[0]
        node = self._findNode(time_input["name"])
        shape = self._readNodeShape(node)
        size = int(np.prod(shape)) if len(shape) > 0 else 1
        if size != 1:
            raise ValueError(
                f"Time input '{time_input['name']}' must have a total "
                f"size of 1, got shape {shape}."
            )
        derivative_targets = {
            out["derivativeOf"] for out in self.model_description[OUTPUTS]
            if out.get("derivativeOf")
        }
        if time_input["name"] in derivative_targets:
            raise ValueError(
                f"Time input '{time_input['name']}' cannot also be a "
                "'derivativeOf' target (a continuous state)."
            )

    def _checkContinuousStateDeclarations(self) -> None:
        """Validate `derivativeOf` (continuous state) declarations on `outputs`.

        The target must name an `inputs` entry, the ONNX node shapes of
        the state/derivative pair must match, and the state must declare
        a `start` value (its required initial condition).
        """
        input_names = {inp["name"]: inp
                       for inp in self.model_description[INPUTS]}
        for out in self.model_description[OUTPUTS]:
            derivative_of = out.get("derivativeOf")
            if not derivative_of:
                continue
            if derivative_of not in input_names:
                raise ValueError(
                    f"Output '{out['name']}' declares "
                    f"derivativeOf='{derivative_of}', but no input named "
                    f"'{derivative_of}' exists."
                )
            state_input = input_names[derivative_of]
            state_node = self._findNode(state_input["name"])
            derivative_node = self._findNode(out["name"])
            state_shape = self._readNodeShape(state_node)
            derivative_shape = self._readNodeShape(derivative_node)
            if state_shape != derivative_shape:
                raise ValueError(
                    f"State '{state_input['name']}' shape {state_shape} "
                    f"and derivative '{out['name']}' shape "
                    f"{derivative_shape} are not consistent."
                )
            if "start" not in state_input:
                raise ValueError(
                    f"State '{state_input['name']}' must declare a "
                    "'start' value (its initial condition)."
                )
            if len(self.model_description.get(FEEDBACK_STATES, [])) > 0:
                # In Co-Simulation one internal step would have to drive
                # both the forward Euler update and the feedback commit.
                raise ValueError(
                    f"Output '{out['name']}' declares derivativeOf, which "
                    f"cannot be combined with '{FEEDBACK_STATES}'. Remove "
                    f"'{FEEDBACK_STATES}' or the continuous states."
                )

    def _checkStepSize(self) -> None:
        """Validate `defaultExperiment` and `fixedInternalStepSize`.

        Both mirror FMI attributes. `defaultExperiment` is the FMI
        `DefaultExperiment` element, defined in FMI 2.0 and 3.0; its
        `stepSize` is the preferred communication step size.
        `fixedInternalStepSize` is an attribute of the FMI 3.0
        `<CoSimulation>` element that FMI 2.0 does not define, so FMI 2.0
        descriptions declare the step with `defaultExperiment.stepSize`.
        """
        experiment = self.model_description.get(DEFAULT_EXPERIMENT, {})
        if not isinstance(experiment, dict):
            raise ValueError(
                f"'{DEFAULT_EXPERIMENT}' must be an object with any of the "
                f"keys {DEFAULT_EXPERIMENT_KEYS}."
            )
        for key, value in experiment.items():
            if key not in DEFAULT_EXPERIMENT_KEYS:
                raise ValueError(
                    f"'{key}' is not a '{DEFAULT_EXPERIMENT}' key. Admissible "
                    f"keys are {DEFAULT_EXPERIMENT_KEYS}."
                )
            if not _isNumber(value):
                raise ValueError(
                    f"'{DEFAULT_EXPERIMENT}.{key}' must be a number, not "
                    f"{value!r}."
                )
        if "stepSize" in experiment and experiment["stepSize"] <= 0:
            raise ValueError(
                f"'{DEFAULT_EXPERIMENT}.stepSize' must be positive, not "
                f"{experiment['stepSize']}."
            )
        start_time = experiment.get("startTime", DEFAULT_START_TIME)
        stop_time = experiment.get("stopTime", DEFAULT_STOP_TIME)
        if stop_time <= start_time:
            raise ValueError(
                f"'{DEFAULT_EXPERIMENT}.stopTime' ({stop_time}) must be "
                f"greater than its startTime ({start_time})."
            )
        if FIXED_INTERNAL_STEP_SIZE not in self.model_description:
            return
        if self.model_description["FMIVersion"] != "3.0":
            raise ValueError(
                f"'{FIXED_INTERNAL_STEP_SIZE}' is an FMI 3.0 attribute and "
                "cannot be used in an FMI 2.0 model description. Declare the "
                f"step with '{DEFAULT_EXPERIMENT}': {{\"stepSize\": ...}} "
                "instead."
            )
        step = self.model_description[FIXED_INTERNAL_STEP_SIZE]
        if not _isNumber(step) or step <= 0:
            raise ValueError(
                f"'{FIXED_INTERNAL_STEP_SIZE}' must be a positive number, "
                f"not {step!r}."
            )

    def _checkStartValuesBroadcastability(self, start, node_name: str) -> None:
        if isinstance(start, (str, int, float)):
            return
        if not isinstance(start, list):
            raise TypeError(
                f"Start values of '{node_name}' must be a string, int, "
                f"float or a list, not {type(start)}."
            )
        # Dynamic dimensions (0) become 1, as in VariableFactory._setShape.
        node_shape = tuple(
            1 if dim == 0 else dim
            for dim in self._readNodeShape(self._findNode(node_name))
        )
        try:
            start_shape = np.array(start, dtype=np.float32).shape
            broadcast_shape = np.broadcast_shapes(start_shape, node_shape)
        except ValueError:
            start_shape, broadcast_shape = None, None
        if broadcast_shape != node_shape:
            raise ValueError(
                f"Start values of '{node_name}' have shape {start_shape}, "
                f"which does not broadcast to the node shape {node_shape}."
            )

    def _checkStartValueShapes(self) -> None:
        """Check that every start value is a scalar or a list that
        broadcasts to the shape of its node."""
        for entry, key in [(INPUTS, "name"), (OUTPUTS, "name"),
                           (FEEDBACK_STATES, "nameIn")]:
            for variable in self.model_description.get(entry, []):
                if "start" in variable:
                    self._checkStartValuesBroadcastability(
                        variable["start"], variable[key])

    def _findNode(self, nodeName: str) -> ValueInfoProto:
        entries = ["input", "output"]
        for entry_type in entries:
            for node in getattr(self.onnx_model.graph, entry_type):
                if node.name == nodeName:
                    return node
        raise ValueError(f"Node {nodeName} not found in ONNX model.")

    def _readNodeShape(self, node: ValueInfoProto) -> tuple:
        return tuple(dim.dim_value for dim in node.type.tensor_type.shape.dim)

    def _readNodesShape(self) -> dict:
        shapes = {}
        entries = ["input", "output"]
        for entry in entries:
            for node in getattr(self.onnx_model.graph, entry):
                shapes[node.name] = self._readNodeShape(node=node)
        return shapes

    def _parseModel(self) -> None:
        self._initializeModel()
        shapes = self._readNodesShape()
        state_names = {
            out["derivativeOf"] for out in self.model_description.get(OUTPUTS, [])
            if out.get("derivativeOf")
        }
        entries = zip([INPUTS, OUTPUTS, FEEDBACK_STATES], [Input, Output, FeedbackState])
        for entry, cls in entries:
            for kwargs in self.model_description.get(entry, []):
                kwargs["fmiVersion"] = self.model_description["FMIVersion"]
                if cls is FeedbackState:
                    kwargs["shape"] = shapes[kwargs["nameIn"]]
                else:
                    kwargs["shape"] = shapes[kwargs["name"]]
                if cls is Input and kwargs["name"] in state_names:
                    # Derived, not a JSON key: this input is the state
                    # targeted by an output's `derivativeOf`.
                    kwargs["isContinuousState"] = True
                self.model.addVariable(cls(**kwargs))

    def _initializeModel(self) -> None:
        experiment = self.model_description.get(DEFAULT_EXPERIMENT, {})
        step_size = experiment.get("stepSize")
        self.model = Model(
            name=self.model_description["name"],
            description=self.model_description["description"],
            fmiVersion=self.model_description["FMIVersion"],
            fmuType=self.model_description[FMU_TYPE],
            # FMI 3.0 may declare an internal step that differs from the
            # preferred communication step; otherwise they coincide.
            fixedInternalStepSize=self.model_description.get(
                FIXED_INTERNAL_STEP_SIZE, step_size),
            stepSize=step_size,
            startTime=experiment.get("startTime"),
            stopTime=experiment.get("stopTime"),
        )

    def generateContext(self) -> dict:
        if getattr(self, "model", None) is None:
            self._parseModel()
        return self.model.generateContext()


if __name__ == "__main__":
    pass
