# 导入库
import numpy as np
import matplotlib.pyplot as plt

# 数据读取
xy = np.loadtxt('coordinate.txt')
phi = np.loadtxt('x.txt')
# 坐标提取
x = xy[:, 0]
y = xy[:, 1]

# 结果可视化
plt.tricontourf(x, y, phi)
plt.title(r'$-\nabla^2\varphi=1$')
plt.colorbar()
plt.axis('image')
plt.show()