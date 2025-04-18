import json
import unittest
import numpy as np
from onnx import load
from pathlib import Path
from fmpy import simulate_fmu
from fmpy.validation import validate_fmu

from onnx2fmu.app import ScalarVariable, Model, build


FMI_VERSIONS = ["2.0", "3.0"]


class TestApp(unittest.TestCase):

    def setUp(self):
        self.base_dir = Path(__file__).resolve().parent / 'example1'
        self.model_name = 'example1'
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        self.model = load(self.model_path)
        self.model_description_path = \
            self.base_dir / f'{self.model_name}Description.json'
        self.model_description = \
            json.loads(self.model_description_path.read_text())

    def test_fmi2_scalar_variable(self):
        # Check againts name containing non-alfanumeric characters
        var = ScalarVariable(
            name='example;:!|~',
            description='',
            variability='continuous',
            causality='input',
            valueReference=1,
            vType=1,
            start=0.0
        )
        self.assertTrue(var.name == 'example')

        # Value referece cannot be zero
        with self.assertRaises(ValueError):
            var = ScalarVariable(
                name='example',
                description='',
                variability='continuous',
                causality='input',
                valueReference=0,
                vType=1,
                start=0.0
            )

        # Check that inexisting causality return ValueError
        with self.assertRaises(ValueError):
            ScalarVariable(
                name='example',
                description='',
                variability='continuous',
                causality='wrong causality',
                valueReference=1,
                vType=1,
                start=0.0
            )
        # Check that inexisting variability return ValueError
        with self.assertRaises(ValueError):
            ScalarVariable(
                name='example',
                description='',
                variability='wrong variability',
                causality='input',
                valueReference=1,
                vType=1,
                start=0.0
            )

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

    def test_FMU_fmi2(self):
        # Build the FMNU
        # Test the model build process. Remember to check for multiple OSs
        build(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            fmi_version="2.0"
        )
        # Set FMU path
        fmu_path = Path(f"{self.model_name}.fmu")
        # Validate
        res = validate_fmu(fmu_path)
        self.assertEqual(len(res), 0)
        # Read data
        signals = np.genfromtxt(self.base_dir / "input.csv",
                                delimiter=",", names=True)
        # Test the FMU using fmpy and check output against benchmark
        res = simulate_fmu(
            fmu_path,
            start_time=0,
            stop_time=100,
            output_interval=1,
            input=signals,
        )
        res = np.vstack([res[field] for field in
                         res.dtype.names if field != 'time']).T
        # Load real output
        out_real = np.genfromtxt(self.base_dir / "output.csv",
                                 delimiter=",", names=True)
        out_real = np.vstack([out_real[field] for field in
                              out_real.dtype.names if field != 'time']).T
        # Set real output precision to 1e-5
        out_real = np.round(out_real, decimals=5)
        # Cut out first row or res because it is repeated
        # TODO: discover why the first row is repeated
        res = res[1:]
        # Compare results with the ground truth
        mse = np.sum(np.power(res - out_real, 2))
        # Check that mse is lower than 1e-6
        self.assertLessEqual(mse, 1e-6)
        # Cleanup FMU
        fmu_path.unlink()

    def test_FMU_fmi3(self):
        # Build the FMNU
        # Test the model build process. Remember to check for multiple OSs
        build(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            fmi_version="3.0"
        )
        # Set FMU path
        fmu_path = Path(f"{self.model_name}.fmu")
        # Validate
        res = validate_fmu(fmu_path)
        self.assertEqual(len(res), 0)
        # Read data
        signals = np.genfromtxt(self.base_dir / "input.csv",
                                delimiter=",", names=True)
        # Test the FMU using fmpy and check output against benchmark
        res = simulate_fmu(
            fmu_path,
            start_time=0,
            stop_time=100,
            output_interval=1,
            input=signals,
        )
        res = np.vstack([res[field] for field in
                         res.dtype.names if field != 'time']).T
        # Load real output
        out_real = np.genfromtxt(self.base_dir / "output.csv",
                                 delimiter=",", names=True)
        out_real = np.vstack([out_real[field] for field in
                              out_real.dtype.names if field != 'time']).T
        # Set real output precision to 1e-5
        out_real = np.round(out_real, decimals=5)
        # Cut out first row or res because it is repeated
        # TODO: discover why the first row is repeated
        res = res[1:]
        # Compare results with the ground truth
        mse = np.sum(np.power(res - out_real, 2))
        # Check that mse is lower than 1e-6
        self.assertLessEqual(mse, 1e-6)
        # Cleanup FMU
        fmu_path.unlink()
