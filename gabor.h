
#include "defs.h"

#define GABOR_MAX_ORIENT      64
#define GABOR_MAX_PHASE        8

class GaborSet
{
public:
  GaborSet();
  ~GaborSet();

  Colorindex* getPatch(float theta, float phi ) const;

private:
  Colorindex* Patch[GABOR_MAX_ORIENT][GABOR_MAX_PHASE];
};
