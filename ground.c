
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

void Ground::contourElements( int *pnpts )
{
  int n, npts;
  float x, y, theta;

  npts = *pnpts;

  for( n=0; n<FOREG_NUMBER; n++ )
    {
      if( GetElement( n, &x, &y, &theta ) )
        {
          array[npts].flag  = 2;
          array[npts].xpos  = x;
          array[npts].ypos  = y;
          array[npts].theta = theta;
          npts++;
        }
    }

  if( npts > MAX_GABOR_NUMBER )
    {
      printf( " More than %d elements!\n", MAX_GABOR_NUMBER );
      Exit();
    }

  *pnpts = npts;
}

void Ground::gridElements( int *pnpts )
{
  int i, j, nx, ny, npts;
  float x, y, ix, iy, dx, dy;
  dx = BACKG_INI_SPACING;
  nx = (int)( ( 2.0 * HALF_X_FRAME - BACKG_MIN_SPACING - 0.5*dx ) / dx );
  ix =  -0.5 * (nx-1) * dx - 0.25 * dx;

  dy = SQRT3 * BACKG_INI_SPACING / 2.0;
  ny = (int)( ( 2.0 * HALF_Y_FRAME - BACKG_MIN_SPACING ) / dy );
  iy =  -0.5 * (ny-1) * dy;

  npts = *pnpts;
  for( j=0, y=iy; j<ny; j++, y+=dy )
    {
      for( i=0, x=ix+0.5*(j%2)*dx; i<nx; i++, x+=dx )
        {
          if( tooClose( FOREG_NUMBER, x, y, FOREG_NUMBER+1 ) )
            continue;

          array[npts].flag = 0;
          array[npts].xpos = x;
          array[npts].ypos = y;
          array[npts].theta = TWOPI * drand48();

          npts++;
        }
    }

  if( npts > MAX_GABOR_NUMBER )
    {
      printf( " More than %d elements!\n", MAX_GABOR_NUMBER );
      Exit();
    }

  *pnpts = npts;
}

void Ground::fillElements( int *pnpts )
{
  int i, npts, ntry;
  float x, y, dx, xl[ TRY_TO_FILL_NUMBER ], yl[ TRY_TO_FILL_NUMBER ];

  ntry = (int)( DISPLAY_X * DISPLAY_Y / TRY_TO_FILL_AREA );

  if( ntry > TRY_TO_FILL_NUMBER )
    {
      printf( " TRY_TO_FILL_NUMBER too small!\n" );
      printf( "\n Need %d, have %d\n", ntry, TRY_TO_FILL_NUMBER );
      Exit();
    }

  ntry = 0;

  dx = (float) sqrt((double) TRY_TO_FILL_AREA );

  for( x = -HALF_X_FRAME; x <= HALF_X_FRAME; x+=dx )
    for( y = -HALF_Y_FRAME; y <= HALF_Y_FRAME; y+=dx )
      {
        xl[ ntry ] = x;
        yl[ ntry ] = y;
        ntry++;
      }

  npts = *pnpts;
  for( i=0; i<ntry; i++ )
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
  printf( " added %d to ave spacing %f\n",
          (npts-(*pnpts)), BACKG_AVE_SPACING );

  *pnpts = npts;
}

void Ground::jitterElement( void )
{
  int n, niter;
  float x, y, dx, dy, jitter;

  jitter = DIFFUSION_STEP;

  for( niter=0; niter<BACKGROUND_ITERATION; niter++ )
    {
      for( n=FOREG_NUMBER; n<TOTAL_NUMBER; n++ )
        {
          dx = 2.*jitter*drand48() - jitter;
          dy = 2.*jitter*drand48() - jitter;

          x  = array[n].xpos + dx;
          y  = array[n].ypos + dy;

          if( !tooClose( TOTAL_NUMBER, x, y, n ) )
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

void Ground::map2array(const GaborSet& g, int npts)
{
  int i;
  float theta, phi, Theta[ MAX_FOREG_NUMBER ], Phi[ MAX_FOREG_NUMBER ];

  for( i=0; i<npts && i<FOREG_NUMBER; i++ )
    {
      Phi[i]   = Zerototwopi( TWOPI * drand48() );
      Theta[i] = Zerototwopi( TWOPI * drand48() );
    }

  for( i=0; i<npts; i++ )
    {
      phi   = TWOPI * drand48();
      theta = TWOPI * drand48();

      if( i < FOREG_NUMBER )
        {
          phi   = Phi[ i ];
          theta = Zerototwopi( array[i].theta + M_PI_2 );
        }

      XPatch[ i ] = (int)( array[ i ].xpos + DISPLAY_X / 2.0 + 0.5 );
      YPatch[ i ] = (int)( array[ i ].ypos + DISPLAY_Y / 2.0 + 0.5 );

      PPatch[ i ] = g.getPatch( theta, phi );
    }

  NPatch = npts;

  SeedRand();
}

/*****************************************************************/

Ground::Ground(const GaborSet& g)
{
  HALF_X_FRAME = 0.5 * DISPLAY_X;
  HALF_Y_FRAME = 0.5 * DISPLAY_Y;
  BACKG_MIN_SPACING_SQR = BACKG_MIN_SPACING*BACKG_MIN_SPACING;

  int npts = 0;

  this->contourElements( &npts );

  this->map2array(g, npts); // FIXME need to do this?

  TOTAL_NUMBER = npts;

  this->gridElements( &npts );

  TOTAL_NUMBER = npts;

  for(int i = 0; i < DIFFUSION_CYCLES; ++i)
    {
      this->jitterElement();

      this->fillElements( &npts );

      this->map2array(g, npts);

      TOTAL_NUMBER = npts;
    }

  this->insideElements( TOTAL_NUMBER, FOREG_NUMBER, &PATCH_NUMBER );

  printf( " FOREG_NUMBER %d    PATCH_NUMBER ??    TOTAL_NUMBER %d\n",
          FOREG_NUMBER, TOTAL_NUMBER );
  BACKG_NUMBER = TOTAL_NUMBER - PATCH_NUMBER;
}

void Ground::renderInto(FakeWindow* wind) const
{
  wind->clear(0.0);

  const int* px           = XPatch;
  const int* py           = YPatch;
  const double* const* pp = PPatch;

  for(int i = 0; i < NPatch; ++i, ++px, ++py, ++pp)
    {
      // bottom left:
      int x0  = *px - GABOR_SIZE / 2;
      int y0  = *py - GABOR_SIZE / 2;
      // top right:
      int x1 = x0 + GABOR_SIZE;
      int y1 = y0 + GABOR_SIZE;

      for (int yy = y0; yy < y1; ++yy)
        for (int xx = x0; xx < x1; ++xx)
          if (xx>=0 && xx<DISPLAY_X && yy>=0 && yy<DISPLAY_Y)
            {
              if (fabs(wind->data[xx+yy*DISPLAY_X])
                  < fabs((*pp)[xx-x0+(yy-y0)*GABOR_SIZE]))
                wind->data[xx+yy*DISPLAY_X] =
                  (*pp)[xx-x0+(yy-y0)*GABOR_SIZE];
            }
    }
}
