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

This is the script form of `generate-example-model.ipynb`; see the notebook
for why the network encodes time as Fourier features, why it carries a
linear skip path, and why training scores short and long integration windows
at the same time.

Run with: uv run python train_and_export.py
"""
import copy
import json
from pathlib import Path

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


def true_derivative_batch(t: float, X: np.ndarray) -> np.ndarray:
    """Same right-hand side, vectorised over a batch of states, shape (N, 2)."""
    return np.stack([
        X[:, 1],
        (1.0 - X[:, 0] ** 2) * X[:, 1] - X[:, 0] + A * np.sin(OMEGA * t),
    ], axis=1)


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


def rk4_batch(X0, t_eval, dt=1e-3):
    """Reference trajectories for many initial conditions at once.

    Returns shape (n_initial_conditions, len(t_eval), 2).
    """
    X = np.array(X0, dtype=float)
    t = 0.0
    out = [X.copy()]
    next_idx = 1
    n_steps = int(round(t_eval[-1] / dt))
    for _ in range(n_steps):
        k1 = true_derivative_batch(t, X)
        k2 = true_derivative_batch(t + dt / 2, X + dt / 2 * k1)
        k3 = true_derivative_batch(t + dt / 2, X + dt / 2 * k2)
        k4 = true_derivative_batch(t + dt, X + dt * k3)
        X = X + dt / 6 * (k1 + 2 * k2 + 2 * k3 + k4)
        t += dt
        if next_idx < len(t_eval) and abs(t - t_eval[next_idx]) < dt / 2:
            out.append(X.copy())
            next_idx += 1
    return np.stack(out, axis=1)


class DerivativeNet(nn.Module):
    """f_theta(x, t) -> dx/dt. This is the module exported to ONNX, with
    inputs in ONNX2FMU's order: state first, time second.

    Time enters as `sin`/`cos` features at a few fixed frequencies rather
    than as a raw `t` sweeping 0..30 s, which would saturate the first
    `tanh` immediately, and a linear skip path carries the part of the
    vector field that is linear in those features. Both use ordinary ONNX
    ops, so the exported graph still runs in the pinned ONNX Runtime.
    """

    def __init__(self, hidden: int = 128, freqs=(0.25, 0.5, 1.0, 2.0),
                 depth: int = 2):
        super().__init__()
        self.register_buffer("freqs", torch.tensor(freqs, dtype=torch.float32))
        n_in = 2 + 2 * len(freqs)
        layers = [nn.Linear(n_in, hidden), nn.Tanh()]
        for _ in range(depth - 1):
            layers += [nn.Linear(hidden, hidden), nn.Tanh()]
        layers += [nn.Linear(hidden, 2)]
        self.net = nn.Sequential(*layers)
        self.skip = nn.Linear(n_in, 2)

    def features(self, x: torch.Tensor, t: torch.Tensor) -> torch.Tensor:
        """[x, sin(w*t), cos(w*t)], with t broadcast over x's batch shape."""
        if x.dim() > 1:
            t_col = t.reshape(-1, 1).expand(x.shape[0], 1)
        else:
            t_col = t.reshape(1, 1)
        angles = t_col * self.freqs
        feats = torch.cat([torch.sin(angles), torch.cos(angles)], dim=-1)
        if x.dim() == 1:
            feats = feats.reshape(-1)
        return torch.cat([x, feats], dim=-1)

    def forward(self, x: torch.Tensor, t: torch.Tensor) -> torch.Tensor:
        z = self.features(x, t)
        return self.net(z) + self.skip(z)


class TorchdiffeqRHS(nn.Module):
    """torchdiffeq.odeint calls func(t, y); wrap DerivativeNet to match.

    `t_offset` shifts the solver's time so that a batch of training segments
    cut from different points of the horizon each see their own absolute
    time, which the forcing term needs.
    """

    def __init__(self, net: DerivativeNet, t_offset=0.0):
        super().__init__()
        self.net = net
        self.t_offset = t_offset

    def forward(self, t: torch.Tensor, y: torch.Tensor) -> torch.Tensor:
        return self.net(y, t + self.t_offset)


