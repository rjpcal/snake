
#include "gabor.h"

#include "gamma.h"
#include "geom.h"
#include "params.h"

#include <stdlib.h>

namespace
{
  const float GABOR_CONTRAST = 1.0f;

  const float DELTA_THETA    = M_PI / GABOR_MAX_ORIENT;
  const float DELTA_PHASE    = TWOPI / GABOR_MAX_PHASE;
  const float OFF_THETA      = 0.5 * DELTA_THETA;
  const float OFF_PHASE      = 0.5 * DELTA_PHASE;

  Colorindex* CreatePatch(float sigma, float omega, float theta,
                          float phi, float contrast, int xysize )
  {
    Colorindex* const result = new Colorindex[xysize*xysize];

    const float ssqr  = 2.*sigma*sigma;

    const float cos_theta = cos((double) theta );
    const float sin_theta = sin((double) theta );

    Colorindex* ptr = result;

    for(int iy=0; iy<xysize; ++iy)
      {
        const float fy = (float)( iy - xysize / 2.0 + 0.5 );

        for(int ix=0; ix<xysize; ++ix)
          {
            const float fx = (float)( ix - xysize / 2.0 + 0.5 );

            const float dx = cos_theta * fx - sin_theta * fy;
            const float dy = sin_theta * fx + cos_theta * fy;

            const float dsqr  = ( dx*dx + dy*dy ) / ssqr;

            const float sinus = cos((double) omega * dx + phi );

            const float gauss = exp((double) -dsqr );
            *ptr++ = F2I( contrast * sinus * gauss );
          }
      }

    return result;
  }

  float Index2Theta( int index )
  {
    while( index < 0 )
      index += GABOR_MAX_ORIENT;

    index = index % GABOR_MAX_ORIENT;

    return( index * DELTA_THETA );
  }

  int Theta2Index( float theta )
  {
    int index;

    theta = Zerotopi( theta );

    index = (int)( ( theta + OFF_THETA ) / DELTA_THETA );

    return( index % GABOR_MAX_ORIENT );
  }

  float Index2Phi( int index )
  {
    while( index < 0 )
      index += GABOR_MAX_PHASE;

    index = index % GABOR_MAX_PHASE;

    return( index * DELTA_PHASE );
  }

  int Phi2Index( float phi )
  {
    int index;

    phi   = Zerototwopi( phi );

    index = (int)( ( phi + OFF_PHASE ) / DELTA_PHASE );

    return( index % GABOR_MAX_PHASE );
  }
}

GaborSet::GaborSet()
{
  const float GABOR_OMEGA    = TWOPI / GABOR_PERIOD;

  for(int n=0; n<GABOR_MAX_ORIENT; ++n)
    for(int m=0; m<GABOR_MAX_PHASE; ++m)
      {
        Patch[n][m] = CreatePatch(GABOR_SIGMA,
                                  GABOR_OMEGA,
                                  Index2Theta( n ),
                                  Index2Phi( m ),
                                  GABOR_CONTRAST,
                                  GABOR_SIZE);
      }

  printf( " Gabors allocated\n" );
  fflush( stdout );
}

GaborSet::~GaborSet()
{
  for(int i=0; i<GABOR_MAX_ORIENT; ++i)
    for(int j=0; j<GABOR_MAX_PHASE; ++j)
      delete [] Patch[i][j];

  printf( " Gabors de-allocated\n" );
  fflush( stdout );
}

Colorindex* GaborSet::getPatch(float theta, float phi) const
{
  const int itheta = Theta2Index( theta );
  const int iphi   = Phi2Index( phi );

  return( Patch[itheta][iphi] );
}
