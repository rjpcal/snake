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

struct Vector
{
  float x;
  float y;
};

extern void MakeForeg( void );
extern int GetElement( int n, float *x, float *y, float *theta );
extern void InitSnake( int length );
extern void EllipsoidSnake( int first, int length );
extern void Center_snake( int length );
extern void New_delta_theta( int length );
extern void Jiggle_snake( int length );
extern void Pick_nodes( int length, int ni[] );
extern void Swap( int *a, int *b );
extern void Get_sides( Vector no[], float a[] );
extern void Get_angles( Vector no[], float alpha[], float theta[] );
extern int Squash_quadrangle( Vector *no0, Vector *no1, Vector *no2, Vector *no3, Vector *new_no0, Vector *new_no1, Vector *new_no2, Vector *new_no3, float theta0, float incr );
extern int New_apex( Vector *no1, Vector *no2, Vector *no3, float b, float c );
extern int Monte_Carlo( float old_alpha[], float new_alpha[], float lo_alpha[], float hi_alpha[] );
extern void New_nodes( int length, int ni, int nj, Vector new_no_i, Vector new_no_j );

#endif // !SNAKE_H_DEFINED
