
#include "defs.h"

class Ground;

class FakeWindow
{
public:
  Colorindex* data;
};

extern void ShowArray(const Ground* g, FakeWindow* wind);
extern void Window2Raster(const FakeWindow* wind);
