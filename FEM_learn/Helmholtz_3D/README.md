# 三维 Helmholtz 方程有限元分析

[TOC]

## 边值问题

$$
\left\{\begin{aligned}
    &-\frac{\partial}{\partial x}\left(\alpha_x\frac{\partial\varphi}{\partial x}\right)-\frac{\partial}{\partial y}\left(\alpha_y\frac{\partial \varphi}{\partial y}\right)-\frac{\partial}{\partial z}\left(\alpha_z\frac{\partial \varphi}{\partial z}\right)+\beta\varphi=f &(x,y,z)\in V\\
    &\varphi=p &(x,y,z)\in S_1\\
    &\left(\alpha_x\frac{\partial \varphi}{\partial x}\hat{x}+\alpha_y\frac{\partial \varphi}{\partial y}\hat{y}+\alpha_z\frac{\partial \varphi}{\partial z}\hat{z}\right)\cdot\hat{n}+\gamma\varphi=q &(x,y,z)\in S_2
\end{aligned}\right.
$$

## Ritz 有限元方法

微分方程问题等价的泛函极值问题为

$$
\begin{aligned}
    &\begin{cases}
        \delta F(\varphi)=0\\
        \varphi|_{S_1}=p
    \end{cases}\\
    F(\varphi)=&\frac{1}{2}\int_V\left[\alpha_x\left(\frac{\partial\varphi}{\partial x}\right)^2+\alpha_y\left(\frac{\partial\varphi}{\partial y}\right)^2+\alpha_z\left(\frac{\partial\varphi}{\partial z}\right)^2+\beta\varphi^2\right]\mathrm{d}V\\
    &+\int_{S_{2}}\left(\frac{\gamma}{2}\varphi^2-q\varphi\right)\mathrm{d}S-\int_Vf\varphi\mathrm{d}V
\end{aligned}
$$

**证明**

对泛函 $F$ 取第一变分

$$
\begin{aligned}
    \delta F=&\int_V\left(\alpha_x\frac{\partial\varphi}{\partial x}\frac{\partial\delta\varphi}{\partial x}+\alpha_y\frac{\partial\varphi}{\partial y}\frac{\partial\delta\varphi}{\partial y}+\alpha_z\frac{\partial\varphi}{\partial z}\frac{\partial\delta\varphi}{\partial z}+\beta\varphi\delta\varphi\right)\mathrm{d}V\\
    &+\int_{S_2}(\gamma\varphi\delta\varphi-q\delta\varphi)\mathrm{d}S-\int_Vf\delta\varphi\mathrm{d}V\\
    =&\int_V\left[\frac{\partial}{\partial x}\left(\alpha_x\frac{\partial\varphi}{\partial x}\delta\varphi\right)-\frac{\partial}{\partial x}\left(\alpha_x\frac{\partial\varphi}{\partial x}\right)\delta\varphi+\frac{\partial}{\partial y}\left(\alpha_y\frac{\partial\varphi}{\partial y}\delta\varphi\right)-\frac{\partial}{\partial y}\left(\alpha_y\frac{\partial\varphi}{\partial y}\right)\delta\varphi\right.\\
    &+\left.\frac{\partial}{\partial z}\left(\alpha_z\frac{\partial\varphi}{\partial z}\delta\varphi\right)-\frac{\partial}{\partial z}\left(\alpha_z\frac{\partial\varphi}{\partial z}\right)\delta\varphi+\beta\varphi\delta\varphi\right]\mathrm{d}V\\
    &+\int_{S_2}(\gamma\varphi-q)\delta\varphi\mathrm{d}S-\int_Vf\delta\varphi\mathrm{d}V\\
    =&\int_V\left[-\frac{\partial}{\partial x}\left(\alpha_x\frac{\partial\varphi}{\partial x}\right)-\frac{\partial}{\partial y}\left(\alpha_y\frac{\partial\varphi}{\partial y}\right)-\frac{\partial}{\partial z}\left(\alpha_z\frac{\partial\varphi}{\partial z}\right)+\beta\varphi-f\right]\delta\varphi\mathrm{d}V\\
    &+\oint_S\left[\left(\alpha_x\frac{\partial\varphi}{\partial x}\hat{x}+\alpha_y\frac{\partial\varphi}{\partial y}\hat{y}+\alpha_z\frac{\partial\varphi}{\partial z}\hat{z}\right)\cdot\hat{n}+\gamma\varphi-q\right]\delta\varphi\mathrm{d}S
\end{aligned}
$$

为了使任意变分 $\delta\varphi$ 都能使泛函极值成立，要求

$$
\left\{\begin{aligned}
    &-\frac{\partial}{\partial x}\left(\alpha_x\frac{\partial\varphi}{\partial x}\right)-\frac{\partial}{\partial y}\left(\alpha_y\frac{\partial\varphi}{\partial y}\right)-\frac{\partial}{\partial z}\left(\alpha_z\frac{\partial\varphi}{\partial z}\right)+\beta\varphi-f=0 &(x,y,z)\in V\\
    &\left(\alpha_x\frac{\partial\varphi}{\partial x}\hat{x}+\alpha_y\frac{\partial\varphi}{\partial y}\hat{y}+\alpha_z\frac{\partial\varphi}{\partial z}\hat{z}\right)\cdot\hat{n}+\gamma\varphi-q=0 &(x,y,z)\in S_2
\end{aligned}\right.
$$

强加 Dirichlet 边界条件 $\varphi|_{S_1}=p$ 后，变分问题与微分方程等价。