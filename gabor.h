
#include "defs.h"

#define GABOR_MAX_ORIENT      64
#define GABOR_MAX_PHASE        8

class GaborSet
{
public:
  GaborSet(double period, double sigma, int size);
  ~GaborSet();

  const double* getPatch(float theta, float phi) const;

private:
  double* Patch[GABOR_MAX_ORIENT][GABOR_MAX_PHASE];
};
