#include "defs.h"
#include "snake.h"

extern int         NPatch;

extern int         XPatch[ ];    /** int pos of patches **/
extern int         YPatch[ ];

extern Colorindex  *PPatch[ MAX_GABOR_NUMBER ];

extern ELEMENT     array[ MAX_GABOR_NUMBER ];

extern float       BACKG_AVE_SPACING;

void MakeGround( void );
