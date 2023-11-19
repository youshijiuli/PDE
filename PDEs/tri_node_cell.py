import numpy as np 
import matplotlib.pyplot as plt 
from fealpy.mesh.TriangleMesh import TriangleMesh
#%matplotlib inline
# 网格顶点坐标数组
node = np.array([
    (0, 0), 
    (1, 0), 
    (1, 1),
    (0, 1)], dtype=np.float)
# 单元顶点编号数组
cell = np.array([
    (1, 2, 0), 
    (3, 0, 2)], dtype=np.int)
tmesh = TriangleMesh(node, cell) # 三角形网格实例
print(node)
print(cell)
fig, axes = plt.subplots(1,1,figsize=(8,4))
tmesh.add_plot(axes[0], cellcolor = 'r')
#tmesh.find_node(axes[0], showindex = True, markersize = 25, fontsize = 12)
#tmesh.find_cell(axes[0], showindex = True, markersize = 25, fontsize = 12)
axes[0].set_title('mesh')
plt.show()
