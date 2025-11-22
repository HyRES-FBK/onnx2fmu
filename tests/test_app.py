import json
import unittest
import numpy as np
import pandas as pd
from onnx import load
from pathlib import Path
from shutil import rmtree
from fmpy.validation import validate_fmu
from fmpy.simulation import simulate_fmu

from onnx2fmu.app import _createFMUFolderStructure, generate, compile, build


class TestApp(unittest.TestCase):

    def setUp(self):
        self.model_name = 'example1'
        self.base_dir = Path(__file__).resolve().parent.parent \
            / "examples" / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'

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


class TestExample1(unittest.TestCase):

    def setUp(self):
        self.model_name = 'example1'
        self.base_dir = Path(__file__).resolve().parent.parent \
            / "examples" / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        self.model = load(self.model_path)
        self.model_description_path = \
            self.base_dir / f'{self.model_name}Description.json'
        self.model_description = \
            json.loads(self.model_description_path.read_text())
        self.destination = Path(".")
        self.fmu_path = self.destination / f"{self.model_name}.fmu"

    def tearDown(self) -> None:
        if self.fmu_path.exists():
            self.fmu_path.unlink()

    def test_generate_fmi2(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI2")
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI2.xml",
        ]
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / self.model_name / file).is_file(),
                f"File {file} has not been generated."
            )
        if target_path.exists():
            rmtree(target_path)

    def test_generate_fmi3(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI3")
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
            target_folder=target_path
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
        target_path = Path(f"test_{self.model_name}_compile")
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        compile(
            target_folder=target_path,
            model_description_path=self.model_description_path,
            cmake_config="Debug",
            destination=self.destination
        )
        self.assertTrue(self.fmu_path.exists())
        results = validate_fmu(self.fmu_path)
        self.assertEqual(len(results), 0, results)

    def test_compile_and_simulate(self):
        self.test_compile()
        # Read input data
        signals = np.genfromtxt(self.base_dir / "input.csv",
                                delimiter=",", names=True)
        # Test the FMU using fmpy and check output against benchmark
        results = simulate_fmu(
            self.fmu_path,
            start_time=0,
            stop_time=100,
            output_interval=1,
            step_size=1,
            input=signals,
        )
        results = np.vstack([results[field] for field in
                         results.dtype.names if field != 'time']).T
        # Skip the first step, which is obtained before the first doStep
        results = results[1:]
        real_output = pd.read_csv(self.base_dir / "output.csv",
                                  index_col='time')
        self.assertGreater(1e-4, np.sum(results - real_output.values))


class TestExample2(unittest.TestCase):

    def setUp(self):
        self.model_name = 'example2'
        self.base_dir = Path(__file__).resolve().parent.parent \
            / "examples" / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        self.model = load(self.model_path)
        self.model_description_path = \
            self.base_dir / f'{self.model_name}Description.json'
        self.model_description = \
            json.loads(self.model_description_path.read_text())
        self.destination = Path(".")
        self.fmu_path = self.destination / f"{self.model_name}.fmu"

    def tearDown(self) -> None:
        if self.fmu_path.exists():
            self.fmu_path.unlink()

    def test_generate_fmi2(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI2")
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI2.xml",
        ]
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / self.model_name / file).is_file(),
                f"File {file} has not been generated."
            )
        if target_path.exists():
            rmtree(target_path)

    def test_generate_fmi3(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI3")
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
            target_folder=target_path
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
        target_path = Path(f"test_{self.model_name}_compile")
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        compile(
            target_folder=target_path,
            model_description_path=self.model_description_path,
            cmake_config="Debug",
            destination=self.destination
        )
        self.assertTrue(self.fmu_path.exists())
        results = validate_fmu(self.fmu_path)
        self.assertEqual(len(results), 0, results)

    def test_compile_and_simulate(self):
        self.test_compile()
        # Read input data
        signals = np.genfromtxt(self.base_dir / "input.csv",
                                delimiter=",", names=True)
        # Test the FMU using fmpy and check output against benchmark
        results = simulate_fmu(
            self.fmu_path,
            start_time=0,
            stop_time=100,
            output_interval=1,
            step_size=1,
            input=signals,
        )
        results = np.vstack([results[field] for field in
                         results.dtype.names if field != 'time']).T
        # Skip the first step, which is obtained before the first doStep
        results = results[1:]
        real_output = pd.read_csv(self.base_dir / "output.csv",
                                  index_col='time')
        self.assertGreater(1e-6, np.sum(results - real_output.values))


class TestExample3(unittest.TestCase):

    def setUp(self):
        self.model_name = 'example3'
        self.base_dir = Path(__file__).resolve().parent.parent \
            / "examples" / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        self.model = load(self.model_path)
        self.model_description_path = \
            self.base_dir / f'{self.model_name}Description.json'
        self.model_description = \
            json.loads(self.model_description_path.read_text())
        self.destination = Path(".")
        self.fmu_path = self.destination / f"{self.model_name}.fmu"

    def tearDown(self) -> None:
        if self.fmu_path.exists():
            self.fmu_path.unlink()

    def test_generate_fmi2(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI2")
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI2.xml",
        ]
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / self.model_name / file).is_file(),
                f"File {file} has not been generated."
            )
        if target_path.exists():
            rmtree(target_path)

    def test_generate_fmi3(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI3")
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
            target_folder=target_path
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
        target_path = Path(f"test_{self.model_name}_compile")
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        compile(
            target_folder=target_path,
            model_description_path=self.model_description_path,
            cmake_config="Debug",
            destination=self.destination
        )
        self.assertTrue(self.fmu_path.exists())
        results = validate_fmu(self.fmu_path)
        self.assertEqual(len(results), 0, results)


class TestExample4(unittest.TestCase):

    def setUp(self):
        self.model_name = 'example4'
        self.base_dir = Path(__file__).resolve().parent.parent \
            / "examples" / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        self.model = load(self.model_path)
        self.model_description_path = \
            self.base_dir / f'{self.model_name}Description.json'
        self.model_description = \
            json.loads(self.model_description_path.read_text())
        self.destination = Path(".")
        self.fmu_path = self.destination / f"{self.model_name}.fmu"

    def tearDown(self) -> None:
        if self.fmu_path.exists():
            self.fmu_path.unlink()

    def test_generate_fmi2(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI2")
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI2.xml",
        ]
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / self.model_name / file).is_file(),
                f"File {file} has not been generated."
            )
        if target_path.exists():
            rmtree(target_path)

    def test_generate_fmi3(self):
        target_path = Path(f"test_{self.model_name}_generate_target_FMI3")
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
            target_folder=target_path
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
        target_path = Path(f"test_{self.model_name}_compile")
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        compile(
            target_folder=target_path,
            model_description_path=self.model_description_path,
            cmake_config="Debug",
            destination=self.destination
        )
        self.assertTrue(self.fmu_path.exists())
        results = validate_fmu(self.fmu_path)
        self.assertEqual(len(results), 0, results)

    def test_compile_and_simulate(self):
        target_path = Path(f"test_{self.model_name}_compile")
        # The test is though without start values, so we set them to None
        for entry in ["inputs", "outputs", "locals"]:
            for variable in self.model_description.get(entry, []):
                variable["start"] = "0.0"
        temp_model_description_path = Path("modelDescription.json")
        with open(temp_model_description_path, "w", encoding="utf-8") as f:
            json.dump(self.model_description, f)
        build(
            model_path=self.model_path,
            model_description_path=temp_model_description_path,
            target_folder=target_path,
            cmake_config="Debug",
            destination=self.destination
        )
        # Read input data
        signals = np.genfromtxt(self.base_dir / "input.csv",
                                delimiter=",", names=True)
        # Test the FMU using fmpy and check output against benchmark
        results = simulate_fmu(
            self.fmu_path,
            start_time=0,
            stop_time=100,
            output_interval=1,
            step_size=1,
            input=signals,
        )
        results = np.vstack([results[field] for field in
                         results.dtype.names if field != 'time']).T
        # Skip the first step, which is obtained before the first doStep
        results = results[1:]
        real_output = pd.read_csv(self.base_dir / "output.csv",
                                  index_col='time')
        self.assertTrue(np.array_equal(results, real_output.values))


if __name__ == "__main__":
    unittest.main()
