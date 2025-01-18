// 拥有碰撞箱的物品
#ifndef REAL
#define REAL

#include <vector>
#include "Object.h"
#include "../src/Box.cpp"
#include "../src/CoordinateSystem.cpp"

class Real : Object
{
protected:
    std::vector<Box *> box;

    // how to move
    /**
     * We use a function to location where is the character
     * You can stay still, move directly and so on
     */
    point (*pace)(Uint64 /* Here is the RELATIVE time */ );
    /**
     * To match with above one, we need an anchor
     */
    Uint64 time_anchor;
    /**
     * Use this add on the pace function to have the right position
     */
    CoordinateSystem coordinateSys;

public:
    Real(/* args */);
    ~Real();
    void loadPosition(); // change box, based on the "how to move"\
    just like move a step ^^
    point getPosition(); // read box
};

Real::Real(/* args */)
{
}

Real::~Real()
{
}



#endif // REAL
