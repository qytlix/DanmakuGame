// 坐标系统、顺便加了一个转换
#if !defined(COORDINATESYSTEM)
#define COORDINATESYSTEM

#include "Point.cpp"
struct CoordinateSystem
{
    point origin;
    point e1,e2;
    point transform( point pos ) {return pos.x*e1 + pos.y*e2 + origin;}
    CoordinateSystem():origin({0.,0.}),e1({1.,0.}),e2({0.,1.}){}
};

#endif // COORDINATESYSTEM
