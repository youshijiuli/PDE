import numpy as np 
from scipy import linalg 
import matplotlib.pyplot as plt 

#形成均匀网格剖分
a=0
b=1
N=np.array([6,11,21,41,81])
for t in N:
    i=(b-a)/(t-1)
    x=np.linspace(a,b,t)

#形成系数矩阵A
    c1=-1/(i**2)
    c2=2/(i**2)
    g=np.hstack((c1*np.ones(t-2),0))
    c=np.hstack((0,c1*np.ones(t-2)))
    d=np.hstack((1,c2*np.ones(t-2),1))
    A=np.diag(g,-1)+np.diag(d)+np.diag(c,1)

#形成右端项f
    def u(x):
        u=np.sin(4*np.pi*x)
        return u
    def f1(x):
        f1=16*np.pi**2*np.sin(4*np.pi*x)
        return f1
    f=f1(x)
    f[0]=u(x[0])
    f[t-1]=u(x[t-1])

#解线性方程组
    U=linalg.solve(A,f)
    print(U)

#画图可视化
    plt.plot(x,u,label="exact",color="red")
    plt.xlabel("x")
    plt.ylabel("u")
    plt.title("The solution plot")
    plt.legend()
    plt.show()
