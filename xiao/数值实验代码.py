import torch
import torch.nn as nn
import numpy as np
import matplotlib.pyplot as plt
import time


def f(x, y):
    return 2 * (np.pi**2) * (np.cos(np.pi * x) + np.cos(np.pi * y))


def u_accuracy(x, y):
    return np.cos(np.pi * x) + np.cos(np.pi * y)


"""
def kernel(x,y):
    x_ = (x - xa/2 - xb/2)/(xb - xa)
    y_ = (y - ya/2 - yb/2)/(yb - ya)
    h = (xb -xa)**2 + (yb - ya)**2
    return np.exp(-(x_**2 + y_**2)/(2*h))
"""


def kernel(x, y):
    return np.exp(x) * np.exp(-x)


xa, xb = 0, 1
ya, yb = 0, 1


class INSET:
    def __init__(self):
        self.xa, self.xb = xa, xb
        self.ya, self.yb = ya, yb
        self.nx, self.ny = 20, 30
        self.hx, self.hy = (self.xb - self.xa) / self.nx, (self.yb - self.ya) / self.ny
        self.size = self.nx * self.ny
        self.X = torch.zeros(self.size, 2)
        for i in range(self.nx):
            for j in range(self.ny):
                self.X[i * self.ny + j, 0] = self.xa + (i + 0.5) * self.hx
                self.X[i * self.ny + j, 1] = self.ya + (j + 0.5) * self.hy
        self.right = f(self.X[:, 0], self.X[:, 1]).view(-1, 1)
        self.ker = kernel(self.X[:, 0], self.X[:, 1]).view(-1, 1)
        self.u_acc = u_accuracy(self.X[:, 0], self.X[:, 1]).view(-1, 1)


class TESET:
    def __init__(self):
        self.xa, self.xb = xa, xb
        self.ya, self.yb = ya, yb
        self.hx = 0.1
        self.hy = 0.05
        self.nx = int((self.xb - self.xa) / self.hx) + 1
        self.ny = int((self.yb - self.ya) / self.hy) + 1
        self.size = self.nx * self.ny
        self.X = torch.zeros(self.size, 2)
        for j in range(self.ny):
            for i in range(self.nx):
                self.X[j * self.nx + i, 0] = self.xa + i * self.hx
                self.X[j * self.nx + i, 1] = self.ya + j * self.hy
        self.right = f(self.X[:, 0], self.X[:, 1]).view(-1, 1)
        self.ker = kernel(self.X[:, 0], self.X[:, 1]).view(-1, 1)
        self.u_acc = u_accuracy(self.X[:, 0], self.X[:, 1]).view(-1, 1)


np.random.seed(1234)
torch.manual_seed(1234)


class Res(nn.Module):
    def __init__(self, input_size, output_size):
        super(Res, self).__init__()
        self.model = nn.Sequential(
            nn.Linear(input_size, output_size),
            nn.Tanh(),
            nn.Linear(output_size, output_size),
            nn.LeakyReLU(inplace=True),
        )
        self.input = input_size
        self.output = output_size

    def forward(self, x):
        x = self.model(x) + x @ torch.eye(x.shape[1], self.output)
        return x


class NET(nn.Module):
    def __init__(self):
        super(NET, self).__init__()
        self.model = nn.Sequential(Res(2, 20), Res(20, 1))

    def forward(self, x):
        return self.model(x)


def LOSS(net, inset, belta):
    x_in = inset.X
    x_in.requires_grad = True
    mi = torch.tensor([[inset.xa, inset.ya]]).float()
    mx = torch.tensor([[inset.xb, inset.yb]]).float()
    H = 2 * (x_in - mi) / (mx - mi) - 1
    U_in = net.forward(x_in) * inset.ker
    (ux,) = torch.autograd.grad(
        U_in,
        x_in,
        create_graph=True,
        retain_graph=True,
        grad_outputs=torch.ones(inset.size, 1),
    )

    return (
        0.5
        * (
            ux[:, 0:1] ** 2
            + ux[:, 1:2] ** 2
            + (np.pi * U_in) ** 2
            - 2 * inset.right * U_in
        ).sum()
    )


def error(net, model):
    u_pred = net.forward(model.X).detach().numpy()
    u_acc = model.u_acc.detach().numpy()
    fenzi = np.sqrt(((u_acc - u_pred) ** 2).sum())
    fenmu = np.sqrt((u_acc**2 + 1e-8).sum())
    return fenzi / fenmu


"""
def train_error(net,model):
    x = model.X
    x.requires_grad = True
    u = net.forward(model.X)
    ux, = torch.autograd.grad(u,x,create_graph = True,retain_graph = True,
                             grad_outputs = torch.ones(model.size,1))
    
    uxx, = torch.autograd.grad(ux[:,0:1],x,create_graph = True,retain_graph = True,
                        grad_outputs = torch.ones(model.size,1))
    uyy, = torch.autograd.grad(ux[:,1:2],x,create_graph = True,retain_graph = True,
                        grad_outputs = torch.ones(model.size,1))
    return (((uxx[:,0:1] + uyy[:,1:2] + model.right)**2).mean())**(0.5)
"""
net = NET()
inset = INSET()
teset = TESET()
optim = torch.optim.Adam(net.parameters(), lr=5e-3)
belta = 5e2
ERROR_new = []

STEP = []
st = time.time()
for i in range(6000):
    optim.zero_grad()
    loss = LOSS(net, inset, belta)
    loss.backward()
    optim.step()
    record = 600
    if i % record == 0:
        ERROR_new.append(error(net, teset))
        STEP.append(i)
        print("step:%d,loss:%.2e,error:%.3f" % (i, loss.item(), error(net, teset)))
al = time.time() - st
print("time:%.2f" % (al))

M, N = teset.nx, teset.ny
x_train = np.linspace(teset.xa, teset.xb, M)
y_train = np.linspace(teset.ya, teset.yb, N)
x, y = np.meshgrid(x_train, y_train)
u = (net.forward(teset.X) * teset.ker).view(N, M).detach().numpy()
plt.contourf(x, y, u, 40, cmap="Blues")
plt.colorbar()
plt.xlabel("x")
plt.ylabel("y")
plt.title("the DRM solution")
plt.savefig("DRM.jpg")
plt.plot(STEP, ERROR_new, "r*", label="new method")
plt.plot(STEP, ERROR_new, color="red", label="new method")
# plt.plot(STEP, ERROR_old, "k*", label="DRM ")
# plt.plot(STEP, ERROR_old, color="blue", label="DRM")
plt.xlabel("step")
plt.ylabel("error")
plt.title("the error")
plt.savefig("error.jpg")
