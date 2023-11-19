SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.1;                           // 网格尺寸 dx
w = 2.0;                            // 矩形宽度 w
h = 1.0;                            // 矩形高度 h

// 定义四个顶点
Point(1) = {0.0, 0.0, 0.0, dx};
Point(2) = {w, 0.0, 0.0, dx};
Point(3) = {w, h, 0.0, dx};
Point(4) = {0.0, h, 0.0 ,dx};
// 定义四条边
Line(1) = {1, 2};
Line(4) = {2, 3};
Line(2) = {3, 4};
Line(3) = {4, 1};
// 定义平面
Curve Loop(2) = {2, 3, 1, 4};
Plane Surface(1) = {2};

// 物理组件
Physical Curve("left", 5) = {3};
Physical Curve("right", 6) = {4};
Physical Curve("bottom", 7) = {1};
Physical Curve("top", 8) = {2};
Physical Surface("block", 9) = {1};

// 加密网格
// Transfinite Curve {3, 4} = 31 Using Progression 1;
// Transfinite Curve {2, 1} = 61 Using Progression 1;
