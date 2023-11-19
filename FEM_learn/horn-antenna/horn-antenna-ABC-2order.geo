SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.005;                          // 网格尺寸 dx

w = 0.03;                           // 波导宽度
l = 0.1;                            // 波导长度
rho = 0.1;                         // 喇叭天线
WA = 0.3;                           // ABC宽度
HA = 0.3;                           // ABC高度
R = 0.03;                           // 圆角半径

// 几何组件(定义顶点)
Point(1) = {0, -w/2, 0, dx};
Point(2) = {0, w/2, 0, dx};
Point(3) = {l, -w/2, 0, dx};
Point(4) = {l, w/2, 0, dx};
Point(5) = {l+rho*Cos(Pi/24), w/2+rho*Sin(Pi/24), 0, dx};
Point(6) = {l+rho*Cos(Pi/24), -w/2-rho*Sin(Pi/24), 0, dx};
Point(7) = {l+WA, w/2, 0, dx};
Point(8) = {l+WA, -w/2, 0, dx};
Point(9) = {R, HA/2-R, 0, dx};
Point(10) = {R, HA/2, 0, dx};
Point(11) = {0, HA/2-R, 0,dx};
Point(12) = {l+WA-R, HA/2-R, 0, dx};
Point(13) = {l+WA-R, HA/2, 0, dx};
Point(14) = {l+WA, HA/2-R, 0, dx};
Point(15) = {R, -HA/2+R, 0, dx};
Point(16) = {R, -HA/2, 0, dx};
Point(17) = {0, -HA/2+R, 0, dx};
Point(18) = {l+WA-R, -HA/2+R, 0, dx};
Point(19) = {l+WA-R, -HA/2, 0, dx};
Point(20) = {l+WA, -HA/2+R, 0, dx};
// 几何组件(定义连线)
Line(1) = {2, 1};
Line(2) = {2, 4};
Line(3) = {1, 3};
Line(4) = {4, 5};
Line(5) = {3, 6};
Line(6) = {5, 6};
Line(7) = {7, 8};
Line(8) = {11, 2};
Line(9) = {10, 13};
Line(10) = {14, 7};
Line(11) = {1, 17};
Line(12) = {8, 20};
Line(13) = {16, 19};
Circle(14) = {11, 9, 10};
Circle(15) = {13, 12, 14};
Circle(16) = {17, 15, 16};
Circle(17) = {20, 18, 19};
// 几何组件(定义平面)
Curve Loop(1) = {1, 2, 4, 6, 5, 3};
Plane Surface(1) = {1};
Curve Loop(3) = {9, 15, 10, 7, 12, 17, 13, 16, 11, 3, 5, 6, 4, 2, 8, 14};
Plane Surface(2) = {3};

// 物理组件
Physical Surface("air1", 18) = {1};
Physical Surface("air2", 19) = {2};
Physical Curve("in", 20) = {1};
Physical Curve("ABC1", 21) = {7};
Physical Curve("ABC2", 22) = {8, 9, 10, 12, 13, 11};
Physical Curve("ABC3", 23) = {14, 15, 16, 17};
Physical Curve("PEC", 24) = {2, 4, 3, 5};
