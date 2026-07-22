"""Train a small neural ODE (torchdiffeq) that approximates a forced Van der
Pol oscillator, export the *derivative network* to ONNX, and write the
ONNX2FMU model description for a Model Exchange FMU.

Unlike examples 1-4, the network learned here is not the end-to-end model:
it is `f_theta(t, x)`, the right-hand side of an ODE `dx/dt = f_theta(t, x)`.
ONNX2FMU wraps it as a Model Exchange FMU, whose *importer* integrates it
(e.g. with CVode) -- ONNX2FMU never runs an internal solver over it. This
mirrors how the network was trained: torchdiffeq's `odeint` integrates the
same right-hand side during training and backpropagates through the solve.

The dynamics used as the training target are a forced Van der Pol
oscillator:

    dx0/dt = x1
    dx1/dt = (1 - x0^2) * x1 - x0 + A * sin(omega * t)

which is why the exported network also takes the simulation time `t` as an
input (declared with `"time": true` in the model description) -- the forcing
term makes the system non-autonomous.

Run with: uv run python train_and_export.py
"""
import json
import numpy as np
import torch
from torch import nn
import onnx
from torchdiffeq import odeint


torch.manual_seed(0)
np.random.seed(0)

A, OMEGA = 0.5, 1.0


def true_derivative(t: float, x: np.ndarray) -> np.ndarray:
    return np.array([
        x[1],
        (1.0 - x[0] ** 2) * x[1] - x[0] + A * np.sin(OMEGA * t),
    ])


def rk4_trajectory(x0, t_eval, dt=1e-3):
    """Reference trajectory of the true dynamics, sampled at t_eval."""
    x = np.array(x0, dtype=float)
    t = 0.0
    out = [x.copy()]
    next_idx = 1
    n_steps = int(round(t_eval[-1] / dt))
    for _ in range(n_steps):
        k1 = true_derivative(t, x)
        k2 = true_derivative(t + dt / 2, x + dt / 2 * k1)
        k3 = true_derivative(t + dt / 2, x + dt / 2 * k2)
        k4 = true_derivative(t + dt, x + dt * k3)
        x = x + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
        t += dt
        if next_idx < len(t_eval) and abs(t - t_eval[next_idx]) < dt / 2:
            out.append(x.copy())
            next_idx += 1
    return np.array(out)


class DerivativeNet(nn.Module):
    """f_theta(x, t) -> dx/dt. This is the module exported to ONNX, with
    inputs in ONNX2FMU's order: state first, time second."""

    def __init__(self, hidden: int = 32):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(3, hidden), nn.Tanh(),
            nn.Linear(hidden, hidden), nn.Tanh(),
            nn.Linear(hidden, 2),
        )

    def forward(self, x: torch.Tensor, t: torch.Tensor) -> torch.Tensor:
        if x.dim() > 1:
            t_col = t.reshape(1).expand(x.shape[0], 1)
        else:
            t_col = t.reshape(1)
        return self.net(torch.cat([x, t_col], dim=-1))


class TorchdiffeqRHS(nn.Module):
    """torchdiffeq.odeint calls func(t, y); wrap DerivativeNet to match."""

    def __init__(self, net: DerivativeNet):
        super().__init__()
        self.net = net

    def forward(self, t: torch.Tensor, y: torch.Tensor) -> torch.Tensor:
        return self.net(y, t)


def make_training_set(n_trajectories=16, t_span=10.0, n_points=41):
    t_eval = np.linspace(0.0, t_span, n_points)
    x0s = np.random.uniform(low=[-2.5, -2.5], high=[2.5, 2.5],
                            size=(n_trajectories, 2))
    trajectories = np.stack(
        [rk4_trajectory(x0, t_eval) for x0 in x0s], axis=1
    )  # (n_points, n_trajectories, 2)
    return (
        torch.tensor(t_eval, dtype=torch.float32),
        torch.tensor(x0s, dtype=torch.float32),
        torch.tensor(trajectories, dtype=torch.float32),
    )


def train(n_iters=1200, lr=1e-2):
    t_eval, x0s, target = make_training_set()
    net = DerivativeNet()
    rhs = TorchdiffeqRHS(net)
    optimizer = torch.optim.Adam(rhs.parameters(), lr=lr)
    scheduler = torch.optim.lr_scheduler.StepLR(
        optimizer, step_size=300, gamma=0.5)

    for it in range(n_iters):
        optimizer.zero_grad()
        pred = odeint(rhs, x0s, t_eval, method="rk4",
                      options={"step_size": 0.05})
        loss = torch.mean((pred - target) ** 2)
        loss.backward()
        torch.nn.utils.clip_grad_norm_(rhs.parameters(), max_norm=1.0)
        optimizer.step()
        scheduler.step()
        if it % 50 == 0 or it == n_iters - 1:
            print(f"iter {it:4d}  loss {loss.item():.6f}  "
                  f"lr {scheduler.get_last_lr()[0]:.5f}")

    return net


def export_onnx(net: DerivativeNet, path: str):
    net.eval()
    x = torch.tensor([2.0, 0.0])
    # ONNX2FMU requires every declared node to have rank >= 1 (a 0-d scalar
    # shape is rejected), so "t" must be exported with shape (1,), not as a
    # bare scalar tensor.
    t = torch.tensor([0.0])
    torch.onnx.export(
        net, (x, t), path,
        input_names=["x", "t"],
        output_names=["dx"],
    )
    onnx_model = onnx.load(path)
    onnx.checker.check_model(onnx_model)
    # Pin the IR version to what the pinned ONNX Runtime release in
    # onnx2fmu/CMakeLists.txt actually supports (avoids a load failure if
    # the local `onnx` package is newer than the pinned ORT release).
    onnx_model.ir_version = 10
    onnx_model.graph.doc_string = \
        "Right-hand side of a forced Van der Pol neural ODE, trained with torchdiffeq."
    onnx.save(onnx_model, path)


def write_model_description(path: str):
    description = {
        "name": "example5",
        "description": (
            "Neural ODE (trained with torchdiffeq) approximating a forced "
            "Van der Pol oscillator. dx/dt = f_theta(t, x); the FMU's own "
            "Model Exchange solver integrates it."
        ),
        "FMIVersion": "3.0",
        "FMUType": ["ModelExchange", "CoSimulation"],
        "inputs": [
            {
                "name": "x",
                "description": "State vector [x0, x1].",
                "labels": ["x0", "x1"],
                "start": [2.0, 0.0],
            },
            {
                "name": "t",
                "description": "Simulation time, fed to the forcing term.",
                "time": True,
            },
        ],
        "outputs": [
            {
                "name": "dx",
                "description": "dx/dt, the learned state derivative.",
                "labels": ["der_x0", "der_x1"],
                "derivativeOf": "x",
            },
        ],
    }
    with open(path, "w", encoding="utf-8") as f:
        json.dump(description, f, indent=4)


def write_reference_trajectory(path: str, x0=(2.0, 0.0), t_span=10.0,
                               n_points=21):
    t_eval = np.linspace(0.0, t_span, n_points)
    trajectory = rk4_trajectory(list(x0), t_eval)
    import pandas as pd
    df = pd.DataFrame(trajectory, columns=["x_0", "x_1"])
    df["time"] = t_eval
    df.set_index("time", inplace=True)
    df.to_csv(path)


if __name__ == "__main__":
    net = train()
    export_onnx(net, "example5.onnx")
    write_model_description("example5Description.json")
    write_reference_trajectory("Example5_ref.csv")
    print("Done: example5.onnx, example5Description.json, Example5_ref.csv")
