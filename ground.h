#include "snake.h"

class FakeWindow;
class GaborSet;

class Ground
{
public:
  Ground(const Snake& s, int sizeX_, int sizeY_,
         double backgIniSpacing_,
         double backgMinSpacing_,
         FakeWindow* w, const GaborSet& g);

  void renderInto(FakeWindow* wind, const GaborSet& g) const;

  void writeArray(const char* filestem, int displayCount) const;

private:
  static const int MAX_GABOR_NUMBER = 1800;

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

  FakeWindow* window;
  const GaborSet& gabors;

  void dumpFrame() const;

  bool tryPush(const Element& e);
  bool tooClose(const Vector& v, int except);
  void insideElements();
  void hexGridElements();
  void fillElements();
  void jitterElement();
};
