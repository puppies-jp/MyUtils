
#ifndef TURTLE
#define TURTLE

class Turtle
{
public:
  virtual ~Turtle() {}
  virtual void PenUp() = 0;
  virtual int PenDown() = 0;
  virtual void Forward(int distance) = 0;
  virtual void Turn(int degrees) = 0;
  virtual void GoTo(int x, int y) = 0;
  virtual int GetX() const = 0;
  virtual int GetY() const = 0;
};

#endif