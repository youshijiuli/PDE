SetFactory("OpenCASCADE");          // 设置几何内核

dx = 0.1;                           // 网格尺寸 dx

w = 2.0;                            // 区域宽度 w
h = 1.0;                            // 区域高度 h
he = 0.2;                           // 介质板高度 he
wm = 0.5;                           // 微带宽度 wm
hm = 0.03;                          // 微带高度 hm

// 几何组件(定义顶点)
Point(1) = {0.0, 0.0, 0.0, dx};
Point(2) = {w, 0.0, 0.0, dx};
Point(3) = {0.0, h, 0.0, dx};
Point(4) = {w, h, 0.0, dx};
Point(5) = {0.0, he, 0.0, dx};
Point(6) = {w, he, 0.0, dx};
Point(7) = {-wm/2+w/2, he, 0.0, dx};
Point(8) = {wm/2+w/2, he, 0.0, dx};
Point(9) = {-wm/2+w/2, he+hm, 0.0, dx};
Point(10) = {wm/2+w/2, he+hm, 0.0, dx};
// 几何组件(定义连线)
Line(1) = {1, 2};
Line(2) = {1, 5};
Line(3) = {2, 6};
Line(4) = {3, 5};
Line(5) = {7, 9};
Line(6) = {9, 10};
Line(7) = {10, 8};
Line(8) = {6, 4};
Line(9) = {4, 3};
Line(10) = {7, 8};
// 几何组件(定义平面)
Curve Loop(1) = {9, 4, -2, 1, 3, 8};
Curve Loop(2) = {6, 7, -10, 5};
Plane Surface(1) = {1, 2};

// 物理组件
Physical Curve("shield", 11) = {9, 8, 3, 1, 2, 4};
Physical Curve("microstrip", 12) = {6, 7, 10, 5};
Physical Surface("air", 13) = {1};

// 加密网格
Transfinite Curve {6, 10} = 10 Using Progression 1;