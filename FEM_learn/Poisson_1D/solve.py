# 导入库
import numpy as np
import matplotlib.pyplot as plt

# 解析解
x = np.linspace(0, 1, 101)
phi = np.exp(2*x)*np.sin(4*np.pi*x)

# 有限元求解
data = np.loadtxt('out.txt')
X = data[:, 0]; Phi = data[:, 1]

# 绘图
plt.figure()
plt.plot(x, phi, 'r', label = "exact solve")
plt.scatter(X, Phi, label = "FEM solve")
plt.xlabel('$x$'); plt.ylabel(r'$\varphi$')
# plt.title('$-d^2\phi/dx^2=[(16\pi^2-4)\sin(4\pi x)-16\pi\cos(4\pi)]\exp(2x)$')
plt.legend()
# plt.show()

plt.savefig('solve.svg', transparent=True)
