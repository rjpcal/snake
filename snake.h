#ifndef SNAKE_H_DEFINED
#define SNAKE_H_DEFINED

typedef struct Element
{
    int   flag;   /* 2 contour, 1 inside, 0 outside */
    float theta;  /* angle to x-axis, 0 to TWOPI */
    float xpos;   /* x-position of node */
    float ypos;   /* y-position of node */
    float delta;  /* difference of thetas */
} ELEMENT;

typedef struct Vector
{
    float x;
    float y;
} VECTOR;

extern void MakeForeg( void );
extern int GetElement( int n, float *x, float *y, float *theta );
extern void InitSnake( int length );
extern void EllipsoidSnake( int first, int length );
extern void Center_snake( int length );
extern void New_delta_theta( int length );
extern void Jiggle_snake( int length );
extern void Pick_nodes( int length, int ni[] );
extern void Swap( int *a, int *b );
extern void Get_sides( VECTOR no[], float a[] );
extern void Get_angles( VECTOR no[], float alpha[], float theta[] );
extern int Squash_quadrangle( VECTOR *no0, VECTOR *no1, VECTOR *no2, VECTOR *no3, VECTOR *new_no0, VECTOR *new_no1, VECTOR *new_no2, VECTOR *new_no3, float theta0, float incr );
extern int New_apex( VECTOR *no1, VECTOR *no2, VECTOR *no3, float b, float c );
extern int Monte_Carlo( float old_alpha[], float new_alpha[], float lo_alpha[], float hi_alpha[] );
extern void New_nodes( int length, int ni, int nj, VECTOR new_no_i, VECTOR new_no_j );

#endif // !SNAKE_H_DEFINED
