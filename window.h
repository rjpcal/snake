#include "defs.h"

class FakeWindow
{
public:
  FakeWindow(int x, int y) :
    sizeX(x),
    sizeY(y),
    data(new double[sizeX*sizeY])
  {}

  ~FakeWindow()
  {
    delete [] data;
  }

  void clear(double col)
  {
    for (int i = 0; i < sizeX*sizeY; ++i)
      data[i] = col;
  }

  void writePnm(const char* fname) const;

  const int sizeX;
  const int sizeY;
  double* const data;
};
