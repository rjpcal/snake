
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

/*************************** local *******************************/

namespace
{
  float              BACKG_ORIENTATION;
  float              FOREG_ORIENTATION;

  const double SQRT3 = 1.7320508075;

  float              BACKG_MIN_SPACING_SQR;
}

int Ground::tooClose( int upto, float x, float y, int except )
{
  for(int n = 0; n < upto; ++n)
    {
      float dx = fabs( array[n].xpos - x );
      if( dx > halfX )
        dx = sizeX - dx;

      if( dx > PM.BACKG_MIN_SPACING )
        continue;

      float dy = fabs( array[n].ypos - y );
      if( dy > halfY )
        dy = sizeY - dy;

      if( dy > PM.BACKG_MIN_SPACING )
        continue;

      if( dx*dx+dy*dy > BACKG_MIN_SPACING_SQR )
        continue;

      if( n == except )
        continue;

      return 1;
    }

  return 0;
}

void Ground::insideElements()
{
  int pn = PM.FOREG_NUMBER;

  for(int n = PM.FOREG_NUMBER; n < NPatch; ++n)
    {
      int side = 1;

      for(int i = 0; i < PM.FOREG_NUMBER; ++i)
        {
          const int j = (i+1) % PM.FOREG_NUMBER;

          const float Yij = array[i].xpos - array[j].xpos;
          const float Xij = array[j].ypos - array[i].ypos;

          const float Xin = array[n].xpos - array[i].xpos;
          const float Yin = array[n].ypos - array[i].ypos;

          const float vp = Xij*Xin + Yij*Yin;

          if( vp < 0.0 )
            side = 0;
        }

      if( side )
        {
          array[n].flag = 1;
          pn++;
        }
    }

  PM.PATCH_NUMBER = pn;
}

void Ground::contourElements(const Snake& s)
{
  for(int n = 0; n < PM.FOREG_NUMBER; ++n)
    {
      float x, y, theta;

      if( s.getElement( n, &x, &y, &theta ) )
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
  const float dx = PM.BACKG_INI_SPACING;
  const float dy = SQRT3 * PM.BACKG_INI_SPACING / 2.0;

  const int nx = (int)( ( 2.0 * halfX - PM.BACKG_MIN_SPACING - 0.5*dx ) / dx );
  const int ny = (int)( ( 2.0 * halfY - PM.BACKG_MIN_SPACING ) / dy );

  const float ix =  -0.5 * (nx-1) * dx - 0.25 * dx;
  const float iy =  -0.5 * (ny-1) * dy;

  int i, j;
  float x, y;

  for(j = 0, y = iy; j < ny; ++j, y += dy)
    {
      for(i = 0, x = ix+0.5*(j%2)*dx; i < nx; ++i, x += dx)
        {
          if( tooClose( PM.FOREG_NUMBER, x, y, PM.FOREG_NUMBER+1 ) )
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

  int ntry = (int)( sizeX * sizeY / TRY_TO_FILL_AREA );

  if( ntry > TRY_TO_FILL_NUMBER )
    {
      printf( " TRY_TO_FILL_NUMBER too small!\n" );
      printf( "\n Need %d, have %d\n", ntry, TRY_TO_FILL_NUMBER );
      Exit();
    }

  ntry = 0;

  const float dx = (float) sqrt((double) TRY_TO_FILL_AREA );

  for(float x = -halfX; x <= halfX; x += dx)
    for(float y = -halfY; y <= halfY; y += dx)
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

  BACKG_AVE_SPACING = sqrt((double)2.0*sizeX*sizeY/(SQRT3*npts));
  printf(" added %d to ave spacing %f\n", npts-NPatch, BACKG_AVE_SPACING );

  NPatch = npts;
}

void Ground::jitterElement()
{
  const float jitter = (PM.BACKG_MIN_SPACING/16.0);

  for(int niter = 0; niter < BACKGROUND_ITERATION; ++niter)
    {
      for(int n = PM.FOREG_NUMBER; n < NPatch; ++n)
        {
          const float dx = 2.*jitter*drand48() - jitter;
          const float dy = 2.*jitter*drand48() - jitter;

          float x  = array[n].xpos + dx;
          float y  = array[n].ypos + dy;

          if( !tooClose(NPatch, x, y, n) )
            {
              if( x < -halfX ) x += 2.*halfX;
              if( x >  halfX ) x -= 2.*halfX;
              if( y < -halfY ) y += 2.*halfY;
              if( y >  halfY ) y -= 2.*halfY;

              array[n].xpos = x;
              array[n].ypos = y;
            }
        }
    }
}

/*****************************************************************/

Ground::Ground(const Snake& s, int sizeX_, int sizeY_) :
  sizeX(sizeX_),
  sizeY(sizeY_),
  halfX(0.5*sizeX),
  halfY(0.5*sizeY)
{
  BACKG_MIN_SPACING_SQR = PM.BACKG_MIN_SPACING*PM.BACKG_MIN_SPACING;

  NPatch = 0;

  contourElements(s);

  gridElements();

  for(int i = 0; i < DIFFUSION_CYCLES; ++i)
    {
      jitterElement();
      fillElements();
    }

  insideElements();

  printf( " FOREG_NUMBER %d    PATCH_NUMBER %d    TOTAL_NUMBER %d\n",
          PM.FOREG_NUMBER, PM.PATCH_NUMBER, NPatch );
  PM.BACKG_NUMBER = NPatch - PM.PATCH_NUMBER;
}

void Ground::renderInto(FakeWindow* w, const GaborSet& g) const
{
  w->clear(0.0);

  for(int i = 0; i < NPatch; ++i)
    {
      const float phi   = TWOPI * drand48();
      const float randtheta = TWOPI * drand48();

      const float theta =
        (i < PM.FOREG_NUMBER)
        ? Zerototwopi( array[i].theta + M_PI_2 )
        : randtheta;

      const int xcenter = (int)( array[i].xpos + sizeX / 2.0 + 0.5 );
      const int ycenter = (int)( array[i].ypos + sizeY / 2.0 + 0.5 );

      const double* p = g.getPatch( theta, phi );

      // bottom left:
      const int x0  = xcenter - PM.GABOR_SIZE / 2;
      const int y0  = ycenter - PM.GABOR_SIZE / 2;
      // top right:
      const int x1 = x0 + PM.GABOR_SIZE;
      const int y1 = y0 + PM.GABOR_SIZE;

      for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x)
          w->blendVal(x, y, p[x-x0+(y-y0)*PM.GABOR_SIZE]);
    }
}
