import unittest
from onnx import TensorProto

from onnx2fmu.config import FMI2TYPES, FMI3TYPES
from onnx2fmu.variables import VariableFactory, Input, Output, Local


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

    def test_time_input_has_no_scalars(self):
        v = Input(name="t", time=True)
        self.assertTrue(v.isTime)
        self.assertEqual(v.scalarValues, [])

    def test_regular_input_is_unaffected_by_state_flags(self):
        v = Input(name="x")
        self.assertFalse(v.isTime)
        self.assertFalse(v.isState)
        self.assertEqual(v.causality, "input")
        self.assertEqual(v.initial, "")

    def test_state_input_is_reclassified_as_output(self):
        v = Input(name="x", shape=(2, ), start=[2.0, 0.0], isState=True)
        self.assertTrue(v.isState)
        self.assertEqual(v.causality, "output")
        self.assertEqual(v.initial, "exact")
        # Continuous states must be Float64 for the fmi{2,3} state API; in
        # FMI 2.0 both FLOAT and DOUBLE map to "Real", so check via a 3.0
        # instance where the distinction is visible.
        v3 = Input(name="x", shape=(2, ), start=[2.0, 0.0], isState=True,
                   fmiVersion="3.0")
        self.assertEqual(v3.vType, FMI3TYPES[TensorProto.DOUBLE])

    def test_state_input_context_includes_new_keys(self):
        v = Input(name="x", isState=True)
        context = v.generateContext()
        for key in ["isTime", "isState", "initial"]:
            self.assertIn(key, context)


class TestOutputVariable(unittest.TestCase):

    def test_regular_output_is_unaffected_by_derivative_flag(self):
        v = Output(name="y")
        self.assertIsNone(v.derivativeOf)
        self.assertEqual(v.causality, "output")
        self.assertEqual(v.initial, "")

    def test_derivative_output_is_reclassified_as_local(self):
        v = Output(name="dx", derivativeOf="x")
        self.assertEqual(v.derivativeOf, "x")
        self.assertEqual(v.causality, "local")
        self.assertEqual(v.initial, "calculated")
        v3 = Output(name="dx", derivativeOf="x", fmiVersion="3.0")
        self.assertEqual(v3.vType, FMI3TYPES[TensorProto.DOUBLE])

    def test_derivative_output_context_includes_new_keys(self):
        v = Output(name="dx", derivativeOf="x")
        context = v.generateContext()
        for key in ["derivativeOf", "initial"]:
            self.assertIn(key, context)


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
