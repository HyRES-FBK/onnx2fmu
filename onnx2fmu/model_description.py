import numpy as np
from onnx import ModelProto, ValueInfoProto

from onnx2fmu.model import Model
from onnx2fmu.variables import Input, Output, Local


INPUTS = "inputs"
OUTPUTS = "outputs"
LOCALS = "locals"
FMU_TYPE = "FMUType"
MODEL_EXCHANGE = "ModelExchange"
CO_SIMULATION = "CoSimulation"
FMU_TYPES = [MODEL_EXCHANGE, CO_SIMULATION]
DEFAULT_FMU_TYPES = [CO_SIMULATION]

class ModelDescription:

    def __init__(self,
                 onnx_model: ModelProto,
                 model_description: dict,
                 ) -> None:
        self.onnx_model = onnx_model
        self.model_description = model_description
        self._checkModelDescription()
        self._checkFMUType()
        self._checkLocalVariableNodesShape()
        self._checkTimeInputs()
        self._checkStateDeclarations()

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
        for local in self.model_description.get(LOCALS, []):
            assert local["nameIn"] in input_nodes_names, \
                f"'{local['nameIn']}' is not one of the model input nodes {input_nodes_names}"
            assert local["nameOut"] in output_nodes_names, \
                f"'{local['nameOut']}' is not one of the model output nodes {output_nodes_names}"

    def _checkLocalVariableNodesShape(self):
        if self.model_description.get(LOCALS) is None:
            return
        for var in self.model_description[LOCALS]:
            node_in = self._findNode(var["nameIn"])
            node_out = self._findNode(var["nameOut"])
            node_shape_in = self._readNodeShape(node_in)
            node_shape_out = self._readNodeShape(node_out)
            assert node_shape_in == node_shape_out, \
                f"Local variable {var['nameIn']} shape {node_shape_in} and {var['nameOut']} shape {node_shape_out} are not consistent."

    def _checkFMUType(self) -> None:
        """Validate/normalize the top-level `FMUType` key.

        Optional; defaults to Co-Simulation only so descriptions written
        before Model Exchange support was added keep producing identical
        FMUs. Model Exchange is currently incompatible with `locals`
        (discrete feedback variables would advance at solver-internal
        evaluation rate, not once per communication step).
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
                len(self.model_description.get(LOCALS, [])) > 0:
            raise ValueError(
                "Model Exchange is not compatible with 'locals' (stateful "
                "feedback variables). Remove 'locals' or drop "
                "'ModelExchange' from FMUType."
            )
        self.model_description[FMU_TYPE] = fmu_type

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
                "'derivativeOf' target (a state)."
            )

    def _checkStateDeclarations(self) -> None:
        """Validate `derivativeOf` on `outputs` entries.

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

    def _checkStartValuesBroadcastability(self, variable: dict) -> None:
        if isinstance(variable["start"], (str, int, float)):
            return
        elif isinstance(variable["start"], list):
            start = np.array(variable["start"])
            node = self._findNode(variable["name"])
            node_shape = self._readNodeShape(node)
            if len(start.shape) > len(node_shape):
                raise ValueError(
                    f"Start values for entry {variable['name']} "
                    f"have shape {start.shape}, but node shape is "
                    f"{node_shape}. Start values must be "
                    "broadcastable to the node shape."
                )
            else:
                start_shape = \
                    (1, ) * (len(node_shape) - len(start.shape)) + \
                        start.shape
                np.broadcast_shapes(start_shape, node_shape)
        else:
            raise TypeError(
                f"Start values for variable {variable['name']} must be "
                "a string, int, float or a list, not "
                f"{type(variable['start'])}."
            )

    def _checkStartValueShapes(self) -> None:
        """
        Check that start values are either a scalar or a list of sclart, whose
        shape allows broadcasting to the variable shape.
        """
        for entry in [INPUTS, OUTPUTS, LOCALS]:
            for variable in self.model_description.get(entry, []):
                if "start" in variable:
                    self._checkStartValuesBroadcastability(variable=variable)

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
        entries = zip([INPUTS, OUTPUTS, LOCALS], [Input, Output, Local])
        for entry, cls in entries:
            for kwargs in self.model_description.get(entry, []):
                kwargs["fmiVersion"] = self.model_description["FMIVersion"]
                if cls is Local:
                    kwargs["shape"] = shapes[kwargs["nameIn"]]
                else:
                    kwargs["shape"] = shapes[kwargs["name"]]
                if cls is Input and kwargs["name"] in state_names:
                    # Derived, not a JSON key: this input is the state
                    # targeted by an output's `derivativeOf`.
                    kwargs["isState"] = True
                self.model.addVariable(cls(**kwargs))

    def _initializeModel(self) -> None:
        self.model = Model(
            name=self.model_description["name"],
            description=self.model_description["description"],
            fmiVersion=self.model_description["FMIVersion"],
            fmuType=self.model_description[FMU_TYPE]
        )

    def generateContext(self) -> dict:
        if getattr(self, "model", None) is None:
            self._parseModel()
        return self.model.generateContext()


if __name__ == "__main__":
    pass
