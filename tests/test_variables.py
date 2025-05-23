import unittest
from onnx import TensorProto

from onnx2fmu.config import FMI2TYPES, FMI3TYPES
from onnx2fmu.variables import VariableFactory


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
            "VariableFactory(x, None, continuous)",
            v.__str__()
        )

    def test_generate_context(self):
        v = VariableFactory(name="x")
        context = {
            "name": "x",
            "description": "",
            "causality": None,
            "variability": "continuous",
            "fmiVersion": "2.0",
            "vType": FMI2TYPES[TensorProto.FLOAT],
        }
        for k in v.generate_context():
            self.assertEqual(context[k], getattr(v, k))
