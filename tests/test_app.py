import json
import unittest
import numpy as np
from onnx import load
from pathlib import Path
from fmpy import simulate_fmu
from fmpy.validation import validate_fmu

from onnx2fmu.app import _find_version, _createFMUFolderStructure


class TestApp(unittest.TestCase):

    def setUp(self):
        self.model_name = 'example4'
        self.base_dir = Path(__file__).resolve().parent / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        self.model = load(self.model_path)
        self.model_description_path = \
            self.base_dir / f'{self.model_name}Description.json'
        self.model_description = \
            json.loads(self.model_description_path.read_text())
        self.target_path = Path("target")

    def test_create_project_structure(self):
        _createFMUFolderStructure(self.target_path, self.model_path)
        self.assertIn(
            "CMakeLists.txt",
            [f.name for f in self.target_path.iterdir() if f.is_file()]
        )
        for folder in [self.model_name, "include", "src"]:
            self.assertIn(
                folder,
                [f.name for f in self.target_path.iterdir() if not f.is_file()]
            )

    def test_version(self):
        pattern = r'^\d+\.\d+\.\d+$'
        self.assertRegex(_find_version("pyproject.toml"), pattern)

    def test_model_declaration(self):
        model = Model(
            onnx_model=self.model,
            model_description=self.model_description
        )
        self.assertTrue(model)
        # Check that model has a name
        self.assertTrue(model.name)
        # Check that model input is not empty
        self.assertTrue(len(model.input) > 0)
        # Check that model output is not empty
        self.assertTrue(len(model.output) > 0)
        # Check that model has a version
        self.assertTrue(model.FMIVersion)
        # Check that model version is in the list of valid values
        self.assertIn(model.FMIVersion, FMI_VERSIONS)
        # Check GUID length
        self.assertEqual(len(model.GUID), 36)

    def test_empty_model_declaration(self):
        # Raise error if model description is empty
        with self.assertRaises(AssertionError):
            Model(self.model, {})
        # Raise error if model is None
        with self.assertRaises(AttributeError):
            Model({}, self.model_description)

    def test_number_of_inputs(self):
        pass

    def test_number_of_outputs(self):
        pass

        # # Set FMU path
        # fmu_path = Path(f"{self.model_name}.fmu")
        # # Validate
        # res = validate_fmu(fmu_path)
        # self.assertEqual(len(res), 0)
        # # Read data
        # signals = np.genfromtxt(self.base_dir / "input.csv",
        #                         delimiter=",", names=True)
        # # Test the FMU using fmpy and check output against benchmark
        # res = simulate_fmu(
        #     fmu_path,
        #     start_time=0,
        #     stop_time=100,
        #     output_interval=1,
        #     input=signals,
        # )
        # res = np.vstack([res[field] for field in
        #                  res.dtype.names if field != 'time']).T
        # # Load real output
        # out_real = np.genfromtxt(self.base_dir / "output.csv",
        #                          delimiter=",", names=True)
        # out_real = np.vstack([out_real[field] for field in
        #                       out_real.dtype.names if field != 'time']).T
        # # Set real output precision to 1e-5
        # out_real = np.round(out_real, decimals=5)
        # # Cut out first row or res because it is repeated
        # # TODO: discover why the first row is repeated
        # res = res[1:]
        # # Compare results with the ground truth
        # mse = np.sum(np.power(res - out_real, 2))
        # # Check that mse is lower than 1e-6
        # self.assertLessEqual(mse, 1e-6)
        # # Cleanup FMU
        # fmu_path.unlink()
