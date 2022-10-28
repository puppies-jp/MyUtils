#include "Turtle.h"

class Painter
{
public:
    Turtle *turtle;
    Painter(Turtle *turtle)
    {

        this->turtle = turtle;
    }

    bool DrawCircle(int x, int y, int z)
    {

        turtle->PenDown();

        return true;
    }
};