import torch
import torch.nn as nn
import numpy as np
import matplotlib.pyplot as plt
import time


def f(x):
    return 0 * x[:, 0]


def u_accuracy(x):
    out = 0
    for i in range(int(x.size(1) / 2)):
        out += x[:, 2 * i] * x[:, 2 * i + 1]
    return out


def u_boundary(x):
    out = 0
    for i in range(int(x.size(1) / 2)):
        out += x[:, 2 * i] * x[:, 2 * i + 1]
    return out


dim = 10


class INSET:
    def __init__(self):
        self.size = 800
        self.X = torch.rand(self.size, dim)
        self.u_acc = u_accuracy(self.X).view(-1, 1)
        self.right = f(self.X).view(-1, 1)


class BDSET:
    def __init__(self):
        self.rock = 40
        self.ro = 10
        self.size = self.rock * dim
        self.X = torch.rand(self.size, dim)
        for i in range(dim):
            self.X[i * self.rock : i * self.rock + self.ro, i] = 0  # 以40为一个单元，其中10取0
            self.X[
                i * self.rock + self.ro : (i + 1) * self.rock, i
            ] = 1  ##以40为一个单元，其中30取1
        self.ub = u_boundary(self.X).view(-1, 1)


class TESET:
    def __init__(self):
        self.size = 800
        self.X = torch.rand(self.size, dim)
        self.u_acc = u_accuracy(self.X).view(-1, 1)


# 该问题求解区域为10维空间的[0,1]，为此需要重新设定mi,ma
class SIN(nn.Module):
    def __init__(self, order):
        super(SIN, self).__init__()
        self.e = order

    def forward(self, x):
        return torch.relu(x) ** self.e


class Res(nn.Module):
    def __init__(self, input_size, output_size):
        super(Res, self).__init__()
        self.model = nn.Sequential(
            nn.Linear(input_size, output_size),
            SIN(2),
            nn.Linear(output_size, output_size),
            SIN(2),
        )
        self.input = input_size
        self.output = output_size

    def forward(self, x):
        x = self.model(x) + x @ torch.eye(x.size(1), self.output)
        return x


class NETG(nn.Module):  # u = netg*lenthfactor + netf，此为netf，此netg逼近内部点取值
    def __init__(self):
        super(NETG, self).__init__()
        self.model = nn.Sequential(Res(dim, 15), Res(15, 10), Res(10, 1))

    def forward(self, x):
        out = self.model(x)
        return out


def deep_pred(net, model):  # u = netg*lenthfactor + netf
    return net.forward(model.X)


def deep_error(net, model):
    fenzi = ((deep_pred(net, model) - model.u_acc) ** 2).detach().numpy().sum()
    fenmu = (model.u_acc**2 + 1e-7).detach().numpy().sum()
    return np.sqrt(fenzi / fenmu)


def loss(net, inset, bdset, beta):
    inset.X.requires_grad = True
    u_in = deep_pred(net, inset)
    (ux,) = torch.autograd.grad(
        u_in,
        inset.X,
        create_graph=True,
        retain_graph=True,
        grad_outputs=torch.ones(inset.size, 1),
    )
    ub = deep_pred(net, bdset)
    return (
        0.5 * (ux**2).sum()
        - (inset.right * u_in).sum()
        + beta * ((ub - bdset.ub) ** 2).sum()
    )


def train(net, inset, bdset, beta, optim, epoch):
    print("train Deep Ritz neural network ")
    ERROR, BUZHOU = [], []
    LOSS = loss(net, inset, bdset, beta)
    lossoptimal = LOSS
    trainerror = deep_error(net, inset)
    print("epoch:%d,loss:%.3e,train error:%.3e" % (0, LOSS.item(), trainerror))
    torch.save(net.state_dict(), "best_net.pkl")
    cycle = 100
    for i in range(epoch):
        st = time.time()
        for j in range(cycle):
            optim.zero_grad()
            LOSS = loss(net, inset, bdset, beta)
            LOSS.backward()
            optim.step()
        if LOSS < lossoptimal:
            lossoptimal = LOSS
            torch.save(net.state_dict(), "best_net.pkl")
        ela = time.time() - st
        trainerror = deep_error(net, inset)
        ERROR.append(trainerror)
        BUZHOU.append((i + 1) * cycle)
        print(
            "epoch:%d,loss:%.3e,train error:%.3e,time:%.2f"
            % ((i + 1) * cycle, LOSS.item(), trainerror, ela)
        )
    return ERROR, BUZHOU


net = NETG()
optim = torch.optim.Adam(net.parameters(), lr=5e-3)
epoch = 10000
beta = 1e-2
OLDERROR, BUZHOU = train(net, INSET(), BDSET(), beta, optim, epoch)
testerror = deep_error(net, TESET())
print(testerror)
