import json
import shutil
import tempfile
import unittest
import zipfile
import numpy as np
import pandas as pd
from onnx import load
from pathlib import Path
from fmpy.validation import validate_fmu
from fmpy.simulation import simulate_fmu

from onnx2fmu.app import _createFMUFolderStructure, generate, compile, build

# Generic filenames the bundled ONNX Runtime library must never use: shipping
# it under its stock name risks colliding with another copy of ONNX Runtime
# already loaded by the FMI importer or another FMU in the same process. See
# https://github.com/HyRES-FBK/onnx2fmu/issues/53.
GENERIC_ORT_LIBRARY_NAMES = {"onnxruntime.dll", "libonnxruntime.so", "libonnxruntime.dylib"}


def assert_private_ort_library(test_case, fmu_path):
    """The bundled ONNX Runtime library must be renamed with the model name,
    and never shipped under a generic name (issue #53)."""
    # The compiled FMU is always named "<model_name>.fmu" (see app.py's
    # `compile`), so the stem is the authoritative model name — the fixture's
    # own `self.model_name` can differ (e.g. TestExample4 compiles FMUs named
    # "example4FMI2"/"example4FMI3" from base name "example4").
    model_name = fmu_path.stem
    with zipfile.ZipFile(fmu_path) as fmu_zip:
        binaries = [
            name for name in fmu_zip.namelist()
            if name.startswith("binaries/") and "onnxruntime" in name.lower()
        ]

    test_case.assertTrue(binaries, "No ONNX Runtime library found in binaries/.")

    private_libs = [
        name for name in binaries
        if Path(name).stem.endswith(f"{model_name}_onnxruntime")
    ]
    test_case.assertTrue(
        private_libs,
        f"Expected the bundled ONNX Runtime library to be renamed with the "
        f"model name ('{model_name}_onnxruntime'), found: {binaries}"
    )

    for name in binaries:
        test_case.assertNotIn(
            Path(name).name, GENERIC_ORT_LIBRARY_NAMES,
            f"Found a generically-named ONNX Runtime library ({name}) — it "
            "must be renamed per-model to avoid collisions (issue #53)."
        )


