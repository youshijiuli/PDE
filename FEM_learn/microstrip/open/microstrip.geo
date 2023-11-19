SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.1;                           // 网格尺寸 dx

R = 1.2;                            // 半径 R
he = 0.2;                           // 介质板高度 he
wm = 0.5;                           // 微带宽度 wm
hm = 0.03;                          // 微带高度 hm

// 几何组件(定义顶点)
Point(1) = {0, 0, 0, dx};
Point(2) = {-R, 0, 0, dx};
Point(3) = {R, 0, 0, dx};
Point(4) = {-Sqrt(R*R-he*he), he, 0, dx};
Point(5) = {Sqrt(R*R-he*he), he, 0, dx};
Point(6) = {-wm/2, he, 0, dx};
Point(7) = {wm/2, he, 0, dx};
Point(8) = {-wm/2, he+hm, 0, dx};
Point(9) = {wm/2, he+hm, 0, dx};
// 几何组件(定义连线)
Circle(1) = {4, 1, 5};
Circle(2) = {2, 1, 4};
Circle(3) = {3, 1, 5};
Line(4) = {2, 1};
Line(5) = {3, 1};
Line(6) = {4, 6};
Line(7) = {5, 7};
Line(8) = {8, 9};
Line(9) = {6, 7};
Line(10) = {8, 6};
Line(11) = {9, 7};
// 几何组件(定义平面)
Curve Loop(1) = {1, 7, 11, 8, 10, 6};
Plane Surface(1) = {1};
Curve Loop(2) = {2, 6, 9, 7, 3, 5, 4};
Plane Surface(2) = {2};

// 物理组件
Physical Curve("open1", 12) = {1};
Physical Curve("open2", 13) = {2, 3};
Physical Curve("ground", 14) = {4, 5};
Physical Curve("microstrip", 15) = {8, 11, 9, 10};
Physical Surface("air", 16) = {1};
Physical Surface("media", 17) = {2};

// 加密网格
Transfinite Curve {8, 9} = 10 Using Progression 1;