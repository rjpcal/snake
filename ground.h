#include "defs.h"
#include "snake.h"

#define MAX_GABOR_NUMBER    1800

class Ground
{
public:
  int         NPatch;
  int         XPatch[ MAX_GABOR_NUMBER ];    /** int pos of patches **/
  int         YPatch[ MAX_GABOR_NUMBER ];
  Colorindex  *PPatch[ MAX_GABOR_NUMBER ];
  Element     array[ MAX_GABOR_NUMBER ];
  float       BACKG_AVE_SPACING;

  static Ground* make();

private:
  int tooClose(int upto, float x, float y, int except);
  void insideElements( int total_number, int foreg_number, int *ppatch_number );
  void contourElements( int *pnpts );
  void gridElements( int *pnpts );
  void fillElements( int *pnpts );
  void jitterElement( void );
  void map2array(int npts);
};
