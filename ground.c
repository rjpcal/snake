#include "ground.h"

#include "gabor.h"
#include "geom.h"
#include "params.h"
#include "timing.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

namespace
{
  const double SQRT3 = 1.7320508075;
}

int Ground::tooClose( int upto, float x, float y, int except )
{
  for(int n = 0; n < upto; ++n)
    {
      float dx = fabs( array[n].xpos - x );
      if( dx > halfX )
        dx = sizeX - dx;

      if( dx > backgMinSpacing )
        continue;

      float dy = fabs( array[n].ypos - y );
      if( dy > halfY )
        dy = sizeY - dy;

      if( dy > backgMinSpacing )
        continue;

      if( dx*dx+dy*dy > backgMinSpacingSqr )
        continue;

      if( n == except )
        continue;

      return 1;
    }

  return 0;
}

void Ground::insideElements()
{
  patchNumber = snake.getLength();

  for(int n = snake.getLength(); n < NPatch; ++n)
    {
      int side = 1;

      for(int i = 0; i < snake.getLength(); ++i)
        {
          const int j = (i+1) % snake.getLength();

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
          ++patchNumber;
        }
    }
}

void Ground::contourElements()
{
  for(int n = 0; n < snake.getLength(); ++n)
    {
      float x, y, theta;

      if( snake.getElement( n, &x, &y, &theta ) )
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
      exit(1);
    }
}

void Ground::gridElements()
{
  const float dx = backgIniSpacing;
  const float dy = SQRT3 * backgIniSpacing / 2.0;

  const int nx = (int)( ( 2.0 * halfX - backgMinSpacing - 0.5*dx ) / dx );
  const int ny = (int)( ( 2.0 * halfY - backgMinSpacing ) / dy );

  const float ix =  -0.5 * (nx-1) * dx - 0.25 * dx;
  const float iy =  -0.5 * (ny-1) * dy;

  int i, j;
  float x, y;

  for(j = 0, y = iy; j < ny; ++j, y += dy)
    {
      for(i = 0, x = ix+0.5*(j%2)*dx; i < nx; ++i, x += dx)
        {
          if( tooClose( snake.getLength(), x, y, snake.getLength()+1 ) )
            continue;

          array[NPatch].flag = 0;
          array[NPatch].xpos = x;
          array[NPatch].ypos = y;
          array[NPatch].theta = 2 * M_PI * drand48();

          ++NPatch;
        }
    }

  if( NPatch > MAX_GABOR_NUMBER )
    {
      printf( " More than %d elements!\n", MAX_GABOR_NUMBER );
      exit(1);
    }
}

void Ground::fillElements()
{
  const int    tryFillNumber = 250000;
  const double tryFillArea   = 6.0;

  int ntry = (int)( sizeX * sizeY / tryFillArea );

  if( ntry > tryFillNumber )
    {
      printf( " tryFillNumber too small!\n" );
      printf( "\n Need %d, have %d\n", ntry, tryFillNumber );
      exit(1);
    }

  ntry = 0;

  float xl[ tryFillNumber ];
  float yl[ tryFillNumber ];

  const float dx = (float) sqrt((double) tryFillArea );

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
      array[npts].theta = 2 * M_PI * drand48();
      npts++;
    }

  if( npts > MAX_GABOR_NUMBER )
    {
      printf( " More than %d elements: %d\n", MAX_GABOR_NUMBER, npts );
      exit(1);
    }

  backgAveSpacing = sqrt((double)2.0*sizeX*sizeY/(SQRT3*npts));
  printf(" added %d to ave spacing %f\n", npts-NPatch, backgAveSpacing );

  NPatch = npts;
}

void Ground::jitterElement()
{
  const float jitter = (backgMinSpacing/16.0);

  const int backgroundIters = 1000;

  for(int niter = 0; niter < backgroundIters; ++niter)
    {
      for(int n = snake.getLength(); n < NPatch; ++n)
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

Ground::Ground(const Snake& s, int sizeX_, int sizeY_,
               float backgIniSpacing_,
               float backgMinSpacing_) :
  snake(s),
  sizeX(sizeX_),
  sizeY(sizeY_),
  halfX(0.5*sizeX),
  halfY(0.5*sizeY),
  backgIniSpacing(backgIniSpacing_),
  backgMinSpacing(backgMinSpacing_),
  backgMinSpacingSqr(backgMinSpacing*backgMinSpacing),
  patchNumber(0),
  backgNumber(0),
  NPatch(0)
{
  contourElements();

  gridElements();

  const int diffusionCycles = 10;

  for(int i = 0; i < diffusionCycles; ++i)
    {
      jitterElement();
      fillElements();
    }

  insideElements();

  printf( " FOREG_NUMBER %d    PATCH_NUMBER %d    TOTAL_NUMBER %d\n",
          snake.getLength(), patchNumber, NPatch );
  backgNumber = NPatch - patchNumber;
}

void Ground::renderInto(FakeWindow* w, const GaborSet& g) const
{
  w->clear(0.0);

  for(int i = 0; i < NPatch; ++i)
    {
      const float phi   = 2 * M_PI * drand48();
      const float randtheta = 2 * M_PI * drand48();

      const float theta =
        (i < snake.getLength())
        ? Zerototwopi( array[i].theta + M_PI_2 )
        : randtheta;

      const int xcenter = (int)( array[i].xpos + sizeX / 2.0 + 0.5 );
      const int ycenter = (int)( array[i].ypos + sizeY / 2.0 + 0.5 );

      const double* p = g.getPatch( theta, phi );

      // bottom left:
      const int x0  = xcenter - g.getPatchSize() / 2;
      const int y0  = ycenter - g.getPatchSize() / 2;
      // top right:
      const int x1 = x0 + g.getPatchSize();
      const int y1 = y0 + g.getPatchSize();

      for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x)
          w->blendVal(x, y, p[x-x0+(y-y0)*g.getPatchSize()]);
    }
}

void Ground::writeArray(const char* filestem, int displayCount) const
{
  int x, y, o, s;

  char fname[STRINGSIZE];
  sprintf( fname, "%s.snk", filestem);

  FILE* fp = fopen(fname, "a");
  if (fp == 0)
    {
      printf("cannot append to %s\n", fname);
      exit(0);
    }

  putint(fp, displayCount, "DISPLAY_COUNT");
  putint(fp, NPatch, "TOTAL_NUMBER");
  putint(fp, snake.getLength(), "FOREG_NUMBER");
  putint(fp, this->patchNumber, "PATCH_NUMBER");
  putfloat(fp, snake.getSpacing(), "FOREG_SPACING");
  putfloat(fp, backgAveSpacing, "BACKG_AVE_SPACING");
  putfloat(fp, this->backgIniSpacing, "BACKG_INI_SPACING");
  putfloat(fp, this->backgMinSpacing, "BACKG_MIN_SPACING");

  const double RAD2DEG = (180./M_PI);

  for(int i = 0; i < NPatch; ++i)
    {
      if( this->flag(i) )
        {
          o = (int)( RAD2DEG * this->theta(i) + 0.5 );

          x = (int)( this->xpos(i) + 0.5 );

          y = (int)( this->ypos(i) + 0.5 );

          s = this->flag(i);

          fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

  for(int i = 0; i < NPatch; ++i)
    {
      if( !this->flag(i) )
        {
          o = (int)( RAD2DEG * this->theta(i) + 0.5 );

          x = (int)( this->xpos(i) + 0.5 );

          y = (int)( this->ypos(i) + 0.5 );

          s = this->flag(i);

          fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

  fclose(fp);
}
