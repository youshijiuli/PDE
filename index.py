#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
@File    :   index.py
@Author  :   Cat 
@Version :   3.11
"""

# 有限差分法求解PDE

"""这是一个使用有限差分法求解偏导数方程的示例。
在这个例子中，我们使用一维的欧拉方程作为例子。
我们首先定义了一些基本函数，如UU和f，它们分别表示欧拉方程的左部和右部。
然后我们创建了一个名为TESET的类，用于表示有限差分法的问题。在这个类中，我们定义了一些基本的属性和方法，如矩阵、右端、解和误差。
最后，我们使用TESET类来求解欧拉方程，并绘制出解的图像。"""

import torch
import numpy as np
import matplotlib.pyplot as plt


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
                    self.A[i * self.nx[1] + j, i * self.nx[1] + j + 1] = -dx / dy
                    self.A[i * self.nx[1] + j, (i - 1) * self.nx[1] + j] = -dy / dx
                    self.A[i * self.nx[1] + j, (i + 1) * self.nx[1] + j] = -dy / dx
        return self.A

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

    def solve(self):
        A = self.matrix()
        b = self.right()
        # u, lu = torch.solve(b, A)   上面的代码已经被弃用
        u = torch.linalg.solve(A, b)
        return u


bound = torch.tensor([[-1, 1], [-1, 1]]).float()
nx = [10, 20]
prob = 3
teset = TESET(bound, nx, prob)
u_pred = teset.solve()


def error(u_pred, u_acc):
    fenzi = ((u_pred - u_acc) ** 2).sum()
    fenmu = (u_acc**2 + 1e-6).sum()
    return np.sqrt(fenzi / fenmu)


print(error(teset.u_acc, teset.solve()))

x_train = np.linspace(bound[0, 0], bound[0, 1], nx[0])
y_train = np.linspace(bound[1, 0], bound[1, 1], nx[1])
x, y = np.meshgrid(x_train, y_train)
plt.contourf(x, y, u_pred.reshape(nx[0], nx[1]).t(), 40, cmap="Blues")
plt.colorbar()
plt.xlabel("x")
plt.ylabel("y")
plt.title("the FD solution")
plt.grid()
plt.show()


"""当然可以。下面我将详细介绍每个函数的功能和参数。

UU(prob, X)：
功能：计算欧拉方程的左部，即U(x, y)。
参数：
prob：表示问题的维度，取值为1、2或3，分别表示一维欧拉方程、二维欧拉方程和三维欧拉方程。
X：表示问题的输入，是一个大小为nx2的二维张量，其中每一行表示一个样本点。
f(prob, X)：
功能：计算欧拉方程的右部，即f(x, y)。
参数：
prob：表示问题的维度，取值为1、2或3，分别表示一维欧拉方程、二维欧拉方程和三维欧拉方程。
X：表示问题的输入，是一个大小为nx2的二维张量，其中每一行表示一个样本点。
TESET(bound, nx, prob)：
功能：创建一个TESET类的实例，用于求解欧拉方程。
参数：
bound：表示问题的边界，是一个大小为2x2的二维张量，分别表示x和y方向的边界。
nx：表示问题的网格点数，是一个大小为2的二维张量，分别表示x和y方向的网格点数。
prob：表示问题的维度，取值为1、2或3，分别表示一维欧拉方程、二维欧拉方程和三维欧拉方程。
matrix(self)：
功能：计算有限差分法的矩阵。
参数：
self：表示TESET类的实例。
right(self)：
功能：计算有限差分法的右端。
参数：
self：表示TESET类的实例。
solve(self)：
功能：使用有限差分法求解欧拉方程。
参数：
self：表示TESET类的实例。
error(u_pred, u_acc)：
功能：计算两个解的误差。
参数：
u_pred：表示预测解。
u_acc：表示实际解。
    """