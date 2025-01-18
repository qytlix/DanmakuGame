#ifndef CHARACTER
#define CHARACTER

#include "Real.h"
#include "../src/Point.cpp"

#include <SDL2/SDL.h>

class Character : Real
{
protected:
    // texture, box
    Uint16 health; // Every charactor can bleed >w<
    
public:
    Character(/* args */);
    ~Character();
};

Character::Character(/* args */)
{
}

Character::~Character()
{
}



#endif // CHARACTER
