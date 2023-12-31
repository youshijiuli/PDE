import torch
import numpy as np
from matplotlib import pyplot as plt


def UU(prob, X):
    if prob == 1:
        return X[:, 0] ** 2 + X[:, 1] ** 2
    if prob == 2:
        r = (X[:, 0] ** 2 + X[:, 1] ** 2) ** (0.5)
        theta = np.arctan(X[:, 1] / (X[:, 0] + 1e-8))
        return torch.sin(theta / 2) * r ** (0.5)
    if prob == 3:
        return X[:, 0] * X[:, 1]


def f(prob, X):
    if prob == 1:
        return -4 * torch.exp(-X[:, 0]) * torch.exp(X[:, 0])
    if prob == 2:
        return 0 * X[:, 0]
    if prob == 3:
        return 0 * X[:, 0]


class TESET:
    def __init__(self, bound, nx, prob):
        self.dim = 2
        self.nx = nx
        self.prob = prob
        self.bound = bound
        self.hx = [
            (bound[0, 1] - bound[0, 0]) / (nx[0] - 1),
            (bound[1, 1] - bound[1, 0]) / (nx[1] - 1),
        ]
        self.X = torch.zeros(nx[1] * nx[0], self.dim)
        for i in range(self.nx[0]):
            for j in range(self.nx[1]):
                self.X[i * self.nx[1] + j, 0] = self.bound[0, 0] + i * self.hx[0]
                self.X[i * self.nx[1] + j, 1] = self.bound[1, 0] + j * self.hx[1]
        self.u_acc = UU(prob, self.X).view(-1, 1)

    def matrix(self):
        self.A = torch.zeros(self.nx[0] * self.nx[1], self.nx[0] * self.nx[1])
        self.D = np.zeros_like(self.A)
        self.L = np.zeros_like(self.A)
        self.U = np.zeros_like(self.A)
        dx = self.hx[0]
        dy = self.hx[1]
        for i in range(self.nx[0]):
            for j in range(self.nx[1]):
                dx = self.hx[0]
                dy = self.hx[1]
                if i == 0 or i == self.nx[0] - 1 or j == 0 or j == self.nx[1] - 1:
                    self.A[i * self.nx[1] + j, i * self.nx[1] + j] = 1
                elif (
                    self.X[i * self.nx[1] + j, 0] >= 0
                    and self.X[i * self.nx[1] + j, 1] == 1
                ):
                    self.A[i * self.nx[1] + j, i * self.nx[1] + j] = 1
                else:
                    self.A[i * self.nx[1] + j, i * self.nx[1] + j] = 2 * (
                        dx / dy + dy / dx
                    )

                    self.A[i * self.nx[1] + j, i * self.nx[1] + j - 1] = -dx / dy
                    self.L[i * self.nx[1] + j, i * self.nx[1] + j - 1] = -self.A[
                        i * self.nx[1] + j, i * self.nx[1] + j - 1
                    ]
                    # ------------------------------
                    self.A[i * self.nx[1] + j, i * self.nx[1] + j + 1] = -dx / dy
                    self.U[i * self.nx[1] + j, i * self.nx[1] + j + 1] = -self.A[
                        i * self.nx[1] + j, i * self.nx[1] + j + 1
                    ]
                    # -----------------------------
                    self.A[i * self.nx[1] + j, (i - 1) * self.nx[1] + j] = -dy / dx
                    self.L[i * self.nx[1] + j, (i - 1) * self.nx[1] + j] = -self.A[
                        i * self.nx[1] + j, (i - 1) * self.nx[1] + j
                    ]
                    # ----------------------------------------
                    self.A[i * self.nx[1] + j, (i + 1) * self.nx[1] + j] = -dy / dx
                    self.U[i * self.nx[1] + j, (i + 1) * self.nx[1] + j] = -self.A[
                        i * self.nx[1] + j, (i + 1) * self.nx[1] + j
                    ]
                self.D[i * self.nx[1] + j, i * self.nx[1] + j] = self.A[
                    i * self.nx[1] + j, i * self.nx[1] + j
                ]
        return self.A, self.D, self.L, self.U

    def right(self):
        self.b = torch.zeros(self.nx[0] * self.nx[1], 1)
        for i in range(self.nx[0]):
            for j in range(self.nx[1]):
                dx = self.hx[0]
                dy = self.hx[1]
                x = i * dx
                y = j * dy
                X = torch.tensor([[x, y]]).float()
                if i == 0 or i == self.nx[0] - 1 or j == 0 or j == self.nx[1] - 1:
                    self.b[i * nx[1] + j] = UU(
                        prob, self.X[i * nx[1] + j : i * nx[1] + j + 1, :]
                    )
                elif (
                    self.X[i * self.nx[1] + j, 0] >= 0
                    and self.X[i * self.nx[1] + j, 1] == 1
                ):
                    self.b[i * nx[1] + j] = UU(
                        prob, self.X[i * nx[1] + j : i * nx[1] + j + 1, :]
                    )
                else:
                    self.b[i * nx[1] + j] = (
                        f(prob, self.X[i * nx[1] + j : i * nx[1] + j + 1, :]) * dx * dy
                    )
        return self.b

    def SOR(self, eps, n):
        A, D, L, U = self.matrix()
        b = self.right()
        omega = 1
        MI = np.dot(np.linalg.inv(D - omega * L), (1 - omega) * D + omega * U)
        BI = omega * np.dot(np.linalg.inv(D - omega * L), b)
        x = np.random.rand(nx[0] * nx[1], 1)
        eps = 1e-9
        for i in range(n):
            x_old = x
            x = np.dot(MI, x_old) + BI
            if max(abs(x - x_old)) < eps:
                break
        print(i)
        return x


bound = torch.tensor([[-1, 1], [-1, 1]]).float()
nx = [10, 20]
prob = 3
teset = TESET(bound, nx, prob)

u_pred = teset.SOR(1e-9, 1000)


def error(u_pred, u_acc):
    fenzi = ((u_pred - u_acc) ** 2).sum()
    fenmu = (u_acc**2 + 1e-6).sum()
    return np.sqrt(fenzi / fenmu)


print(error(teset.u_acc.numpy(), u_pred))

x_train = np.linspace(bound[0, 0], bound[0, 1], nx[0])
y_train = np.linspace(bound[1, 0], bound[1, 1], nx[1])
x, y = np.meshgrid(x_train, y_train)
plt.contourf(x, y, u_pred.reshape(nx[0], nx[1]).T, 40, cmap="Blues")
plt.colorbar()
plt.xlabel("x")
plt.ylabel("y")
plt.title("the FD solution")

"""389
2.064598075997314e-07"""