#include "gabor.h"
#include "snake.h"

class Ground
{
public:
  Ground(double gaborPeriod, double gaborSigma, int gaborSize,
         int foregNumber, double foregSpacing,
         int sizeX_, int sizeY_,
         double backgIniSpacing_,
         double backgMinSpacing_);

  void writeArray(const char* filestem, int displayCount) const;

  void writePnm(const char* filename) const;

private:
  static const int MAX_GABOR_NUMBER = 1800;

  const Snake snake;
  const GaborSet gabors;
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

  void dumpFrame() const;

  bool tryPush(const Element& e);
  bool tooClose(const Vector& v, int except);
  void insideElements();
  void hexGridElements();
  void fillElements();
  void jitterElement();
};
