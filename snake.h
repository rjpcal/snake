#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

struct Element
{
  int   flag;   /* 2 contour, 1 inside, 0 outside */
  float theta;  /* angle to x-axis, 0 to TWOPI */
  float xpos;   /* x-position of node */
  float ypos;   /* y-position of node */
  float delta;  /* difference of thetas */
};

extern void MakeForeg( void );
extern int GetElement( int n, float *x, float *y, float *theta );

#endif // !SNAKE_H_DEFINED
