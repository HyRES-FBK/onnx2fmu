"""Co-Simulation stepping semantics (issue #59).

In Co-Simulation the ONNX network runs only inside ``doStep``, once per
internal step. ``set``, ``get`` and ``exitInitializationMode`` never run it:
an output read at communication point ``t_{k+1}`` holds what the internal
step ``[t_k, t_{k+1}]`` computed, and before the first step it holds its
``start`` value.

The tests build small FMUs from ONNX graphs written with ``onnx.helper`` and
drive them through the FMI API with FMPy, so they observe the FMU only as an
importing tool would.
"""
import json
import shutil
import tempfile
import unittest
from pathlib import Path
from xml.etree import ElementTree

import numpy as np
from fmpy import extract, read_model_description, simulate_fmu
from fmpy.fmi2 import FMU2Slave
from fmpy.fmi3 import FMU3Slave
from onnx import TensorProto, helper, save

from onnx2fmu.app import build, generate


def _build_onnx(stateful: bool):
    """Echo the input ``u`` and the time input ``t`` as ``u_seen``/``t_seen``.

    With ``stateful``, also count the network runs: ``count_out = count_in
    + 1`` is wired back into ``count_in`` as a feedback state, and ``evals``
    reports ``count_out``.
    """
    def scalar(name):
        return helper.make_tensor_value_info(name, TensorProto.FLOAT, [1])

    inputs = [scalar("u"), scalar("t")]
    outputs = [scalar("u_seen"), scalar("t_seen")]
    nodes = [
        helper.make_node("Identity", ["u"], ["u_seen"]),
        helper.make_node("Identity", ["t"], ["t_seen"]),
    ]
    initializers = []
    if stateful:
        inputs.append(scalar("count_in"))
        outputs += [scalar("evals"), scalar("count_out")]
        initializers.append(
            helper.make_tensor("one", TensorProto.FLOAT, [1], [1.0]))
        nodes += [
            helper.make_node("Add", ["count_in", "one"], ["count_out"]),
            helper.make_node("Identity", ["count_out"], ["evals"]),
        ]
    graph = helper.make_graph(nodes, "counter", inputs, outputs,
                              initializer=initializers)
    model = helper.make_model(graph, producer_name="onnx2fmu-tests")
    model.opset_import[0].version = 13
    # Pinned to what the bundled ONNX Runtime release supports, see the Van
    # der Pol model in test_app.py.
    model.ir_version = 10
    return model


def _build_fmu(directory: Path, name: str, fmi_version: str,
               stateful: bool = True, **description_keys) -> Path:
    description = {
        "name": name,
        "description": "Counts and echoes its network runs.",
        "FMIVersion": fmi_version,
        "inputs": [
            {"name": "u", "start": 0.0},
            {"name": "t", "time": True},
        ],
        "outputs": [
            {"name": "u_seen", "start": -1.0},
            {"name": "t_seen", "start": -1.0},
        ],
    }
    if stateful:
        description["outputs"].append({"name": "evals", "start": 0.0})
        description["feedbackStates"] = [
            {"nameIn": "count_in", "nameOut": "count_out", "start": 0.0}
        ]
    description.update(description_keys)
    model_path = directory / f"{name}.onnx"
    save(_build_onnx(stateful), model_path)
    description_path = directory / f"{name}.json"
    description_path.write_text(json.dumps(description))
    build(
        model_path=model_path,
        model_description_path=description_path,
        target_folder=directory / f"target_{name}",
        destination=directory,
        cmake_config="Debug",
    )
    return directory / f"{name}.fmu"


