// 碰撞箱的定义
#ifndef BOX_CPP
#define BOX_CPP

#include "Point.cpp"
#include <SDL2/SDL.h>
struct Box
{
    point pos;// if i should transform this pos to centrePos??
    Uint32 height, width;
};

bool checkCross(Box, Box);

bool checkCross(Box A,Box B) {

}

SDL_Rect BoxTransform(Box);

SDL_Rect BoxTransform(Box a) {
    SDL_Rect rect;
    rect.x=a.pos.x;
    rect.y=a.pos.y;
    rect.h=a.height;
    rect.w=a.width;
}

#endif