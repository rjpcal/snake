
#include "ground.h"

#include "defs.h"
#include "gabor.h"
#include "geom.h"
#include "main.h"
#include "params.h"
#include "timing.h"
#include "window.h"

#include <stdlib.h>

#define            DIFFUSION_CYCLES         10
#define            BACKGROUND_ITERATION   1000
#define            TRY_TO_FILL_NUMBER   250000
#define            TRY_TO_FILL_AREA         6.0
#define            DIFFUSION_STEP       (BACKG_MIN_SPACING/16.0)

/*************************** local *******************************/

namespace
{
  float              BACKG_ORIENTATION;
  float              FOREG_ORIENTATION;

  float              HALF_X_FRAME;
  float              HALF_Y_FRAME;

  const double SQRT3 = 1.7320508075;

  float              BACKG_MIN_SPACING_SQR;
}

int Ground::tooClose( int upto, float x, float y, int except )
{
  for(int n = 0; n < upto; ++n)
    {
      float dx = fabs( array[n].xpos - x );
      if( dx > HALF_X_FRAME )
        dx = DISPLAY_X - dx;

      if( dx > BACKG_MIN_SPACING )
        continue;

      float dy = fabs( array[n].ypos - y );
      if( dy > HALF_Y_FRAME )
        dy = DISPLAY_Y - dy;

      if( dy > BACKG_MIN_SPACING )
        continue;

      if( dx*dx+dy*dy > BACKG_MIN_SPACING_SQR )
        continue;

      if( n == except )
        continue;

      return 1;
    }

  return 0;
}

void Ground::insideElements( int total_number, int foreg_number,
                             int *ppatch_number )
{
  int n, i, j, side, pn;
  float Xij, Yij, Xin, Yin;
  float vp[ MAX_FOREG_NUMBER ];

  pn = foreg_number;

  for( n=foreg_number; n<total_number; n++ )
    {
      side = 1;

      for( i=0; i<foreg_number; i++ )
        {
          j = (i+1) % foreg_number;

          Yij      = array[i].xpos - array[j].xpos;
          Xij      = array[j].ypos - array[i].ypos;

          Xin      = array[n].xpos - array[i].xpos;
          Yin      = array[n].ypos - array[i].ypos;

          if( ( vp[i] = Xij*Xin + Yij*Yin ) < 0.0 )
            {
              side = 0;
            }
        }

      if( side )
        {
          array[n].flag = 1;
          pn++;
        }
    }

  *ppatch_number = pn;
}

void Ground::contourElements()
{
  for(int n = 0; n < FOREG_NUMBER; ++n)
    {
      float x, y, theta;

      if( GetElement( n, &x, &y, &theta ) )
        {
          array[NPatch].flag  = 2;
          array[NPatch].xpos  = x;
          array[NPatch].ypos  = y;
          array[NPatch].theta = theta;
          ++NPatch;
        }
    }

  if( NPatch > MAX_GABOR_NUMBER )
    {
      printf( " More than %d elements!\n", MAX_GABOR_NUMBER );
      Exit();
    }
}

void Ground::gridElements()
{
  const float dx = BACKG_INI_SPACING;
  const float dy = SQRT3 * BACKG_INI_SPACING / 2.0;

  const int nx = (int)( ( 2.0 * HALF_X_FRAME - BACKG_MIN_SPACING - 0.5*dx ) / dx );
  const int ny = (int)( ( 2.0 * HALF_Y_FRAME - BACKG_MIN_SPACING ) / dy );

  const float ix =  -0.5 * (nx-1) * dx - 0.25 * dx;
  const float iy =  -0.5 * (ny-1) * dy;

  int i, j;
  float x, y;

  for(j = 0, y = iy; j < ny; ++j, y += dy)
    {
      for(i = 0, x = ix+0.5*(j%2)*dx; i < nx; ++i, x += dx)
        {
          if( tooClose( FOREG_NUMBER, x, y, FOREG_NUMBER+1 ) )
            continue;

          array[NPatch].flag = 0;
          array[NPatch].xpos = x;
          array[NPatch].ypos = y;
          array[NPatch].theta = TWOPI * drand48();

          ++NPatch;
        }
    }

  if( NPatch > MAX_GABOR_NUMBER )
    {
      printf( " More than %d elements!\n", MAX_GABOR_NUMBER );
      Exit();
    }
}

