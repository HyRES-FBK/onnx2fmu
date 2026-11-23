import copy
import json
import unittest
import warnings
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
        for node in context["feedbackStates"]:
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
                {"name": "y", "start": 0.0},
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

    def test_model_exchange_with_feedback_states_raises(self):
        description = copy.deepcopy(self.base_description)
        description["FMUType"] = ["ModelExchange"]
        # u (3,) -> y (3,): shapes match, so this is a valid
        # `feedbackStates` entry on its own; it is ME's incompatibility
        # with `feedbackStates` that must raise here, not a shape mismatch.
        description["feedbackStates"] = [{"nameIn": "u", "nameOut": "y"}]
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_co_simulation_with_feedback_and_continuous_states_raises(self):
        description = copy.deepcopy(self.base_description)
        description["FMUType"] = ["CoSimulation"]
        description["feedbackStates"] = [{"nameIn": "u", "nameOut": "y"}]
        with self.assertRaisesRegex(ValueError, "derivativeOf"):
            ModelDescription(self.onnx_model, description)

    def test_deprecated_locals_key_is_an_alias(self):
        description = copy.deepcopy(self.base_description)
        # Feedback states cannot be combined with continuous states.
        description["outputs"] = [{"name": "y", "start": 0.0}]
        description["locals"] = [{"nameIn": "u", "nameOut": "y"}]
        with self.assertWarns(FutureWarning):
            model_description = ModelDescription(self.onnx_model, description)
        context = model_description.generateContext()
        self.assertEqual(
            [var["name"] for var in context["feedbackStates"]], ["u_y"])
        self.assertNotIn("locals", context)

    def test_locals_and_feedback_states_together_raise(self):
        description = copy.deepcopy(self.base_description)
        description["locals"] = [{"nameIn": "u", "nameOut": "y"}]
        description["feedbackStates"] = [{"nameIn": "u", "nameOut": "y"}]
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


class TestOutputStartValues(unittest.TestCase):
    """A Co-Simulation output reports its start value until the first
    step, so leaving it out is deprecated."""

    def setUp(self) -> None:
        self.onnx_model = _build_test_onnx_model()
        self.description = {
            "name": "outputs",
            "description": "test",
            "FMIVersion": "2.0",
            "inputs": [{"name": "x", "start": [2.0, 0.0]}, {"name": "u"}],
            "outputs": [{"name": "y"}],
        }

    def future_warnings(self, description: dict) -> list:
        with warnings.catch_warnings(record=True) as caught:
            warnings.simplefilter("always")
            ModelDescription(self.onnx_model, description)
        return [w for w in caught if w.category is FutureWarning]

    def test_co_simulation_output_without_start_warns(self):
        description = copy.deepcopy(self.description)
        with self.assertWarnsRegex(FutureWarning, r"\['y'\].*start"):
            model_description = ModelDescription(self.onnx_model,
                                                 description)
        output = model_description.generateContext()["outputs"][0]
        self.assertEqual({s["start"] for s in output["scalarValues"]},
                         {"0.0"})

    def test_output_with_start_does_not_warn(self):
        description = copy.deepcopy(self.description)
        description["outputs"][0]["start"] = [1.0, 2.0, 3.0]
        self.assertEqual(self.future_warnings(description), [])

    def test_model_exchange_output_without_start_does_not_warn(self):
        description = copy.deepcopy(self.description)
        description["FMUType"] = ["ModelExchange"]
        self.assertEqual(self.future_warnings(description), [])

    def test_state_derivative_without_start_does_not_warn(self):
        description = copy.deepcopy(self.description)
        description["outputs"] = [{"name": "dx", "derivativeOf": "x"},
                                  {"name": "y", "start": 0.0}]
        self.assertEqual(self.future_warnings(description), [])


class TestStartValueShapes(unittest.TestCase):
    """A start list must broadcast to the shape of its ONNX node."""

    def setUp(self) -> None:
        self.onnx_model = _build_test_onnx_model()
        self.description = {
            "name": "shapes",
            "description": "test",
            "FMIVersion": "2.0",
            "inputs": [{"name": "x", "start": 1.0}, {"name": "u"}],
            "outputs": [{"name": "y", "start": "0.0"}],
            "feedbackStates": [{"nameIn": "t", "nameOut": "dt"}],
        }

    def assertInvalidStart(self, entry: str, index: int, start,
                           name: str) -> None:
        description = copy.deepcopy(self.description)
        description[entry][index]["start"] = start
        with self.assertRaisesRegex(ValueError, f"'{name}'"):
            ModelDescription(self.onnx_model, description)

    def test_broadcastable_starts_are_valid(self):
        description = copy.deepcopy(self.description)
        description["inputs"][0]["start"] = [2.0, 0.0]
        description["inputs"][1]["start"] = [1.0, 2.0, 3.0]
        description["outputs"][0]["start"] = [5.0]
        description["feedbackStates"][0]["start"] = [0.0]
        ModelDescription(self.onnx_model, description)

    def test_input_start_of_wrong_length(self):
        self.assertInvalidStart("inputs", 1, [1.0, 2.0], "u")

    def test_output_start_with_too_many_dimensions(self):
        self.assertInvalidStart("outputs", 0, [[1.0, 2.0, 3.0]] * 2, "y")

    def test_feedback_state_start_of_wrong_length(self):
        self.assertInvalidStart("feedbackStates", 0, [0.0, 1.0], "t")


