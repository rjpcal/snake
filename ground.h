#include "snake.h"

#define MAX_GABOR_NUMBER    1800

class FakeWindow;
class GaborSet;

class Ground
{
public:
  Ground(const Snake& s, int sizeX_, int sizeY_,
         double backgIniSpacing_,
         double backgMinSpacing_);

  void renderInto(FakeWindow* wind, const GaborSet& g) const;

  void writeArray(const char* filestem, int displayCount) const;

private:
  const Snake& snake;
  const int sizeX;
  const int sizeY;
  const double halfX;
  const double halfY;
  const double backgIniSpacing;
  const double backgMinSpacing;
  const double backgMinSpacingSqr;
  int insideNumber;
  int totalNumber;
  Element array[ MAX_GABOR_NUMBER ];
  double backgAveSpacing;

  int tooClose(int upto, double x, double y, int except);
  void insideElements();
  void contourElements();
  void gridElements();
  void fillElements();
  void jitterElement();
};
