#include "defs.h"
#include "snake.h"

#define MAX_GABOR_NUMBER    1800

class FakeWindow;
class GaborSet;

class Ground
{
public:
  Ground(const Snake& s, int sizeX_, int sizeY_,
         float backgIniSpacing_,
         float backgMinSpacing_);

  void renderInto(FakeWindow* wind, const GaborSet& g) const;

  int flag(int i) const { return array[i].flag; }
  float xpos(int i) const { return array[i].xpos; }
  float ypos(int i) const { return array[i].ypos; }
  float theta(int i) const { return array[i].theta; }

  float backgAveSpacing() const { return BACKG_AVE_SPACING; }

  int totalNumber() const { return NPatch; }
  int getForegNumber() const { return snake.getLength(); }
  float getForegSpacing() const { return snake.getSpacing(); }
  int getPatchNumber() const { return patchNumber; }
  int getBackgNumber() const { return backgNumber; }
  float getBackgIniSpacing() const { return backgIniSpacing; }
  float getBackgMinSpacing() const { return backgIniSpacing; }

private:
  const Snake& snake;
  const int sizeX;
  const int sizeY;
  const float halfX;
  const float halfY;
  const float backgIniSpacing;
  const float backgMinSpacing;
  const float backgMinSpacingSqr;
  int patchNumber;
  int backgNumber;
  int           NPatch;
  Element       array[ MAX_GABOR_NUMBER ];
  float         BACKG_AVE_SPACING;

  int tooClose(int upto, float x, float y, int except);
  void insideElements();
  void contourElements();
  void gridElements();
  void fillElements();
  void jitterElement();
};
