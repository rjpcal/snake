#include "defs.h"
#include "snake.h"

class Ground
{
public:
  int         NPatch;
  int         XPatch[ MAX_GABOR_NUMBER ];    /** int pos of patches **/
  int         YPatch[ MAX_GABOR_NUMBER ];
  Colorindex  *PPatch[ MAX_GABOR_NUMBER ];
  ELEMENT     array[ MAX_GABOR_NUMBER ];
  float       BACKG_AVE_SPACING;
};

extern Ground* theGround;

void MakeGround( void );
