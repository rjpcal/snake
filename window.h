#include "defs.h"

class FakeWindow
{
public:
  FakeWindow(int x, int y) :
    sizeX(x),
    sizeY(y),
    data(new Colorindex[sizeX*sizeY])
  {}

  ~FakeWindow()
  {
    delete [] data;
  }

  void clear(int col)
  {
    for (int i = 0; i < sizeX*sizeY; ++i)
      data[i] = col;
  }

  void writeRaster(const char* fname) const;

  const int sizeX;
  const int sizeY;
  Colorindex* const data;
};
