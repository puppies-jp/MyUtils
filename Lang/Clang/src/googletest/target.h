#include "Turtle.h"

class Painter
{
public:
    Turtle *turtle;
    Painter(Turtle *turtle)
    {

        this->turtle = turtle;
    }

    int DrawCircle(int x, int y, int z)
    {
        return turtle->PenDown();
    }
};