class Slave:
    """Drive a Co-Simulation FMU through the FMI 2.0 or 3.0 API by name."""

    def __init__(self, fmu_path: Path, unzipdir: str):
        model_description = read_model_description(fmu_path)
        self.fmi3 = model_description.fmiVersion.startswith("3")
        self.vr = {v.name: v.valueReference
                   for v in model_description.modelVariables}
        cls = FMU3Slave if self.fmi3 else FMU2Slave
        self.fmu = cls(
            guid=model_description.guid,
            unzipDirectory=unzipdir,
            modelIdentifier=model_description.coSimulation.modelIdentifier,
            instanceName="instance",
        )
        self.fmu.instantiate()

    def initialize(self, **inputs):
        """Initialize at t = 0, setting `inputs` in Initialization Mode."""
        if self.fmi3:
            self.fmu.enterInitializationMode(startTime=0.0)
        else:
            self.fmu.setupExperiment(startTime=0.0)
            self.fmu.enterInitializationMode()
        for name, value in inputs.items():
            self.set(name, value)
        self.fmu.exitInitializationMode()

    def get(self, name: str) -> float:
        getter = self.fmu.getFloat32 if self.fmi3 else self.fmu.getReal
        return getter([self.vr[name]])[0]

    def set(self, name: str, value: float) -> None:
        setter = self.fmu.setFloat32 if self.fmi3 else self.fmu.setReal
        setter([self.vr[name]], [value])

    def step(self, time: float, step_size: float) -> None:
        self.fmu.doStep(currentCommunicationPoint=time,
                        communicationStepSize=step_size)

    def get_state(self):
        return self.fmu.getFMUState() if self.fmi3 else self.fmu.getFMUstate()

    def set_state(self, state) -> None:
        if self.fmi3:
            self.fmu.setFMUState(state)
        else:
            self.fmu.setFMUstate(state)

    def free_state(self, state) -> None:
        if self.fmi3:
            self.fmu.freeFMUState(state)
        else:
            self.fmu.freeFMUstate(state)

    def close(self) -> None:
        self.fmu.terminate()
        self.fmu.freeInstance()


