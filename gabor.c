
#include "gabor.h"

#include "gamma.h"
#include "geom.h"
#include "params.h"

#include <stdlib.h>

float DELTA_THETA, DELTA_PHASE, OFF_THETA, OFF_PHASE;

namespace
{
  const float GABOR_CONTRAST = 1.0f;

  void InitPatch( Colorindex *ptr, float sigma, float omega, float theta, float phi, float contrast, int xysize )
  {
    int ix, iy;
    float sin_theta, cos_theta, dx, dy, fx, fy,
      ssqr, dsqr, sinus, gauss;

    ssqr  = 2.*sigma*sigma;

    cos_theta = cos((double) theta );
    sin_theta = sin((double) theta );

    for( iy=0; iy<xysize; iy++ )
      {
        fy = (float)( iy - xysize / 2.0 + 0.5 );

        for( ix=0; ix<xysize; ix++ )
          {
            fx = (float)( ix - xysize / 2.0 + 0.5 );

            dx = cos_theta * fx - sin_theta * fy;
            dy = sin_theta * fx + cos_theta * fy;

            dsqr  = ( dx*dx + dy*dy ) / ssqr;

            sinus = cos((double) omega * dx + phi );

            gauss = exp((double) -dsqr );
            *ptr++ = F2I( contrast * sinus * gauss );
          }
      }
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
  int n, m;

  int i, j;

  for( i=0; i<GABOR_MAX_ORIENT; i++ )
    for( j=0; j<GABOR_MAX_PHASE; j++ )
      {
        Patch[i][j] = (Colorindex*) calloc( GABOR_SIZE*GABOR_SIZE, sizeof( Colorindex ) );
      }

  printf( " Gabors allocated\n" );
  fflush( stdout );

  float GABOR_OMEGA    = TWOPI / GABOR_PERIOD;

  DELTA_THETA    = M_PI / GABOR_MAX_ORIENT;
  OFF_THETA      = 0.5 * DELTA_THETA;
  DELTA_PHASE    = TWOPI / GABOR_MAX_PHASE;
  OFF_PHASE      = 0.5 * DELTA_PHASE;

  for( n=0; n<GABOR_MAX_ORIENT; n++ )
    for( m=0; m<GABOR_MAX_PHASE; m++ )
      {
        InitPatch(  Patch[n][m],
                    GABOR_SIGMA,
                    GABOR_OMEGA,
                    Index2Theta( n ),
                    Index2Phi( m ),
                    GABOR_CONTRAST,
                    GABOR_SIZE );
      }
}

GaborSet::~GaborSet()
{
  for(int i=0; i<GABOR_MAX_ORIENT; ++i)
    for(int j=0; j<GABOR_MAX_PHASE; ++j)
      free( (char*) Patch[i][j] );

  printf( " Gabors de-allocated\n" );
  fflush( stdout );
}

Colorindex* GaborSet::getPatch(float theta, float phi) const
{
  const int itheta = Theta2Index( theta );
  const int iphi   = Phi2Index( phi );

  return( Patch[itheta][iphi] );
}
