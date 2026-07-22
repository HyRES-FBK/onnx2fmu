import json
import shutil
import tempfile
import unittest
import numpy as np
import pandas as pd
from onnx import TensorProto, helper as onnx_helper, load, save as onnx_save
from pathlib import Path
from fmpy.validation import validate_fmu
from fmpy.simulation import simulate_fmu

from onnx2fmu.app import _createFMUFolderStructure, generate, compile, build


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


def _build_van_der_pol_onnx():
    """Analytic Van der Pol oscillator (mu=1) as an ONNX graph computing the
    state derivative dx/dt from the state x = [x0, x1]:

        dx0 = x1
        dx1 = (1 - x0^2) * x1 - x0

    This mirrors the FMI Reference-FMUs VanDerPol example (the canonical
    ME derivatives example the FMI spec ships), used here to exercise
    Model Exchange end-to-end -- build, validate, simulate -- without
    needing to train a real network.
    """
    x = onnx_helper.make_tensor_value_info("x", TensorProto.FLOAT, [2])
    dx = onnx_helper.make_tensor_value_info("dx", TensorProto.FLOAT, [2])

    starts0 = onnx_helper.make_tensor("starts0", TensorProto.INT64, [1], [0])
    ends0 = onnx_helper.make_tensor("ends0", TensorProto.INT64, [1], [1])
    starts1 = onnx_helper.make_tensor("starts1", TensorProto.INT64, [1], [1])
    ends1 = onnx_helper.make_tensor("ends1", TensorProto.INT64, [1], [2])
    axes = onnx_helper.make_tensor("axes", TensorProto.INT64, [1], [0])
    one = onnx_helper.make_tensor("one", TensorProto.FLOAT, [1], [1.0])

    nodes = [
        onnx_helper.make_node(
            "Slice", ["x", "starts0", "ends0", "axes"], ["x0"]),
        onnx_helper.make_node(
            "Slice", ["x", "starts1", "ends1", "axes"], ["x1"]),
        onnx_helper.make_node("Mul", ["x0", "x0"], ["x0sq"]),
        onnx_helper.make_node("Sub", ["one", "x0sq"], ["one_minus_x0sq"]),
        onnx_helper.make_node("Mul", ["one_minus_x0sq", "x1"], ["term"]),
        onnx_helper.make_node("Sub", ["term", "x0"], ["dx1"]),
        onnx_helper.make_node("Concat", ["x1", "dx1"], ["dx"], axis=0),
    ]

    graph = onnx_helper.make_graph(
        nodes, "van_der_pol", [x], [dx],
        initializer=[starts0, ends0, starts1, ends1, axes, one],
    )
    model = onnx_helper.make_model(graph, producer_name="onnx2fmu-tests")
    model.opset_import[0].version = 13
    # Pin the IR version to what the pinned ONNX Runtime release in
    # CMakeLists.txt (LATEST_RELEASE_TAG) actually supports. The `onnx`
    # package stamps the newest IR version it knows by default, which can
    # exceed an older ONNX Runtime's max supported IR version -- ORT then
    # fails to load the model but does not fail loudly, corrupting the
    # session for later calls.
    model.ir_version = 10
    return model


def _van_der_pol_reference(x0, output_times, dt=1e-3):
    """RK4 reference trajectory of the Van der Pol oscillator (mu=1),
    sampled at `output_times` (sorted, starting at 0.0)."""
    def f(x):
        return np.array([x[1], (1.0 - x[0] ** 2) * x[1] - x[0]])

    x = np.array(x0, dtype=float)
    t = 0.0
    results = []
    next_idx = 0
    if output_times[0] == 0.0:
        results.append(x.copy())
        next_idx = 1
    n_steps = int(round(output_times[-1] / dt))
    for _ in range(1, n_steps + 1):
        k1 = f(x)
        k2 = f(x + dt / 2 * k1)
        k3 = f(x + dt / 2 * k2)
        k4 = f(x + dt * k3)
        x = x + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
        t += dt
        if next_idx < len(output_times) and \
                abs(t - output_times[next_idx]) < dt / 2:
            results.append(x.copy())
            next_idx += 1
    return np.array(results)


class TestVanDerPolME(unittest.TestCase):
    """End-to-end Model Exchange test: build the Van der Pol FMU (FMI 2.0
    and 3.0), validate it with fmpy, and check that simulating it in
    Model Exchange mode (the importer's own solver integrating dx/dt)
    matches an independent RK4 reference trajectory.
    """

    def setUp(self):
        self.model_name = "VanDerPolME"
        self.onnx_model = _build_van_der_pol_onnx()
        self.model_description = {
            "name": self.model_name,
            "description": "Van der Pol oscillator (mu=1), Model Exchange",
            "FMIVersion": "2.0",
            "FMUType": ["ModelExchange", "CoSimulation"],
            "inputs": [
                {"name": "x", "start": [2.0, 0.0], "labels": ["x0", "x1"]},
            ],
            "outputs": [
                {"name": "dx", "derivativeOf": "x",
                 "labels": ["der_x0", "der_x1"]},
            ],
        }
        self.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_vdp_"))
        self.addCleanup(shutil.rmtree, self.tmpdir, ignore_errors=True)
        self.model_path = self.tmpdir / "model.onnx"
        onnx_save(self.onnx_model, self.model_path)
        self.destination = self.tmpdir
        self.fmu_path = self.destination / f"{self.model_name}.fmu"

    def _build_and_compile(self, fmi_version):
        description = dict(self.model_description)
        description["FMIVersion"] = fmi_version
        model_description_path = \
            self.tmpdir / f"modelDescription_{fmi_version}.json"
        with open(model_description_path, "w") as f:
            json.dump(description, f)
        target_path = self.tmpdir / f"target_{fmi_version}"
        generate(
            model_path=self.model_path,
            model_description_path=model_description_path,
            target_folder=target_path,
        )
        compile(
            target_folder=target_path,
            model_description_path=model_description_path,
            cmake_config="Debug",
            destination=self.destination,
        )
        self.assertTrue(self.fmu_path.exists())
        results = validate_fmu(self.fmu_path)
        self.assertEqual(len(results), 0, results)
        return self.fmu_path

    def test_compile_and_validate_fmi2(self):
        self._build_and_compile("2.0")

    def test_compile_and_validate_fmi3(self):
        self._build_and_compile("3.0")

    def _simulate_and_compare(self, fmi_version):
        self._build_and_compile(fmi_version)
        stop_time = 5.0
        output_interval = 0.5
        result = simulate_fmu(
            self.fmu_path,
            start_time=0.0,
            stop_time=stop_time,
            output_interval=output_interval,
            fmi_type="ModelExchange",
        )
        times = result["time"]
        simulated = np.vstack([result["x_0"], result["x_1"]]).T
        reference = _van_der_pol_reference([2.0, 0.0], times)
        np.testing.assert_allclose(simulated, reference, atol=5e-2)

    def test_simulate_model_exchange_fmi2(self):
        self._simulate_and_compare("2.0")

    def test_simulate_model_exchange_fmi3(self):
        self._simulate_and_compare("3.0")


if __name__ == "__main__":
    unittest.main()