class TestFeedbackStateInitial(unittest.TestCase):
    """A feedback state's `initial` is the FMI attribute of the same name:
    `exact` lets a master set the state before the end of initialization,
    `calculated` (the default) keeps it internal."""

    def context(self, fmi_version: str, **keys) -> dict:
        description = {
            "name": "initial",
            "description": "test",
            "FMIVersion": fmi_version,
            "inputs": [{"name": "u"}],
            "outputs": [{"name": "y", "start": 0.0}],
            "feedbackStates": [{"nameIn": "t", "nameOut": "dt", **keys}],
        }
        return ModelDescription(
            _build_test_onnx_model(), description).generateContext()

    def test_default_is_calculated(self):
        for fmi_version in ["2.0", "3.0"]:
            with self.subTest(fmi_version=fmi_version):
                context = self.context(fmi_version)
                self.assertEqual(
                    context["feedbackStates"][0]["initial"], "calculated")

    def test_exact(self):
        for fmi_version in ["2.0", "3.0"]:
            with self.subTest(fmi_version=fmi_version):
                context = self.context(fmi_version, initial="exact")
                self.assertEqual(
                    context["feedbackStates"][0]["initial"], "exact")

    def test_approx_raises(self):
        with self.assertRaisesRegex(ValueError, "'approx'"):
            self.context("2.0", initial="approx")


class TestStepSize(unittest.TestCase):
    """The internal step comes from `fixedInternalStepSize` (FMI 3.0 only)
    or from `defaultExperiment.stepSize`, the FMI 2.0 equivalent."""

    def setUp(self) -> None:
        self.onnx_model = _build_test_onnx_model()
        self.description = {
            "name": "stepper",
            "description": "test",
            "FMIVersion": "2.0",
            "inputs": [{"name": "u"}],
            "outputs": [{"name": "y", "start": 0.0}],
        }

    def context(self, **keys) -> dict:
        description = copy.deepcopy(self.description)
        description.update(keys)
        return ModelDescription(self.onnx_model, description).generateContext()

    def assertInvalid(self, **keys) -> None:
        description = copy.deepcopy(self.description)
        description.update(keys)
        with self.assertRaises(ValueError):
            ModelDescription(self.onnx_model, description)

    def test_defaults(self):
        context = self.context()
        self.assertEqual(context["fixedInternalStepSize"], 1)
        self.assertEqual(context["stepSize"], 1)
        self.assertEqual(context["startTime"], 0)
        self.assertEqual(context["stopTime"], 1)

    def test_fmi2_internal_step_is_the_default_experiment_step(self):
        context = self.context(defaultExperiment={
            "startTime": 10, "stopTime": 1000, "stepSize": 50})
        self.assertEqual(context["fixedInternalStepSize"], 50)
        self.assertEqual(context["stepSize"], 50)
        self.assertEqual(context["startTime"], 10)
        self.assertEqual(context["stopTime"], 1000)

    def test_fmi2_rejects_fixed_internal_step_size(self):
        self.assertInvalid(fixedInternalStepSize=50)

    def test_fmi3_fixed_internal_step_size(self):
        context = self.context(FMIVersion="3.0", fixedInternalStepSize=50)
        self.assertEqual(context["fixedInternalStepSize"], 50)
        self.assertEqual(context["stepSize"], 50)

    def test_fmi3_preferred_step_can_differ_from_internal_step(self):
        context = self.context(FMIVersion="3.0", fixedInternalStepSize=1,
                               defaultExperiment={"stepSize": 10})
        self.assertEqual(context["fixedInternalStepSize"], 1)
        self.assertEqual(context["stepSize"], 10)

    def test_fmi3_internal_step_falls_back_to_default_experiment(self):
        context = self.context(FMIVersion="3.0",
                               defaultExperiment={"stepSize": 0.5})
        self.assertEqual(context["fixedInternalStepSize"], 0.5)
        self.assertEqual(context["stepSize"], 0.5)

    def test_invalid_values(self):
        for step in [0, -1, "50", True, None]:
            with self.subTest(stepSize=step):
                self.assertInvalid(defaultExperiment={"stepSize": step})
            with self.subTest(fixedInternalStepSize=step):
                self.assertInvalid(FMIVersion="3.0",
                                   fixedInternalStepSize=step)
        self.assertInvalid(defaultExperiment={"startTime": 5, "stopTime": 5})
        self.assertInvalid(defaultExperiment={"startTime": "0"})
        self.assertInvalid(defaultExperiment={"tolerance": 1e-4})
        self.assertInvalid(defaultExperiment=[50])


if __name__ == "__main__":
    unittest.main()