class CoSimulationSemantics:
    """Shared tests; subclasses set `fmi_version` and the description keys
    that set an internal step of 50 in that FMI version."""

    fmi_version = None
    step_50_keys = None

    @classmethod
    def setUpClass(cls):
        cls.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_cs_"))
        version = cls.fmi_version[0]
        cls.counter = _build_fmu(
            cls.tmpdir, f"Counter{version}", cls.fmi_version)
        cls.counter_50 = _build_fmu(
            cls.tmpdir, f"Counter50_{version}", cls.fmi_version,
            **cls.step_50_keys)
        cls.echo = _build_fmu(
            cls.tmpdir, f"Echo{version}", cls.fmi_version, stateful=False)

    @classmethod
    def tearDownClass(cls):
        shutil.rmtree(cls.tmpdir, ignore_errors=True)

    def slave(self, fmu_path: Path) -> Slave:
        unzipdir = extract(fmu_path)
        self.addCleanup(shutil.rmtree, unzipdir, ignore_errors=True)
        slave = Slave(fmu_path, unzipdir)
        self.addCleanup(slave.close)
        return slave

    def test_simulate_fmu_runs_the_network_once_per_step(self):
        # The reproduction from issue #59: FMPy applies u(t_k), calls
        # doStep(t_k, 1) and samples the outputs at t_{k+1}.
        signals = np.array([(0.0, 0.0), (3.0, 3.0)],
                           dtype=[("time", np.float64), ("u_0", np.float64)])
        result = simulate_fmu(
            self.counter, start_time=0.0, stop_time=3.0, output_interval=1.0,
            input=signals, output=["evals_0", "u_seen_0", "t_seen_0"],
        )
        np.testing.assert_array_equal(result["time"], [0, 1, 2, 3])
        # At t0 the outputs hold their start values; at t_{k+1} they hold
        # what the step [t_k, t_{k+1}] computed from u(t_k) at time t_k.
        np.testing.assert_array_equal(result["evals_0"], [0, 1, 2, 3])
        np.testing.assert_array_equal(result["u_seen_0"], [-1, 0, 1, 2])
        np.testing.assert_array_equal(result["t_seen_0"], [-1, 0, 1, 2])

    def test_only_do_step_runs_the_network(self):
        fmu = self.slave(self.counter)
        fmu.initialize(u_0=5.0)
        # exitInitializationMode does not run the network.
        self.assertEqual(fmu.get("evals_0"), 0)
        self.assertEqual(fmu.get("count_in_count_out_0"), 0)
        self.assertEqual(fmu.get("u_seen_0"), -1)
        # set -> get -> set -> get within one step does not run it either.
        fmu.set("u_0", 1.0)
        self.assertEqual(fmu.get("evals_0"), 0)
        fmu.set("u_0", 2.0)
        self.assertEqual(fmu.get("evals_0"), 0)
        self.assertEqual(fmu.get("u_seen_0"), -1)
        # A step runs it once, on the inputs set before the step.
        fmu.step(0.0, 1.0)
        self.assertEqual(fmu.get("evals_0"), 1)
        self.assertEqual(fmu.get("count_in_count_out_0"), 1)
        self.assertEqual(fmu.get("u_seen_0"), 2)
        self.assertEqual(fmu.get("t_seen_0"), 0)
        # A step in which the importer sets nothing also runs it once.
        fmu.step(1.0, 1.0)
        self.assertEqual(fmu.get("evals_0"), 2)
        self.assertEqual(fmu.get("u_seen_0"), 2)
        self.assertEqual(fmu.get("t_seen_0"), 1)
        # Setting an input does not change the outputs until the next step.
        fmu.set("u_0", 7.0)
        self.assertEqual(fmu.get("u_seen_0"), 2)
        self.assertEqual(fmu.get("evals_0"), 2)

    def test_postponed_step_uses_the_inputs_from_its_start(self):
        # The internal step is 1, so the step [0, 1] runs only once a
        # communication step reaches t = 1, but on u and time at t = 0.
        fmu = self.slave(self.counter)
        fmu.initialize()
        fmu.set("u_0", 10.0)
        fmu.step(0.0, 0.5)
        self.assertEqual(fmu.get("evals_0"), 0)
        fmu.set("u_0", 20.0)
        fmu.step(0.5, 0.5)
        self.assertEqual(fmu.get("evals_0"), 1)
        self.assertEqual(fmu.get("u_seen_0"), 10)
        self.assertEqual(fmu.get("t_seen_0"), 0)

    def test_step_starting_inside_a_communication_step(self):
        # With communication steps of 0.75, the internal step [1, 2] starts
        # inside [0.75, 1.5], where the input holds the value set at 0.75.
        fmu = self.slave(self.counter)
        fmu.initialize()
        fmu.set("u_0", 1.0)
        fmu.step(0.0, 0.75)
        fmu.set("u_0", 2.0)
        fmu.step(0.75, 0.75)
        self.assertEqual(fmu.get("evals_0"), 1)
        self.assertEqual(fmu.get("u_seen_0"), 1)
        fmu.set("u_0", 3.0)
        fmu.step(1.5, 0.75)
        self.assertEqual(fmu.get("evals_0"), 2)
        self.assertEqual(fmu.get("u_seen_0"), 2)
        self.assertEqual(fmu.get("t_seen_0"), 1)

    def test_fmu_state_restores_outputs_and_recorded_inputs(self):
        fmu = self.slave(self.counter)
        fmu.initialize()
        fmu.set("u_0", 10.0)
        fmu.step(0.0, 0.5)  # the step [0, 1] is pending, on u = 10
        state = fmu.get_state()
        self.addCleanup(fmu.free_state, state)
        fmu.set("u_0", 20.0)
        fmu.step(0.5, 0.5)
        completed = [fmu.get(name) for name in ("evals_0", "u_seen_0",
                                                "t_seen_0")]
        self.assertEqual(completed, [1, 10, 0])
        fmu.set_state(state)
        self.assertEqual(fmu.get("evals_0"), 0)
        self.assertEqual(fmu.get("u_seen_0"), -1)
        fmu.set("u_0", 30.0)
        fmu.step(0.5, 0.5)
        repeated = [fmu.get(name) for name in ("evals_0", "u_seen_0",
                                               "t_seen_0")]
        self.assertEqual(repeated, completed)

    def test_stateless_fmu_runs_the_network_only_in_do_step(self):
        fmu = self.slave(self.echo)
        fmu.initialize(u_0=4.0)
        # Initialization does not run the network.
        self.assertEqual(fmu.get("u_seen_0"), -1)
        self.assertEqual(fmu.get("t_seen_0"), -1)
        fmu.step(0.0, 1.0)
        self.assertEqual(fmu.get("u_seen_0"), 4)
        self.assertEqual(fmu.get("t_seen_0"), 0)
        # A step in which the importer sets nothing still sees its own time.
        fmu.step(1.0, 1.0)
        self.assertEqual(fmu.get("t_seen_0"), 1)
        # Setting an input does not change the outputs until the next step.
        fmu.set("u_0", 9.0)
        self.assertEqual(fmu.get("u_seen_0"), 4)

    def test_internal_step_size_from_model_description(self):
        model_description = read_model_description(self.counter_50)
        self.assertEqual(
            float(model_description.defaultExperiment.stepSize), 50)
        fmu = self.slave(self.counter_50)
        fmu.initialize()
        # A communication step equal to the internal step runs it once.
        fmu.step(0.0, 50.0)
        self.assertEqual(fmu.get("evals_0"), 1)
        self.assertEqual(fmu.get("t_seen_0"), 0)
        # Shorter steps run it when an internal step is complete.
        fmu.step(50.0, 25.0)
        self.assertEqual(fmu.get("evals_0"), 1)
        fmu.step(75.0, 25.0)
        self.assertEqual(fmu.get("evals_0"), 2)
        self.assertEqual(fmu.get("t_seen_0"), 50)
        fmu.step(100.0, 50.0)
        self.assertEqual(fmu.get("evals_0"), 3)
        self.assertEqual(fmu.get("t_seen_0"), 100)


