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
            "nodeName": "x",
            "shape": (1, ),
            "description": "",
            "causality": None,
            "variability": "continuous",
            "fmiVersion": "2.0",
            "vType": FMI2TYPES[TensorProto.FLOAT],
            "scalarValues": [{"name": "x_0", "label": "", "start": "1.0"}],
            "start": "1.0"
        }
        for k in v.generateContext():
            self.assertEqual(context[k], getattr(v, k))


class TestInputVariable(unittest.TestCase):

    def test_print(self):
        v = Input(name="x", start="2.0")
        self.assertEqual(
            "Input(x, continuous)(2.0)",
            v.__str__()
        )


class TestLocalVariable(unittest.TestCase):

    def test_names(self):
        v = Local(nameIn="X.1", nameOut="X:2")
        self.assertEqual(v.nameIn, "X1")
        self.assertEqual(v.nameOut, "X2")
        self.assertEqual(v.name, "X1_X2")
        self.assertEqual(v.nodeNameIn, "X.1")
        self.assertEqual(v.nodeNameOut, "X:2")

    def test_generate_context(self):
        v = Local(nameIn="X.1", nameOut="X:2")
        context = v.generateContext()
        self.assertIn("nameIn", context)
        self.assertIn("nameOut", context)
        self.assertIn("nodeNameIn", context)
        self.assertIn("nodeNameOut", context)
        self.assertEqual(context["name"], "X1_X2")


if __name__ == "__main__":
    unittest.main()
