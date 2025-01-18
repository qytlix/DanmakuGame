#ifndef PLAYER
#define PLAYER

#include "Character.h"

struct State
{
    bool direction[4]; // How to move this player -> change pace
    Uint16 lives; // How many lives are left

    // When process these, you should divice 10 for every data
    Uint16 booms[5 /*This number follow the "66d system"*/]; // how many booms are left
    Uint16 powers[21 /*This number follow the "66d system"*/]; // how many power does player have


};


class Player : Character
{
private:
    
public:
    Player(/* args */);
    ~Player();
};

Player::Player(/* args */)
{
}

Player::~Player()
{
}


#endif // PLAYER