def make_training_set(n_trajectories=256, t_span=30.0, dt=0.05, seed=0):
    """Reference trajectories from random initial conditions.

    Returns the time grid and a (n_trajectories, n_points, 2) tensor.
    """
    rng = np.random.RandomState(seed)
    t_grid = np.arange(0.0, t_span + dt / 2, dt)
    x0s = rng.uniform(low=[-2.5, -2.5], high=[2.5, 2.5],
                      size=(n_trajectories, 2))
    trajectories = rk4_batch(x0s, t_grid)
    return (
        torch.tensor(t_grid, dtype=torch.float32),
        torch.tensor(trajectories, dtype=torch.float32),
    )


def segment_loss(rhs, t_grid, trajectories, rng, window, batch, step=0.05):
    """Integrate `batch` random segments of `window` samples and score them."""
    n_trajectories, n_points, _ = trajectories.shape
    starts = rng.randint(0, n_points - window, size=batch)
    idx = starts[:, None] + np.arange(window)[None, :]
    target = trajectories[rng.randint(0, n_trajectories, size=batch)[:, None], idx]
    rhs.t_offset = t_grid[starts].reshape(-1, 1)
    t_rel = t_grid[:window] - t_grid[0]
    pred = odeint(rhs, target[:, 0], t_rel, method="rk4",
                  options={"step_size": step})
    return torch.mean((pred - target.transpose(0, 1)) ** 2)


def rollout_error(net, t_grid, trajectories):
    """Full-horizon rollouts from held-out initial states, scored two ways.

    Returns (rms, worst). Selection below uses the RMS: a couple of initial
    conditions sit near the unstable fixed point, where the trajectory spends
    a long time spiralling out and its *phase* is badly conditioned, so their
    error saturates at roughly the oscillation amplitude however good the
    model is. Picking checkpoints on the worst case just tracks those two and
    ignores everything else.
    """
    with torch.no_grad():
        pred = odeint(TorchdiffeqRHS(net), trajectories[:, 0], t_grid,
                      method="rk4", options={"step_size": 0.02})
    err = (pred - trajectories.transpose(0, 1)).abs()
    return err.pow(2).mean().sqrt().item(), err.max().item()


def train(n_iters=10000, lr=3e-3, short_window=3, short_batch=512,
          long_window=(20, 100), long_batch=32, w_short=400.0, w_long=0.3,
          seed=0):
    torch.manual_seed(seed)
    t_grid, trajectories = make_training_set(seed=seed)
    _, val_trajectories = make_training_set(n_trajectories=8, seed=seed + 99)

    net = DerivativeNet()
    rhs = TorchdiffeqRHS(net)
    optimizer = torch.optim.Adam(net.parameters(), lr=lr)
    scheduler = torch.optim.lr_scheduler.CosineAnnealingLR(
        optimizer, T_max=n_iters, eta_min=lr * 3e-3)
    rng = np.random.RandomState(seed + 1)
    best_err, best_state = float("inf"), copy.deepcopy(net.state_dict())

    for it in range(n_iters):
        grown = min(1.0, 2.0 * it / max(n_iters - 1, 1))
        window = int(round(long_window[0]
                           + (long_window[1] - long_window[0]) * grown))

        optimizer.zero_grad()
        short_loss = segment_loss(rhs, t_grid, trajectories, rng,
                                  short_window, short_batch)
        long_loss = segment_loss(rhs, t_grid, trajectories, rng,
                                 window, long_batch)
        (w_short * short_loss + w_long * long_loss).backward()
        torch.nn.utils.clip_grad_norm_(net.parameters(), max_norm=1.0)
        optimizer.step()
        scheduler.step()

        if it % 250 == 0 or it == n_iters - 1:
            err, worst = rollout_error(net, t_grid, val_trajectories)
            if err < best_err:
                best_err, best_state = err, copy.deepcopy(net.state_dict())
            print(f"iter {it:5d}  short {short_loss.item():.2e}  "
                  f"long {long_loss.item():.2e}  window {window:3d}  "
                  f"lr {scheduler.get_last_lr()[0]:.2e}  "
                  f"held-out rms {err:.4f}  worst {worst:.4f}")

    # Long-horizon rollout error is not monotone in the training loss, so keep
    # the weights that actually rolled out best on the held-out trajectories.
    net.load_state_dict(best_state)
    print(f"best held-out rollout RMS: {best_err:.4f}")
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
    # The exporter may spill the weights into a sidecar `.onnx.data`; the
    # re-save above inlines them again, and ONNX2FMU copies only the `.onnx`
    # into the FMU's resources.
    Path(path + ".data").unlink(missing_ok=True)


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


def write_reference_trajectory(path: str, x0=(2.0, 0.0), t_span=30.0,
                               n_points=200):
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
