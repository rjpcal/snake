#include "defs.h"
#include "snake.h"

#define MAX_GABOR_NUMBER    1800

class FakeWindow;
class GaborSet;

class Ground
{
public:
  Ground(const GaborSet& g);

  void renderInto(FakeWindow* wind) const;

  int flag(int i) const { return array[i].flag; }
  float xpos(int i) const { return array[i].xpos; }
  float ypos(int i) const { return array[i].ypos; }
  float theta(int i) const { return array[i].theta; }

  float backgAveSpacing() const { return BACKG_AVE_SPACING; }

private:
  int           NPatch;
  int           XPatch[ MAX_GABOR_NUMBER ];    /** int pos of patches **/
  int           YPatch[ MAX_GABOR_NUMBER ];
  const double* PPatch[ MAX_GABOR_NUMBER ];
  Element       array[ MAX_GABOR_NUMBER ];
  float         BACKG_AVE_SPACING;

  int tooClose(int upto, float x, float y, int except);
  void insideElements( int total_number, int foreg_number, int *ppatch_number );
  void contourElements();
  void gridElements();
  void fillElements();
  void jitterElement( void );
  void map2array(const GaborSet& g);
};
