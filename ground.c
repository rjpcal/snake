#include "ground.h"

#include "gabor.h"
#include "geom.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

namespace
{
  const double SQRT3 = 1.7320508075;
}

int Ground::tooClose(int upto, double x, double y, int except)
{
  for (int n = 0; n < upto; ++n)
    {
      double dx = fabs(array[n].pos.x - x);
      if (dx > halfX)
        dx = sizeX - dx;

      if (dx > backgMinSpacing)
        continue;

      double dy = fabs(array[n].pos.y - y);
      if (dy > halfY)
        dy = sizeY - dy;

      if (dy > backgMinSpacing)
        continue;

      if (dx*dx+dy*dy > backgMinSpacingSqr)
        continue;

      if (n == except)
        continue;

      return 1;
    }

  return 0;
}

void Ground::insideElements()
{
  insideNumber = snake.getLength();

  for (int n = snake.getLength(); n < totalNumber; ++n)
    {
      int side = 1;

      for (int i = 0; i < snake.getLength(); ++i)
        {
          const int j = (i+1) % snake.getLength();

          const double Yij = array[i].pos.x - array[j].pos.x;
          const double Xij = array[j].pos.y - array[i].pos.y;

          const double Xin = array[n].pos.x - array[i].pos.x;
          const double Yin = array[n].pos.y - array[i].pos.y;

          const double vp = Xij*Xin + Yij*Yin;

          if (vp < 0.0)
            side = 0;
        }

      if (side)
        {
          array[n].type = Element::INSIDE;
          ++insideNumber;
        }
    }
}

void Ground::contourElements()
{
  for (int n = 0; n < snake.getLength(); ++n)
    {
      if (totalNumber >= MAX_GABOR_NUMBER)
        {
          printf(" More than %d elements!\n", MAX_GABOR_NUMBER);
          exit(1);
        }

      array[totalNumber++] = snake.getElement(n);
    }
}

void Ground::gridElements()
{
  const double dx = backgIniSpacing;
  const double dy = SQRT3 * backgIniSpacing / 2.0;

  const int nx = (int)((2.0 * halfX - backgMinSpacing - 0.5*dx) / dx);
  const int ny = (int)((2.0 * halfY - backgMinSpacing) / dy);

  const double ix =  -0.5 * (nx-1) * dx - 0.25 * dx;
  const double iy =  -0.5 * (ny-1) * dy;

  int i, j;
  double x, y;

  for (j = 0, y = iy; j < ny; ++j, y += dy)
    {
      for (i = 0, x = ix+0.5*(j%2)*dx; i < nx; ++i, x += dx)
        {
          if (tooClose(snake.getLength(), x, y, snake.getLength()+1))
            continue;

          if (totalNumber > MAX_GABOR_NUMBER)
            {
              printf(" More than %d elements!\n", MAX_GABOR_NUMBER);
              exit(1);
            }

          array[totalNumber].type = Element::OUTSIDE;
          array[totalNumber].pos.x = x;
          array[totalNumber].pos.y = y;
          array[totalNumber].theta = 2 * M_PI * drand48();

          ++totalNumber;
        }
    }
}

void Ground::fillElements()
{
  const int    tryFillNumber = 250000;
  const double tryFillArea   = 6.0;

  int ntry = (int)(sizeX * sizeY / tryFillArea);

  if (ntry > tryFillNumber)
    {
      printf(" tryFillNumber too small!\n");
      printf("\n Need %d, have %d\n", ntry, tryFillNumber);
      exit(1);
    }

  ntry = 0;

  Vector vl[ tryFillNumber ];

  const double dx = (double) sqrt((double) tryFillArea);

  for (double x = -halfX; x <= halfX; x += dx)
    for (double y = -halfY; y <= halfY; y += dx)
      {
        vl[ ntry ].x = x;
        vl[ ntry ].y = y;
        ntry++;
      }

  int npts = totalNumber;

  for (int i = 0; i < ntry; ++i)
    {
      if (tooClose(npts, vl[i].x, vl[i].y, npts+1))
        continue;

      if (npts > MAX_GABOR_NUMBER)
        {
          printf(" More than %d elements: %d\n", MAX_GABOR_NUMBER, npts);
          exit(1);
        }

      array[npts].type  = Element::OUTSIDE;
      array[npts].pos   = vl[i];
      array[npts].theta = 2 * M_PI * drand48();
      npts++;
    }

  backgAveSpacing = sqrt((double)2.0*sizeX*sizeY/(SQRT3*npts));
  printf(" added %d to ave spacing %f\n", npts-totalNumber, backgAveSpacing);

  totalNumber = npts;
}

