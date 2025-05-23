import unittest
from onnx import TensorProto

from onnx2fmu.config import FMI2TYPES, FMI3TYPES
from onnx2fmu.variables import VariableFactory, Input, Local


class TestVariablesFactory(unittest.TestCase):

    def test_name(self):
        inadmissible_variable_name = "example:/$."
        v = VariableFactory(
            name=inadmissible_variable_name,
        )
        self.assertEqual(v.name, "example")
        with self.assertRaises(ValueError):
            VariableFactory(
                name=""
            )

    def test_shape(self):
        with self.assertRaises(ValueError):
            VariableFactory(name="x", shape=())
        v = VariableFactory(name="x", shape=(0, 3, 0, 4))
        self.assertEqual(v.shape, (1, 3, 1, 4))

    def test_fmiVersion(self):
        with self.assertRaises(ValueError):
            VariableFactory(name="x", fmiVersion="1.0")

    def test_vType(self):
        v = VariableFactory(name="x")
        self.assertEqual(FMI2TYPES[TensorProto.FLOAT], v.vType)
        v = VariableFactory(name="x", fmiVersion="3.0")
        self.assertEqual(FMI3TYPES[TensorProto.FLOAT], v.vType)

    def test_print(self):
        v = VariableFactory(name="x")
        self.assertEqual(
            "VariableFactory(x, continuous)",
            v.__str__()
        )

    def test_generate_context(self):
        v = VariableFactory(name="x")
        context = {
            "name": "x",
            "shape": (1, ),
            "description": "",
            "causality": None,
            "variability": "continuous",
            "fmiVersion": "2.0",
            "vType": FMI2TYPES[TensorProto.FLOAT],
            "scalarValues": [],
        }
        for k in v.generate_context():
            self.assertEqual(context[k], getattr(v, k))


class TestInputVariable(unittest.TestCase):

    def test_print(self):
        v = Input(name="x", start=2.0)
        self.assertEqual(
            "Input(x, continuous)(2.0)",
            v.__str__()
        )


class TestLocalVariable(unittest.TestCase):

    def test_names(self):
        v = Local(name_in="X.1", name_out="X:2")
        self.assertEqual(v.name_in, "X1")
        self.assertEqual(v.name_out, "X2")
        self.assertEqual(v.name, "X1_X2")

    def test_generate_context(self):
        v = Local(name_in="X.1", name_out="X:2")
        context = v.generate_context()
        self.assertIn("name_in", context)
        self.assertIn("name_out", context)
        self.assertEqual(context["name"], "X1_X2")