class TestOutputDependencies(unittest.TestCase):
    """Co-Simulation outputs depend on none of the knowns at a
    communication point (FMI 2.0.4 section 2.2.8, FMI 3.0 section 2.4.8).
    Only FMUs without Model Exchange may declare it, because Model Exchange
    outputs depend on the inputs. Uses `generate` only, no compiler."""

    def setUp(self):
        self.tmpdir = Path(tempfile.mkdtemp(prefix="onnx2fmu_test_deps_"))
        self.addCleanup(shutil.rmtree, self.tmpdir, ignore_errors=True)
        self.model_path = self.tmpdir / "echo.onnx"
        save(_build_onnx(stateful=False), self.model_path)

    def dependencies(self, fmi_version: str, fmu_type: list) -> list:
        """The `dependencies` attribute of every output and initial
        unknown in the generated model description."""
        name = f"Echo{fmi_version[0]}{len(fmu_type)}"
        description_path = self.tmpdir / f"{name}.json"
        description_path.write_text(json.dumps({
            "name": name,
            "description": "",
            "FMIVersion": fmi_version,
            "FMUType": fmu_type,
            "inputs": [{"name": "u"}, {"name": "t", "time": True}],
            "outputs": [{"name": "u_seen", "start": 0.0},
                        {"name": "t_seen", "start": 0.0}],
        }))
        target = self.tmpdir / f"target_{name}"
        generate(model_path=self.model_path,
                 model_description_path=description_path,
                 target_folder=target)
        if fmi_version == "2.0":
            root = ElementTree.parse(target / name / "FMI2.xml").getroot()
            unknowns = root.findall("ModelStructure/Outputs/Unknown") + \
                root.findall("ModelStructure/InitialUnknowns/Unknown")
        else:
            root = ElementTree.parse(target / name / "FMI3.xml").getroot()
            unknowns = root.findall("ModelStructure/Output") + \
                root.findall("ModelStructure/InitialUnknown")
        self.assertEqual(len(unknowns), 4)
        return [unknown.get("dependencies") for unknown in unknowns]

    def test_co_simulation_only_outputs_depend_on_nothing(self):
        for fmi_version in ["2.0", "3.0"]:
            with self.subTest(fmi_version=fmi_version):
                self.assertEqual(
                    self.dependencies(fmi_version, ["CoSimulation"]),
                    [""] * 4)

    def test_model_exchange_outputs_keep_the_default(self):
        for fmi_version in ["2.0", "3.0"]:
            with self.subTest(fmi_version=fmi_version):
                self.assertEqual(
                    self.dependencies(fmi_version,
                                      ["ModelExchange", "CoSimulation"]),
                    [None] * 4)


class TestCoSimulationFMI2(CoSimulationSemantics, unittest.TestCase):
    fmi_version = "2.0"
    # FMI 2.0 has no fixedInternalStepSize: the step is the DefaultExperiment
    # stepSize.
    step_50_keys = {"defaultExperiment": {"stepSize": 50}}


class TestCoSimulationFMI3(CoSimulationSemantics, unittest.TestCase):
    fmi_version = "3.0"
    step_50_keys = {"fixedInternalStepSize": 50}

    def test_fixed_internal_step_size_attribute(self):
        model_description = read_model_description(self.counter_50)
        self.assertEqual(
            float(model_description.coSimulation.fixedInternalStepSize), 50)


if __name__ == "__main__":
    unittest.main()
