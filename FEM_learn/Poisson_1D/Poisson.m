close all; clear all; clc

%% 前处理
N = 21;                                     % 节点总数
X = 0:1/20:1;                               % 位置坐标

%% 刚度 K 矩阵计算
K(1, 1) = 1/(X(2)-X(1));
K(N, N) = 1/(X(N)-X(N-1));
for i = 2:N-1
    K(i, i) = 1/(X(i)-X(i-1)) + 1/(X(i+1)-X(i));
end
for i = 1:N-1
    K(i+1, i) = -1/(X(i+1)-X(i));
    K(i, i+1) = K(i+1, i);
end

%% 矢量 b 计算
fun = @(x) ((16*pi^2-4).*sin(4*pi.*x)-16*pi.*cos(4*pi.*x)).*exp(2.*x);
b(1) = integral(@(x)fun(x).*(X(2)-x)/(X(2)-X(1)), X(1), X(2));
b(N) = integral(@(x)fun(x).*(x-X(N-1))/(X(N)-X(N-1)), X(N-1), X(N));
for i = 2:N-1
    b(i) = integral(@(x)fun(x).*(x-X(i-1))/(X(i)-X(i-1)), X(i-1), X(i))...,
        + integral(@(x)fun(x).*(X(i+1)-x)/(X(i+1)-X(i)), X(i), X(i+1));
end

%% Dirichlet 边界条件
K(1, 1) = 1; K(1, 2:N) = 0; b(1) = 0;
K(2:N, 1) = 0; b(2:N) = b(2:N) - K(2:N, 1)'*0;
K(N, N) = 1;  K(N, 1:N-1) = 0; b(N) = 0;
K(1:N-1, N) = 0; b(1:N-1) = b(1:N-1) - K(N, 1:N-1)*0;
% %% Neumann 边界条件
% K(N, N) = K(N, N)+0; b(N) = b(N)+0;

%% 矩阵方程求解
phi = K\b.';

%% 解析解
x = 0:1/60:1;
phi_exact = exp(2.*x).*sin(4*pi*x);

%% 后处理
plot(X, phi, 'o');
hold on
plot(x, phi_exact);
xlabel('x'); ylabel('y');
legend('FEM solve', 'exact solve')