class TestApp(unittest.TestCase):

    def setUp(self):
        self.model_name = 'example1'
        self.base_dir = Path(__file__).resolve().parent.parent \
            / "examples" / self.model_name
        self.model_path = self.base_dir / f'{self.model_name}.onnx'
        # Isolate all filesystem state for this test in a private temp dir so
        # tests never share build folders / FMU output / the CWD with each
        # other. The whole directory is removed on cleanup.
        self.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_"))
        self.addCleanup(shutil.rmtree, self.tmpdir, ignore_errors=True)

    def test_create_project_structure(self):
        target_path = self.tmpdir / "test_project_structure_target"
        _createFMUFolderStructure(target_path, self.model_path, self.model_name)
        self.assertIn(
            "CMakeLists.txt",
            [f.name for f in target_path.iterdir() if f.is_file()]
        )
        for folder in [self.model_name, "include", "src"]:
            self.assertIn(
                folder,
                [f.name for f in target_path.iterdir() if not f.is_file()]
            )


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
        self.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_"))
        self.addCleanup(shutil.rmtree, self.tmpdir, ignore_errors=True)
        self.destination = self.tmpdir
        self.fmu_path = self.destination / f"{self.model_description['name']}.fmu"

    def test_generate_fmi2(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI2"
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
                (target_path / self.model_description['name'] / file).is_file(),
                f"File {file} has not been generated."
            )

    def test_generate_fmi3(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI3"
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI3.xml",
        ]
        self.model_description["FMIVersion"] = "3.0"
        temp_model_description_path = self.tmpdir / "modelDescription.json"
        with open(temp_model_description_path, "w", encoding="utf-8") as f:
            json.dump(self.model_description, f)
        generate(
            model_path=self.model_path,
            model_description_path=temp_model_description_path,
            target_folder=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / self.model_description['name'] / file).is_file(),
                f"File {file} has not been generated."
            )

    def test_compile_fmi2(self):
        target_path = self.tmpdir / f"test_{self.model_name}_compile_FMI2"
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
        assert_private_ort_library(self, self.fmu_path)

    def test_compile_fmi3(self):
        target_path = self.tmpdir / f"test_{self.model_name}_compile_FMI3"
        self.model_description["FMIVersion"] = "3.0"
        temp_model_description_path = self.tmpdir / "modelDescription.json"
        with open(temp_model_description_path, "w", encoding="utf-8") as f:
            json.dump(self.model_description, f)
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
        assert_private_ort_library(self, self.fmu_path)

    def test_compile_and_simulate(self):
        self.test_compile_fmi2()
        # Read input data
        signals = np.genfromtxt(self.base_dir / "Example1_in.csv",
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
        real_output = pd.read_csv(self.base_dir / "Example1_ref.csv",
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
        self.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_"))
        self.addCleanup(shutil.rmtree, self.tmpdir, ignore_errors=True)
        self.destination = self.tmpdir
        self.fmu_path = self.destination / f"{self.model_name}.fmu"

    def test_generate_fmi2(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI2"
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

    def test_generate_fmi3(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI3"
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI3.xml",
        ]
        self.model_description["FMIVersion"] = "3.0"
        temp_model_description_path = self.tmpdir / "modelDescription.json"
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

    def test_compile(self):
        target_path = self.tmpdir / f"test_{self.model_name}_compile"
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
        assert_private_ort_library(self, self.fmu_path)

    def test_compile_and_simulate(self):
        self.test_compile()
        # Read input data
        signals = np.genfromtxt(self.base_dir / "Example2_in.csv",
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
        real_output = pd.read_csv(self.base_dir / "Example2_ref.csv",
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
        self.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_"))
        self.addCleanup(shutil.rmtree, self.tmpdir, ignore_errors=True)
        self.destination = self.tmpdir
        self.fmu_path = self.destination / f"{self.model_name}.fmu"

    def test_generate_fmi2(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI2"
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

    def test_generate_fmi3(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI3"
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI3.xml",
        ]
        self.model_description["FMIVersion"] = "3.0"
        temp_model_description_path = self.tmpdir / "modelDescription.json"
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

    def test_compile(self):
        target_path = self.tmpdir / f"test_{self.model_name}_compile"
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
        assert_private_ort_library(self, self.fmu_path)


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
        self.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_"))
        self.addCleanup(shutil.rmtree, self.tmpdir, ignore_errors=True)
        self.destination = self.tmpdir
        # The compiled FMU is named after the description's "name" field, which
        # for example4 is "example4FMI2" (not the bare model name).
        self.fmu_path = self.destination / f"{self.model_description['name']}.fmu"

    def test_generate_fmi2(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI2"
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI2.xml",
        ]
        model_name = self.model_description['name']
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / model_name / file).is_file(),
                f"File {file} has not been generated."
            )

    def test_generate_fmi3(self):
        target_path = self.tmpdir / f"test_{self.model_name}_generate_target_FMI3"
        files = [
            "model.c",
            "config.h",
            "buildDescription.xml",
            "FMI3.xml",
        ]
        self.model_description["FMIVersion"] = "3.0"
        model_name = self.model_description['name']
        temp_model_description_path = self.tmpdir / "modelDescription.json"
        with open(temp_model_description_path, "w", encoding="utf-8") as f:
            json.dump(self.model_description, f)
        generate(
            model_path=self.model_path,
            model_description_path=temp_model_description_path,
            target_folder=target_path
        )
        for file in files:
            self.assertTrue(
                (target_path / model_name / file).is_file(),
                f"File {file} has not been generated."
            )

    def test_compile(self):
        target_path = self.tmpdir / f"test_{self.model_name}_compile_FMI2"
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
        assert_private_ort_library(self, self.fmu_path)

    def test_compile_fmi3(self):
        target_path = self.tmpdir / f"test_{self.model_name}_compile_FMI3"
        # Change the model description path to the file specific for the FMI 3.0
        self.model_description_path = self.base_dir / f"{self.model_name}DescriptionFMI3.json"
        # This description's "name" is example4FMI3, so the FMU is named accordingly.
        fmi3_name = json.loads(self.model_description_path.read_text())['name']
        self.fmu_path = self.destination / f"{fmi3_name}.fmu"
        generate(
            model_path=self.model_path,
            model_description_path=self.model_description_path,
            target_folder=target_path,
        )
        compile(
            target_folder=target_path,
            model_description_path=self.model_description_path,
            cmake_config="Debug",
            destination=self.destination,
        )
        self.assertTrue(self.fmu_path.exists())
        results = validate_fmu(self.fmu_path)
        self.assertEqual(len(results), 0, results)
        assert_private_ort_library(self, self.fmu_path)

    def test_compile_and_simulate(self):
        target_path = self.tmpdir / f"test_{self.model_name}_compile_and_simulate"
        # The test is though without start values, so we set them to None
        for entry in ["inputs", "outputs", "locals"]:
            for variable in self.model_description.get(entry, []):
                variable["start"] = "0.0"
        temp_model_description_path = self.tmpdir / "modelDescription.json"
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
        signals = np.genfromtxt(self.base_dir / "Example4_in.csv",
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
        real_output = pd.read_csv(self.base_dir / "Example4_ref.csv",
                                  index_col='time')
        self.assertTrue(np.array_equal(results, real_output.values))


if __name__ == "__main__":
    unittest.main()
