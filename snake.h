#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

#define MAX_FOREG_NUMBER     128

struct Element
{
  int   flag;   /* 2 contour, 1 inside, 0 outside */
  float theta;  /* angle to x-axis, 0 to TWOPI */
  float xpos;   /* x-position of node */
  float ypos;   /* y-position of node */
  float delta;  /* difference of thetas */
};

class Snake
{
public:
  Snake();

  int getElement(int n, float* x, float* y, float* theta) const;

  Element foreg[ MAX_FOREG_NUMBER ];
};

#endif // !SNAKE_H_DEFINED
