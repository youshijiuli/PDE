SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.1;                           // 网格尺寸 dx

w = 5;                              // 计算域宽度 w
h = 3;                              // 波导宽度 h
hm = 0.5;                           // 介质板厚度 hm
xm = 2;                             // 介质板位置 xm

// 几何组件(定义顶点)
Point(1) = {0, 0, 0, dx};
Point(2) = {w, 0, 0, dx};
Point(3) = {w, h, 0, dx};
Point(4) = {0, h, 0, dx};
Point(5) = {xm, 0, 0, dx};
Point(6) = {xm+hm, 0, 0, dx};
Point(7) = {xm, h, 0, dx};
Point(8) = {xm+hm, h, 0, dx};
// 几何组件(定义连线)
Line(1) = {4, 7};
Line(2) = {8, 3};
Line(3) = {1, 5};
Line(4) = {6, 2};
Line(5) = {7, 8};
Line(6) = {5, 6};
Line(7) = {4, 1};
Line(8) = {3, 2};
Line(9) = {7, 5};
Line(10) = {8, 6};
// 几何组件(定义平面)
Curve Loop(1) = {1, 9, 3, 7};
Plane Surface(1) = {1};
Curve Loop(2) = {2, 8, 4, 10};
Plane Surface(2) = {2};
Curve Loop(3) = {5, 10, 6, 9};
Plane Surface(3) = {3};

// 物理组件
Physical Curve("wall", 11) = {1, 3, 2, 4, 5, 6};
Physical Curve("in", 12) = {7};
Physical Curve("out", 13) = {8};
Physical Surface("air", 14) = {1, 2};
Physical Surface("media", 15) = {3};