void Ground::fillElements()
{
  float xl[ TRY_TO_FILL_NUMBER ];
  float yl[ TRY_TO_FILL_NUMBER ];

  int ntry = (int)( DISPLAY_X * DISPLAY_Y / TRY_TO_FILL_AREA );

  if( ntry > TRY_TO_FILL_NUMBER )
    {
      printf( " TRY_TO_FILL_NUMBER too small!\n" );
      printf( "\n Need %d, have %d\n", ntry, TRY_TO_FILL_NUMBER );
      Exit();
    }

  ntry = 0;

  const float dx = (float) sqrt((double) TRY_TO_FILL_AREA );

  for(float x = -HALF_X_FRAME; x <= HALF_X_FRAME; x += dx)
    for(float y = -HALF_Y_FRAME; y <= HALF_Y_FRAME; y += dx)
      {
        xl[ ntry ] = x;
        yl[ ntry ] = y;
        ntry++;
      }

  int npts = NPatch;

  for(int i = 0; i < ntry; ++i)
    {
      if( tooClose( npts, xl[i], yl[i], npts+1 ) )
        continue;

      array[npts].flag  = 0;
      array[npts].xpos  = xl[i];
      array[npts].ypos  = yl[i];
      array[npts].theta = TWOPI * drand48();
      npts++;
    }

  if( npts > MAX_GABOR_NUMBER )
    {
      printf( " More than %d elements: %d\n", MAX_GABOR_NUMBER, npts );
      Exit();
    }

  BACKG_AVE_SPACING = sqrt((double)2.0*DISPLAY_X*DISPLAY_Y/(SQRT3*npts));
  printf(" added %d to ave spacing %f\n", npts-NPatch, BACKG_AVE_SPACING );

  NPatch = npts;
}

void Ground::jitterElement( void )
{
  for(int niter = 0; niter < BACKGROUND_ITERATION; ++niter)
    {
      for(int n = FOREG_NUMBER; n < NPatch; ++n)
        {
          const float dx = 2.*DIFFUSION_STEP*drand48() - DIFFUSION_STEP;
          const float dy = 2.*DIFFUSION_STEP*drand48() - DIFFUSION_STEP;

          float x  = array[n].xpos + dx;
          float y  = array[n].ypos + dy;

          if( !tooClose(NPatch, x, y, n) )
            {
              if( x < -HALF_X_FRAME )
                x += 2.*HALF_X_FRAME;

              if( x >  HALF_X_FRAME )
                x -= 2.*HALF_X_FRAME;

              if( y < -HALF_Y_FRAME )
                y += 2.*HALF_Y_FRAME;

              if( y >  HALF_Y_FRAME )
                y -= 2.*HALF_Y_FRAME;

              array[n].xpos = x;
              array[n].ypos = y;
            }
        }
    }
}

/*****************************************************************/

Ground::Ground()
{
  HALF_X_FRAME = 0.5 * DISPLAY_X;
  HALF_Y_FRAME = 0.5 * DISPLAY_Y;
  BACKG_MIN_SPACING_SQR = BACKG_MIN_SPACING*BACKG_MIN_SPACING;

  NPatch = 0;

  this->contourElements();

  this->gridElements();

  for(int i = 0; i < DIFFUSION_CYCLES; ++i)
    {
      this->jitterElement();

      this->fillElements();
    }

  this->insideElements( NPatch, FOREG_NUMBER, &PATCH_NUMBER );

  printf( " FOREG_NUMBER %d    PATCH_NUMBER %d    TOTAL_NUMBER %d\n",
          FOREG_NUMBER, PATCH_NUMBER, NPatch );
  BACKG_NUMBER = NPatch - PATCH_NUMBER;
}

void Ground::renderInto(FakeWindow* w, const GaborSet& g) const
{
  w->clear(0.0);

  for(int i = 0; i < NPatch; ++i)
    {
      const float phi   = TWOPI * drand48();
      const float randtheta = TWOPI * drand48();

      const float theta =
        (i < FOREG_NUMBER)
        ? Zerototwopi( array[i].theta + M_PI_2 )
        : randtheta;

      const int xcenter = (int)( array[i].xpos + DISPLAY_X / 2.0 + 0.5 );
      const int ycenter = (int)( array[i].ypos + DISPLAY_Y / 2.0 + 0.5 );

      const double* p = g.getPatch( theta, phi );

      // bottom left:
      const int x0  = xcenter - GABOR_SIZE / 2;
      const int y0  = ycenter - GABOR_SIZE / 2;
      // top right:
      const int x1 = x0 + GABOR_SIZE;
      const int y1 = y0 + GABOR_SIZE;

      for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x)
          w->blendVal(x, y, p[x-x0+(y-y0)*GABOR_SIZE]);
    }
}
