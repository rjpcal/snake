#include "gabor.h"

#include "geom.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

namespace
{
  const double DELTA_THETA    = M_PI / GABOR_MAX_ORIENT;
  const double DELTA_PHASE    = 2 * M_PI / GABOR_MAX_PHASE;
  const double OFF_THETA      = 0.5 * DELTA_THETA;
  const double OFF_PHASE      = 0.5 * DELTA_PHASE;

  double* CreatePatch(double sigma, double omega, double theta,
                      double phi, double contrast, int xysize)
  {
    double* const result = new double[xysize*xysize];

    const double ssqr  = 2.*sigma*sigma;

    const double cos_theta = cos(theta);
    const double sin_theta = sin(theta);

    double* ptr = result;

    for (int iy=0; iy<xysize; ++iy)
      {
        const double fy = iy - xysize / 2.0 + 0.5;

        for (int ix=0; ix<xysize; ++ix)
          {
            const double fx = ix - xysize / 2.0 + 0.5;

            const double dx = cos_theta * fx - sin_theta * fy;
            const double dy = sin_theta * fx + cos_theta * fy;

            const double dsqr  = (dx*dx + dy*dy) / ssqr;

            const double sinus = cos(omega * dx + phi);

            const double gauss = exp(-dsqr);
            *ptr++ = contrast * sinus * gauss;
          }
      }

    return result;
  }

  int Theta2Index(double theta)
  {
    theta = Zerotopi(theta);

    int index = int((theta + OFF_THETA) / DELTA_THETA);

    return index % GABOR_MAX_ORIENT;
  }

  int Phi2Index(double phi)
  {
    phi   = Zerototwopi(phi);

    int index = int((phi + OFF_PHASE) / DELTA_PHASE);

    return index % GABOR_MAX_PHASE;
  }
}

GaborSet::GaborSet(double period, double sigma, int size) :
  patchSize(size)
{
  const double omega = 2 * M_PI / period;

  for (int n=0; n<GABOR_MAX_ORIENT; ++n)
    for (int m=0; m<GABOR_MAX_PHASE; ++m)
      {
        Patch[n][m] = CreatePatch(sigma,
                                  omega,
                                  n * DELTA_THETA,
                                  m * DELTA_PHASE,
                                  /* contrast */ 1.0,
                                  size);
      }

  printf(" Gabors allocated\n");
  fflush(stdout);
}

GaborSet::~GaborSet()
{
  for (int i=0; i<GABOR_MAX_ORIENT; ++i)
    for (int j=0; j<GABOR_MAX_PHASE; ++j)
      delete [] Patch[i][j];

  printf(" Gabors de-allocated\n");
  fflush(stdout);
}

const double* GaborSet::getPatch(double theta, double phi) const
{
  const int itheta = Theta2Index(theta);
  const int iphi   = Phi2Index(phi);

  return Patch[itheta][iphi];
}
