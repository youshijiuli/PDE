import numpy as np
import matplotlib.pyplot as plt
import mpl_toolkits.mplot3d


# 定义一个名为HeatEquation的类，用于表示偏微分方程。
# 类包含以下参数：alpha、X、T和初始条件（ic）和边界条件（bcl和bcr）。
class HeatEquation:
    def __init__(
        self,
        alpha,
        X,
        T,
        initialCondition=lambda x: x * (1 - x),
        boundaryConditionL=lambda x: 0,
        boundaryConditionR=lambda x: 0,
    ):
        self.alpha = alpha
        self.ic = initialCondition
        self.bcl = boundaryConditionL
        self.bcr = boundaryConditionR
        self.X = X
        self.T = T

# 定义一个名为ExplicitEulerScheme的类，用于表示显式欧拉法求解偏微分方程。
# 类包含以下参数：M、N和equation
class ExplicitEulerScheme:
    def __init__(self, M, N, equation):
        self.eq = equation
        self.dt = self.eq.T / M
        self.dx = self.eq.X / N
        self.T = np.zeros((N + 1, M + 1))
        self.xArray = np.linspace(0, self.eq.X, N + 1)
        self.T[:, 0] = self.eq.ic(self.xArray)
        self.r = self.eq.alpha * self.dt / self.dx / self.dx
        self.M = M
        self.N = N

    # 用于回溯求解时间步长
    def roll_back(self):
        for n in range(0, self.M):
            for i in range(1, self.N):
                self.T[i][n + 1] = (
                    self.r * self.T[i - 1][n]
                    + (1 - 2 * self.r) * self.T[i][n]
                    + self.r * self.T[i + 1][n]
                )
        self.T[0][n + 1] = self.eq.bcl(self.xArray[0])
        self.T[self.N][n + 1] = self.eq.bcr(self.xArray[-1])

    # 用于生成网格数据
    def mesh_grids(self):
        tArray = np.linspace(0, self.eq.T, self.M + 1)
        tGrids, xGrids = np.meshgrid(tArray, self.xArray)
        return tGrids, xGrids


# 创建一个HeatEquation类的实例，并设置alpha、X和T的值。
ht = HeatEquation(1.0, 1.0, 1.0)

# 创建一个ExplicitEulerScheme类的实例，并设置M和N的值。
scheme = ExplicitEulerScheme(2500, 25, ht)

# 求解时间步长
scheme.roll_back()

# 在主程序中，调用mesh_grids方法，生成网格数据。
tGrids, xGrids = scheme.mesh_grids()

# 创建一个画布，并添加一个3D子图
fig = plt.figure(figsize=(16, 10))
ax = fig.add_subplot(1, 1, 1, projection="3d")


cutoff = int(0.2 / scheme.dt) + 1

# 在3D子图中，绘制曲面，使用plot_surface方法
surface = ax.plot_surface(
    xGrids[:, :cutoff], tGrids[:, :cutoff], scheme.T[:, :cutoff], cmap=plt.cm.Blues_r
)
ax.set_xlabel("$x$")
ax.set_ylabel("$t$")
ax.set_zlabel("$T$")
ax.set_title("$T_{t} = T_{xx}$")
fig.colorbar(surface, shrink=0.75)
plt.grid()


plt.show()

# 这个画出的图的颜色深浅表示的是解的值。蓝色表示解的值较大，白色表示解的值较小。
# 可以看出，在二维欧拉方程中，解的值随着x和y的变化而变化。颜色深浅代表了求解后的解的值。