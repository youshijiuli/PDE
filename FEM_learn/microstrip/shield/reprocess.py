# -*- coding: utf-8 -*-
"""
Created on Mon Aug 21 16:25:45 2023

@author: xudonghan
"""

# 导入库
import numpy as np
import matplotlib.pyplot as plt

# 导入数据
xy = np.loadtxt('coordinate.txt')
phi = np.loadtxt('phi.txt')
# 坐标提取
x = xy[:, 0]
y = xy[:, 1]

# 结果可视化
plt.tricontourf(x, y, phi, levels = 10, cmap = 'rainbow')
plt.triplot(x, y, 'c-', linewidth=0.3)
plt.title(r'$-\nabla^2\varphi$=0')
plt.colorbar()
plt.axis('image')
plt.show()