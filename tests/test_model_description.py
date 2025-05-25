import json
import unittest
from math import prod
from onnx import load
from pathlib import Path

from onnx2fmu.model_description import ModelDescription


class TestModelDescription(unittest.TestCase):

    def setUp(self) -> None:
        self.model_name = 'example4'
        self.base_dir = Path(__file__).resolve().parent / self.model_name
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


if __name__ == "__main__":
    unittest.main()
