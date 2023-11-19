# -*- coding: utf-8 -*-
"""
Created on Mon Aug 21 15:22:22 2023

@author: xudonghan
"""

# 导入库
import numpy as np
import matplotlib.pyplot as plt

# 导入坐标数据
xy = np.loadtxt('coordinate.txt')
# 导入矩阵方程
A = np.loadtxt('A.txt')
b = np.loadtxt('b.txt')

# 坐标提取
x = xy[:, 0]
y = xy[:, 1]
# 场值计算
phi = np.linalg.solve(A, b)

# 计算结果可视化
plt.tricontourf(x, y, phi, cmap = 'rainbow')
plt.title(r'$-\nabla^2\varphi$=0')
plt.colorbar()
plt.axis('image')
plt.show()