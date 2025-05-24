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
        v = Local(name_in="z1", name_out="z2", shape=(4, 5))
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
