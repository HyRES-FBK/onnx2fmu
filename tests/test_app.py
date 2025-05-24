import json
import unittest
from onnx import load
from shutil import rmtree
from pathlib import Path

from onnx2fmu.app import _find_version, _createFMUFolderStructure, generate


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

    def test_create_project_structure(self):
        target_path = Path("test_project_structure_target")
        _createFMUFolderStructure(target_path, self.model_path)
        self.assertIn(
            "CMakeLists.txt",
            [f.name for f in target_path.iterdir() if f.is_file()]
        )
        for folder in [self.model_name, "include", "src"]:
            self.assertIn(
                folder,
                [f.name for f in target_path.iterdir() if not f.is_file()]
            )
        rmtree(target_path)

    def test_version(self):
        pattern = r'^\d+\.\d+\.\d+$'
        self.assertRegex(_find_version("pyproject.toml"), pattern)

    def test_generate_fmi2(self):
        target_path = Path("test_generate_target_FMI2")
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI2.xml",
        ]
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            destination=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / self.model_name / file).is_file(),
                f"File {file} has not been generated."
            )
        if target_path.exists():
            rmtree(target_path)

    def test_generate_fmi3(self):
        target_path = Path("test_generate_target_FMI3")
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI3.xml",
        ]
        self.model_description["FMIVersion"] = "3.0"
        temp_model_description_path = Path("modelDescription.json")
        with open(temp_model_description_path, "w", encoding="utf-8") as f:
            json.dump(self.model_description, f)
        generate(
            model_path=self.model_path,
            model_description_path=temp_model_description_path,
            destination=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / self.model_name / file).is_file(),
                f"File {file} has not been generated."
            )
        if target_path.exists():
            rmtree(target_path)
        temp_model_description_path.unlink()

    def test_compile(self):
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
