const int GABOR_MAX_ORIENT = 64;
const int GABOR_MAX_PHASE = 8;

class GaborSet
{
public:
  GaborSet(double period, double sigma, int size);
  ~GaborSet();

  int getPatchSize() const { return patchSize; }
  const double* getPatch(double theta, double phi) const;

private:
  const int patchSize;
  double* Patch[GABOR_MAX_ORIENT][GABOR_MAX_PHASE];
};
