SetFactory("OpenCASCADE");                  // 设置几何内核

dx = 0.03;                                  // 网格尺寸 dx
w = 0.3;                                    // 波导宽度 w
l = 0.5;                                    // 波导长度 l

// 几何组件(定义顶点)
Point(1) = {0, 0, 0, dx};
Point(2) = {l, 0, 0, dx};
Point(3) = {l, w, 0, dx};
Point(4) = {0, w, 0, dx};
// 几何组件(定义连线)
Line(1) = {4, 3};
Line(2) = {3, 2};
Line(3) = {2, 1};
Line(4) = {1, 4};
// 几何组件(定义平面)
Curve Loop(1) = {1, 2, 3, 4};
Surface(1) = {1};

// 物理组件
Physical Curve("PEC", 5) = {1, 2, 3, 4};
Physical Surface("air", 6) = {1};
