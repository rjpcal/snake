#include <math.h>

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

  void blendVal(int x, int y, double val)
  {
    if (x >=0 && x < sizeX && y >= 0 && y < sizeY)
      if (fabs(data[x+y*sizeX]) < fabs(val))
        data[x+y*sizeX] = val;
  }

  void writePnm(const char* fname) const;

private:
  const int sizeX;
  const int sizeY;
  double* const data;
};
