#include "defs.h"
#include "snake.h"

#define MAX_GABOR_NUMBER    1800

class FakeWindow;
class GaborSet;
class Snake;

class Ground
{
public:
  Ground(const Snake& s);

  void renderInto(FakeWindow* wind, const GaborSet& g) const;

  int flag(int i) const { return array[i].flag; }
  float xpos(int i) const { return array[i].xpos; }
  float ypos(int i) const { return array[i].ypos; }
  float theta(int i) const { return array[i].theta; }

  float backgAveSpacing() const { return BACKG_AVE_SPACING; }

  int totalNumber() const { return NPatch; }

private:
  int           NPatch;
  Element       array[ MAX_GABOR_NUMBER ];
  float         BACKG_AVE_SPACING;

  int tooClose(int upto, float x, float y, int except);
  void insideElements();
  void contourElements(const Snake& s);
  void gridElements();
  void fillElements();
  void jitterElement();
};
