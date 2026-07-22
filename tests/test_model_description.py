import copy
import json
import unittest
from math import prod
from onnx import TensorProto, helper, load
from pathlib import Path

from onnx2fmu.model_description import ModelDescription


class TestModelDescription(unittest.TestCase):

    def setUp(self) -> None:
        self.model_name = 'example4'
        self.base_dir = Path(__file__).resolve().parent.parent \
            / "examples" / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        self.onnx_model = load(self.model_path)
        self.model_description_path = \
            self.base_dir / f'{self.model_name}Description.json'
        self.model_description = \
            json.loads(self.model_description_path.read_text())

    def test_check_model_description(self):
        ModelDescription(
            onnx_model=self.onnx_model,
            model_description=self.model_description
        )

    def test_generate_context(self):
        m = ModelDescription(
            onnx_model=self.onnx_model,
            model_description=self.model_description
        )
        context = m.generateContext()
        values = [
            "name",
            "description",
            "FMIVersion",
            "inputs",
            "outputs"
        ]
        for v in values:
            self.assertIn(v, context)
        self.assertGreater(len(context["inputs"]), 0)
        self.assertGreater(len(context["outputs"]), 0)
        # Test number of elements matches node shapes
        FEATURES, TARGETS, T = 5, 3, 10
        shapes = {
            "u": (1, FEATURES),
            "U": (T - 1, FEATURES),
            "X": (T, TARGETS),
            "x": (1, TARGETS),
            "X1": (T, TARGETS),
            "U1": (T - 1, FEATURES)
        }
        for entry in ["inputs", "outputs"]:
            for node in context[entry]:
                self.assertEqual(
                    prod(shapes[node["name"]]),
                    len(node["scalarValues"])
                )
        for node in context["locals"]:
            self.assertEqual(
                prod(shapes[node["nameIn"]]),
                len(node["scalarValues"])
            )


def _build_test_onnx_model():
    """A tiny ONNX graph used only to exercise JSON-schema-level validation
    in ModelDescription (FMUType / time / derivativeOf). It does not need to
    represent real Van der Pol dynamics -- only expose nodes with the shapes
    the tests below reference: state candidate "x" (2,), time candidate "t"
    (1,), plain input "u" (3,), derivative candidate "dx" (2,), plain output
    "y" (3,).
    """
    x = helper.make_tensor_value_info("x", TensorProto.FLOAT, [2])
    t = helper.make_tensor_value_info("t", TensorProto.FLOAT, [1])
    u = helper.make_tensor_value_info("u", TensorProto.FLOAT, [3])
    dx = helper.make_tensor_value_info("dx", TensorProto.FLOAT, [2])
    y = helper.make_tensor_value_info("y", TensorProto.FLOAT, [3])
    # dt is only referenced by the "time input can't be a state" test, but is
    # always present in the graph for simplicity.
    dt = helper.make_tensor_value_info("dt", TensorProto.FLOAT, [1])
    n1 = helper.make_node("Add", ["x", "t"], ["dx"])
    n2 = helper.make_node("Identity", ["u"], ["y"])
    n3 = helper.make_node("Identity", ["t"], ["dt"])
    graph = helper.make_graph(
        [n1, n2, n3], "test_graph", [x, t, u], [dx, y, dt]
    )
    return helper.make_model(graph, producer_name="onnx2fmu-tests")


class TestModelExchangeValidation(unittest.TestCase):

    def setUp(self) -> None:
        self.onnx_model = _build_test_onnx_model()
        self.base_description = {
            "name": "vdp",
            "description": "test",
            "FMIVersion": "2.0",
            "inputs": [
                {"name": "x", "start": [2.0, 0.0]},
                {"name": "u"},
            ],
            "outputs": [
                {"name": "dx", "derivativeOf": "x"},
                {"name": "y"},
            ],
        }

    def test_default_fmu_type_is_co_simulation(self):
        description = copy.deepcopy(self.base_description)
        md = ModelDescription(self.onnx_model, description)
        self.assertEqual(description["FMUType"], ["CoSimulation"])
        context = md.generateContext()
        self.assertFalse(context["modelExchange"])
        self.assertTrue(context["coSimulation"])

    def test_explicit_model_exchange_and_co_simulation(self):
        description = copy.deepcopy(self.base_description)
        description["FMUType"] = ["ModelExchange", "CoSimulation"]
        md = ModelDescription(self.onnx_model, description)
        context = md.generateContext()
        self.assertTrue(context["modelExchange"])
        self.assertTrue(context["coSimulation"])
        self.assertEqual(context["numberOfContinuousStates"], 2)

    def test_invalid_fmu_type_entry_raises(self):
        description = copy.deepcopy(self.base_description)
        description["FMUType"] = ["NotAFMUType"]
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_empty_fmu_type_raises(self):
        description = copy.deepcopy(self.base_description)
        description["FMUType"] = []
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_model_exchange_with_locals_raises(self):
        description = copy.deepcopy(self.base_description)
        description["FMUType"] = ["ModelExchange"]
        # u (3,) -> y (3,): shapes match, so this is a valid `locals` entry
        # on its own; it is ME's incompatibility with `locals` that must
        # raise here, not a shape mismatch.
        description["locals"] = [{"nameIn": "u", "nameOut": "y"}]
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_derivative_of_unknown_input_raises(self):
        description = copy.deepcopy(self.base_description)
        description["outputs"][0]["derivativeOf"] = "does_not_exist"
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_state_without_start_raises(self):
        description = copy.deepcopy(self.base_description)
        del description["inputs"][0]["start"]
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_time_input_is_valid(self):
        description = copy.deepcopy(self.base_description)
        description["inputs"].append({"name": "t", "time": True})
        md = ModelDescription(self.onnx_model, description)
        context = md.generateContext()
        time_input = next(i for i in context["inputs"] if i["name"] == "t")
        self.assertTrue(time_input["isTime"])
        self.assertEqual(time_input["scalarValues"], [])

    def test_multiple_time_inputs_raise(self):
        description = copy.deepcopy(self.base_description)
        description["inputs"].append({"name": "t", "time": True})
        description["inputs"].append({"name": "u", "time": True})
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_time_input_cannot_be_a_state(self):
        description = copy.deepcopy(self.base_description)
        description["inputs"].append(
            {"name": "t", "time": True, "start": [0.0]}
        )
        description["outputs"].append({"name": "dt", "derivativeOf": "t"})
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)


if __name__ == "__main__":
    unittest.main()
