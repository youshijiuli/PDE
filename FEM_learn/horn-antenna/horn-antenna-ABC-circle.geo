SetFactory("OpenCASCADE");              // 设置几何内核

dx = 0.01;                              // 网格尺寸 dx

w = 0.03;                               // 波导宽度
l = 0.2;                                // 波导长度
rho = 0.15;                             // 喇叭天线
R = 0.3;                                // ABC 半径
xcenter = Sqrt(R*R-(w/2)*(w/2));        // ABC 中心坐标

// 几何组件(定义顶点)
Point(1) = {0, -w/2, 0, dx};
Point(2) = {0, w/2, 0, dx};
Point(3) = {l, -w/2, 0, dx};
Point(4) = {l, w/2, 0, dx};
Point(5) = {l+rho*Cos(Pi/6), w/2+rho*Sin(Pi/6), 0, dx};
Point(6) = {l+rho*Cos(Pi/6), -w/2-rho*Sin(Pi/6), 0, dx};
Point(7) = {xcenter, 0, 0, dx};
Point(8) = {xcenter+R, 0, 0, dx};
// 几何组件(定义连线)
Line(1) = {2, 4};
Line(2) = {4, 5};
Line(3) = {1, 3};
Line(4) = {3, 6};
Line(5) = {2, 1};
Line(6) = {5, 6};
Circle(7) = {2, 7, 8};
Circle(8) = {1, 7, 8};
// 几何组件(定义平面)
Curve Loop(1) = {1, 2, 6, 4, 3, 5};
Plane Surface(1) = {1};
Curve Loop(2) = {7, 8, 3, 4, 6, 2, 1};
Plane Surface(2) = {2};

// 物理组件
Physical Surface("air1", 9) = {1};
Physical Surface("air2", 10) = {2};
Physical Curve("out", 11) = {7, 8};
Physical Curve("in", 12) = {5};
Physical Curve("PEC", 13) = {1, 2, 3, 4};
