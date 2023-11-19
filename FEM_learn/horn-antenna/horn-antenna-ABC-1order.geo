SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.01;                          // 网格尺寸 dx

w = 0.03;                           // 波导宽度
l = 0.1;                            // 波导长度
rho = 0.1;                         // 喇叭天线
WA = 0.3;                           // ABC宽度
HA = 0.3;                           // ABC高度

// 几何组件(定义顶点)
Point(1) = {0, -w/2, 0, dx};
Point(2) = {0, w/2, 0, dx};
Point(3) = {l, -w/2, 0, dx};
Point(4) = {l, w/2, 0, dx};
Point(5) = {l+rho*Cos(Pi/6), w/2+rho*Sin(Pi/6), 0, dx};
Point(6) = {l+rho*Cos(Pi/6), -w/2-rho*Sin(Pi/6), 0, dx};
Point(7) = {0, HA/2, 0, dx};
Point(8) = {0, -HA/2, 0, dx};
Point(9) = {l+WA, HA/2, 0, dx};
Point(10) = {l+WA, -HA/2, 0, dx};
Point(11) = {l+WA, w/2, 0, dx};
Point(12) = {l+WA, -w/2, 0, dx};
// 几何组件(定义连线)
Line(1) = {2, 1};
Line(2) = {2, 4};
Line(3) = {1, 3};
Line(4) = {2, 7};
Line(5) = {1, 8};
Line(6) = {7, 9};
Line(7) = {8, 10};
Line(8) = {9, 11};
Line(9) = {10, 12};
Line(10) = {11, 12};
Line(11) = {4, 5};
Line(12) = {3, 6};
Line(13) = {5, 6};
// 几何组件(定义平面)
Curve Loop(1) = {1, 2, 11, 13, 12, 3};
Plane Surface(1) = {1};
Curve Loop(3) = {4, 6, 8, 10, 9, 7, 5, 3, 12, 13, 11, 2};
Plane Surface(2) = {3};

// 物理组件
Physical Surface("air1", 14) = {1};
Physical Surface("air2", 15) = {2};
Physical Curve("in", 16) = {1};
Physical Curve("PEC", 17) = {2, 3, 11, 12};
Physical Curve("ABC1", 18) = {4, 5, 6, 7, 8, 9, 10};
