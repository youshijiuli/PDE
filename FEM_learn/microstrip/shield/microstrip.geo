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
Line(2) = {5, 6};
Line(3) = {1, 5};
Line(4) = {2, 6};
Line(5) = {3, 5};
Line(6) = {5, 7};
Line(7) = {7, 9};
Line(8) = {9, 10};
Line(9) = {10, 8};
Line(10) = {8, 6};
Line(11) = {6, 4};
Line(12) = {4, 3};
Line(13) = {7, 8};
// 几何组件(定义平面)
Curve Loop(1) = {8, 9, 10, 11, 12, 5, 6, 7};
Plane Surface(1) = {1};
Curve Loop(2) = {3, 1, 4, 10, 13, 6};
Plane Surface(2) = {2};

// 物理组件
Physical Curve("microstrip", 14) = {7, 8, 9, 13};
Physical Curve("shield", 15) = {12, 11, 4, 1, 3, 5};
Physical Surface("air", 16) = {1};
Physical Surface("media", 17) = {2};

// 微带附近加密网格
Transfinite Curve {7, 9} = 3 Using Progression 1;
Transfinite Curve {8, 13} = 20 Using Progression 1;