void Ground::jitterElement()
{
  const double jitter = (backgMinSpacing/16.0);

  const int backgroundIters = 1000;

  for (int niter = 0; niter < backgroundIters; ++niter)
    {
      for (int n = snake.getLength(); n < totalNumber; ++n)
        {
          Vector v;
          v.x = array[n].pos.x + 2*jitter*drand48() - jitter;
          v.y = array[n].pos.y + 2*jitter*drand48() - jitter;

          if (!tooClose(totalNumber, v.x, v.y, n))
            {
              if (v.x < -halfX) v.x += 2.*halfX;
              if (v.x >  halfX) v.x -= 2.*halfX;
              if (v.y < -halfY) v.y += 2.*halfY;
              if (v.y >  halfY) v.y -= 2.*halfY;

              array[n].pos = v;
            }
        }
    }
}

/*****************************************************************/

Ground::Ground(const Snake& s, int sizeX_, int sizeY_,
               double backgIniSpacing_,
               double backgMinSpacing_) :
  snake(s),
  sizeX(sizeX_),
  sizeY(sizeY_),
  halfX(0.5*sizeX),
  halfY(0.5*sizeY),
  backgIniSpacing(backgIniSpacing_),
  backgMinSpacing(backgMinSpacing_),
  backgMinSpacingSqr(backgMinSpacing*backgMinSpacing),
  insideNumber(0),
  totalNumber(0)
{
  contourElements();

  gridElements();

  const int diffusionCycles = 10;

  for (int i = 0; i < diffusionCycles; ++i)
    {
      jitterElement();
      fillElements();
    }

  insideElements();

  printf(" FOREG_NUMBER %d    PATCH_NUMBER %d    TOTAL_NUMBER %d\n",
          snake.getLength(), insideNumber, totalNumber);
}

void Ground::renderInto(FakeWindow* w, const GaborSet& g) const
{
  w->clear(0.0);

  for (int i = 0; i < totalNumber; ++i)
    {
      const double phi   = 2 * M_PI * drand48();
      const double randtheta = 2 * M_PI * drand48();

      const double theta =
        (i < snake.getLength())
        ? zerototwopi(array[i].theta + M_PI_2)
        : randtheta;

      const int xcenter = int(array[i].pos.x + sizeX / 2.0 + 0.5);
      const int ycenter = int(array[i].pos.y + sizeY / 2.0 + 0.5);

      const double* p = g.getPatch(theta, phi);

      // bottom left:
      const int x0 = xcenter - g.getPatchSize() / 2;
      const int y0 = ycenter - g.getPatchSize() / 2;
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
  char fname[256];
  snprintf(fname, 256, "%s.snk", filestem);

  FILE* fp = fopen(fname, "a");
  if (fp == 0)
    {
      printf("cannot append to %s\n", fname);
      exit(0);
    }

  fprintf(fp, "%-19s %d\n", "DISPLAY_COUNT", displayCount);
  fprintf(fp, "%-19s %d\n", "TOTAL_NUMBER", totalNumber);
  fprintf(fp, "%-19s %d\n", "FOREG_NUMBER", snake.getLength());
  fprintf(fp, "%-19s %d\n", "PATCH_NUMBER", insideNumber);
  fprintf(fp, "%-19s %.2f\n", "FOREG_SPACING", snake.getSpacing());
  fprintf(fp, "%-19s %.2f\n", "BACKG_AVE_SPACING", backgAveSpacing);
  fprintf(fp, "%-19s %.2f\n", "BACKG_INI_SPACING", backgIniSpacing);
  fprintf(fp, "%-19s %.2f\n", "BACKG_MIN_SPACING", backgMinSpacing);

  const double RAD2DEG = (180./M_PI);

  for (int i = 0; i < totalNumber; ++i)
    {
      const int o = int(RAD2DEG * array[i].theta + 0.5);
      const int x = int(array[i].pos.x + 0.5);
      const int y = int(array[i].pos.y + 0.5);
      const int s = int(array[i].type);

      fprintf(fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s);
    }

  fclose(fp);
}
