SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.1;                           // 网格尺寸 dx

w = 5;                              // 计算域宽度 w
h = 3;                              // 波导宽度 h
R = 0.5;                            // 介质半径 R

// 几何组件(定义顶点)
Point(1) = {0, 0, 0, dx};
Point(2) = {w, 0, 0, dx};
Point(3) = {w, h, 0, dx};
Point(4) = {0, h, 0, dx};
// 几何组件(定义连线)
Line(1) = {4, 3};
Line(2) = {3, 2};
Line(3) = {2, 1};
Line(4) = {1, 4};
Circle(5) = {w/2, h/2, 0, R, 0, 2*Pi};
// 几何组件(定义平面)
Curve Loop(1) = {1, 2, 3, 4};
Curve Loop(2) = {5};
Plane Surface(1) = {1, 2};
Curve Loop(3) = {5};
Plane Surface(2) = {3};

// 物理组件
Physical Curve("wall", 6) = {1, 3};
Physical Curve("in", 7) = {4};
Physical Curve("out", 8) = {2};
Physical Surface("air", 9) = {1};
Physical Surface("media", 10) = {2};

// 加密网格
Transfinite Curve {5, 5} = 30 Using Progression 1;
