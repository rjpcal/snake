
#include "defs.h"
#include "snake.h"

extern int         NPatch;

extern int         XPatch[ ],    /** int pos of patches **/
                   YPatch[ ];

extern Colorindex  *PPatch[ MAX_GABOR_NUMBER ];

extern ELEMENT     array[ MAX_GABOR_NUMBER ];
extern ELEMENT     pattern[ MAX_GABOR_NUMBER ];

extern float       BACKG_AVE_SPACING;


void MakeGround( void );
void ContourElements( int *pnpts );
void GridElements( int *pnpts );
void FillElements( int *pnpts );
void JitterElement( void );
int TooClose( int upto, float x, float y, int except );
void InsideElements( int total_number, int foreg_number, int *ppatch_number );
