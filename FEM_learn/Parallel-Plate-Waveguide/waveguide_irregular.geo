SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.25;                           // 网格尺寸 dx

w = 5;                              // 计算域宽度 w
h = 3;                              // 波导宽度 h

// 几何组件(定义顶点)
Point(1) = {0, 0, 0, dx};
Point(2) = {w, 0, 0, dx};
Point(3) = {w, h, 0, dx};
Point(4) = {0, h, 0, dx};
Point(5) = {1.6, 1.7, 0, dx};
Point(6) = {2.9, 2, 0, dx};
Point(7) = {3.6, 1.4, 0, dx};
Point(8) = {2.8, 0.9, 0, dx};
Point(9) = {2.1, 1.1, 0, dx};
// 几何组件(定义连线)
Line(1) = {1, 2};
Line(2) = {2, 3};
Line(3) = {3, 4};
Line(4) = {4, 1};
Spline(5) = {5, 6, 7, 8, 9, 5};
// 几何组件(定义平面)
Curve Loop(1) = {3, 4, 1, 2};
Curve Loop(2) = {5};
Plane Surface(1) = {1, 2};
Curve Loop(3) = {5};
Plane Surface(2) = {3};

// 物理组件
Physical Curve("wall", 6) = {3, 1};
Physical Curve("in", 7) = {4};
Physical Curve("out", 8) = {2};
Physical Surface("air", 9) = {1};
Physical Surface("media", 10) = {2};

