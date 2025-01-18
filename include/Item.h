#ifndef ITEM
#define ITEM

#include "Real.h"

class Item : Real
{
protected:
    Uint16 ID; // the type of these items
public:
    Item(/* args */);
    ~Item();
};

Item::Item(/* args */)
{
    // What is the special ?\
    It's pace was fixed to "directly drop", \
    and have a chance to change to "follow the player"\
    The only diference between these items is\
    Texture\
    Effect
    
    // like this -> pace = directlyDrop;
}

Item::~Item()
{
}


#endif // ITEM
