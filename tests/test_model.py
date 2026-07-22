import unittest

from onnx2fmu.model import Model
from onnx2fmu.variables import Input, Output, Local


class TestModel(unittest.TestCase):

    def setUp(self) -> None:
        self.model = Model(
            name="example:</Model"
        )

    def test_name(self):
        self.assertEqual(self.model.name, "exampleModel")

    def test_name_with_underscore(self):
        model = Model(name="my_model")
        self.assertEqual(model.name, "my_model")

    def test_add_variable(self):
        v = Input(name="x", shape=(2, 3))
        self.model.addVariable(v)
        values = []
        for input in self.model.inputs:
            scalars = input["scalarValues"]
            values += [scalar["valueReference"] for scalar in scalars]
        self.assertEqual(len(set(values)), len(values))

    def add_example_variables(self):
        v = Input(name="x", shape=(2, 3))
        self.model.addVariable(v)
        self.assertGreaterEqual(len(self.model.inputs), 1)
        v = Output(name="y", shape=(3, 4))
        self.model.addVariable(v)
        self.assertGreaterEqual(len(self.model.outputs), 1)
        v = Local(nameIn="z1", nameOut="z2", shape=(4, 5))
        self.model.addVariable(v)
        self.assertGreaterEqual(len(self.model.locals), 1)

    def test_add_variables(self):
        self.add_example_variables()
        values = []
        variables = self.model.inputs + self.model.outputs + self.model.locals
        for input in variables:
            scalars = input["scalarValues"]
            values += [scalar["valueReference"] for scalar in scalars]
        self.assertEqual(len(set(values)), len(values))

    def test_generate_context(self):
        with self.assertRaises(ValueError):
            self.model.generateContext()
        self.test_add_variables()
        context = self.model.generateContext()
        self.assertIn("x", [var["name"] for var in context["inputs"]])
        self.assertIn("y", [var["name"] for var in context["outputs"]])
        self.assertIn("z1_z2", [var["name"] for var in context["locals"]])


class TestModelFMUType(unittest.TestCase):

    def test_default_is_co_simulation_only(self):
        model = Model(name="m")
        self.assertFalse(model.modelExchange)
        self.assertTrue(model.coSimulation)

    def test_model_exchange_only(self):
        model = Model(name="m", fmuType=["ModelExchange"])
        self.assertTrue(model.modelExchange)
        self.assertFalse(model.coSimulation)

    def test_both_interfaces(self):
        model = Model(name="m", fmuType=["ModelExchange", "CoSimulation"])
        self.assertTrue(model.modelExchange)
        self.assertTrue(model.coSimulation)

    def test_context_exposes_fmu_type_flags(self):
        model = Model(name="m", fmuType=["ModelExchange", "CoSimulation"])
        model.addVariable(Input(name="x", shape=(2, ), start=[2.0, 0.0],
                                isState=True))
        model.addVariable(Output(name="dx", shape=(2, ), derivativeOf="x"))
        context = model.generateContext()
        self.assertTrue(context["modelExchange"])
        self.assertTrue(context["coSimulation"])


class TestModelStates(unittest.TestCase):

    def setUp(self) -> None:
        self.model = Model(name="vdp", fmuType=["ModelExchange"])

    def test_states_are_paired_with_derivatives(self):
        self.model.addVariable(
            Input(name="x", shape=(2, ), start=[2.0, 0.0], isState=True)
        )
        self.model.addVariable(
            Output(name="dx", shape=(2, ), derivativeOf="x")
        )
        context = self.model.generateContext()
        self.assertEqual(context["numberOfContinuousStates"], 2)
        self.assertEqual(len(context["states"]), 2)
        state_input = context["inputs"][0]
        derivative_output = context["outputs"][0]
        for state_scalar, derivative_scalar, pair in zip(
                state_input["scalarValues"],
                derivative_output["scalarValues"],
                context["states"]):
            self.assertEqual(pair["stateName"], state_scalar["name"])
            self.assertEqual(pair["derivativeName"], derivative_scalar["name"])
            self.assertEqual(
                derivative_scalar["stateValueReference"],
                state_scalar["valueReference"]
            )

    def test_no_states_when_nothing_declared(self):
        self.model.addVariable(Input(name="x"))
        self.model.addVariable(Output(name="y"))
        context = self.model.generateContext()
        self.assertEqual(context["states"], [])
        self.assertEqual(context["numberOfContinuousStates"], 0)

    def test_dangling_derivative_reference_raises(self):
        # "x" is never added as a state (isState=True) input, only as a
        # plain "y" input -- so the derivativeOf reference cannot resolve.
        self.model.addVariable(Input(name="y"))
        self.model.addVariable(Output(name="dx", derivativeOf="x"))
        with self.assertRaises(ValueError):
            self.model.generateContext()


if __name__ == "__main__":
    unittest.main()
