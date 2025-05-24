from typing import Union
from onnx import ModelProto, ValueInfoProto

from onnx2fmu.model import Model
from onnx2fmu.variables import Input, Output, Local


INPUTS = "inputs"
OUTPUTS = "outputs"
LOCALS = "locals"

class ModelDescription:

    def __init__(self,
                 onnx_model: ModelProto,
                 model_description: dict,
                 ) -> None:
        self.onnx_model = onnx_model
        self.model_description = model_description
        self._checkModelDescription()
        self._checkLocalVariableNodesShape()

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
            assert input["name"] in input_nodes_names
        for output in self.model_description[OUTPUTS]:
            assert output["name"] in output_nodes_names
        for local in self.model_description.get(LOCALS, []):
            assert local["nameIn"] in input_nodes_names
            assert local["nameOut"] in output_nodes_names

    def _checkLocalVariableNodesShape(self):
        if self.model_description.get(LOCALS) is None:
            return
        for var in self.model_description[LOCALS]:
            node_in = self._findNode(var["nameIn"])
            node_out = self._findNode(var["nameOut"])
            node_shape_in = self._readNodeShape(node_in)
            node_shape_out = self._readNodeShape(node_out)
            assert node_shape_in == node_shape_out

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
        entries = zip([INPUTS, OUTPUTS, LOCALS], [Input, Output, Local])
        for entry, cls in entries:
            for kwargs in self.model_description.get(entry, []):
                if cls is Local:
                    kwargs["shape"] = shapes[kwargs["nameIn"]]
                else:
                    kwargs["shape"] = shapes[kwargs["name"]]
                self.model.addVariable(cls(**kwargs))

    def _initializeModel(self) -> None:
        self.model = Model(
            name=self.model_description["name"],
            description=self.model_description["description"],
            fmiVersion=self.model_description["FMIVersion"]
        )

    def generateContext(self) -> dict:
        if getattr(self, "model", None) is None:
            self._parseModel()
        return self.model.generateContext()


if __name__ == "__main__":
    pass
