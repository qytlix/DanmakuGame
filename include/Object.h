#ifndef OBJECT
#define OBJECT

#include <vector>
#include <SDL2/SDL.h>


class Object
{
protected:
    std::vector<SDL_Texture *> texture;
public:
    Object();
    ~Object();
    void paint();
};

Object::Object()
{
}

Object::~Object()
{
}



#endif // OBJECT
