#include "ground.h"

#include "gabor.h"
#include "geom.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace
{
  const double SQRT3 = 1.7320508075;
}

bool Ground::tryPush(const Element& e)
{
  if (tooClose(e.pos, -1)) return false;

  if (totalNumber >= MAX_GABOR_NUMBER)
    {
      printf(" More than %d elements!\n", MAX_GABOR_NUMBER);
      exit(1);
    }

  array[totalNumber++] = e;

  dumpFrame();
  dumpFrame();

  return true;
}

bool Ground::tooClose(const Vector& v, int except)
{
  for (int n = 0; n < totalNumber; ++n)
    {
      const double dx = array[n].pos.x - v.x;
      const double dy = array[n].pos.y - v.y;

      if (dx*dx+dy*dy <= backgMinSpacingSqr && n != except)
        return true;
    }

  return false;
}

void Ground::insideElements()
{
  insideNumber = snake.getLength();

  for (int n = 0; n < totalNumber; ++n)
    {
      if (array[n].type == Element::CONTOUR)
        continue;

      bool inside = true;

      for (int i = 0; i < snake.getLength(); ++i)
        {
          const int j = (i+1) % snake.getLength();

          const double Yij = array[i].pos.x - array[j].pos.x;
          const double Xij = array[j].pos.y - array[i].pos.y;

          const double Xin = array[n].pos.x - array[i].pos.x;
          const double Yin = array[n].pos.y - array[i].pos.y;

          const double vp = Xij*Xin + Yij*Yin;

          if (vp < 0.0)
            {
              inside = false;
              continue;
            }
        }

      if (inside)
        {
          array[n].type = Element::INSIDE;
          ++insideNumber;
        }
    }
}

void Ground::hexGridElements()
{
  // lay down a hexagonal grid of elements

  const double dx = backgIniSpacing;
  const double dy = SQRT3 * backgIniSpacing / 2.0;

  const int nx = int((sizeX - backgMinSpacing) / dx - 0.5);
  const int ny = int((sizeY - backgMinSpacing) / dy);

  double y = -0.5 * (ny-1) * dy;

  for (int j = 0; j < ny; ++j, y += dy)
    {
      double x = -0.5 * (nx-1) * dx - 0.25 * dx;

      // this is a hexagonal grid, so every other row is staggered by half
      // a step in the x direction
      if (j%2) x += 0.5*dx;

      for (int i = 0; i < nx; ++i, x += dx)
        {
          tryPush(Element(x, y, 2 * M_PI * drand48(), Element::OUTSIDE));
        }
    }
}

void Ground::fillElements()
{
  const double tryFillArea = 6.0;

  const double dx = sqrt(tryFillArea);

  for (double x = -halfX; x <= halfX; x += dx)
    for (double y = -halfY; y <= halfY; y += dx)
      {
        tryPush(Element(x, y, 2 * M_PI * drand48(), Element::OUTSIDE));
      }

  backgAveSpacing = sqrt(2.0*sizeX*sizeY/(SQRT3*totalNumber));
  printf(" %d elements, ave spacing %f\n", totalNumber, backgAveSpacing);
}

void Ground::jitterElement()
{
  const double jitter = (backgMinSpacing/16.0);

  const int backgroundIters = 1000;

  for (int niter = 0; niter < backgroundIters; ++niter)
    {
      for (int n = 0; n < totalNumber; ++n)
        {
          if (array[n].type == Element::CONTOUR)
            continue;

          Vector v;
          v.x = array[n].pos.x + jitter*(2*drand48() - 1);
          v.y = array[n].pos.y + jitter*(2*drand48() - 1);

          if (v.x < -halfX) v.x += 2.*halfX;
          if (v.x >  halfX) v.x -= 2.*halfX;
          if (v.y < -halfY) v.y += 2.*halfY;
          if (v.y >  halfY) v.y -= 2.*halfY;

          if (!tooClose(v, n))
            {
              array[n].pos = v;
            }
        }

      if (niter % 15 == 0) dumpFrame();
    }
}

void Ground::dumpFrame() const
{
  if (0)
    {
      static int framecount = 0;

      char fname[256];
      snprintf(fname, 256, "frame_%06d.pnm", framecount++);

      this->writePnm(fname);

      printf("dumped frame %s\n", fname);
    }
}

Ground::Ground(const Snake& s, int sizeX_, int sizeY_,
               double backgIniSpacing_,
               double backgMinSpacing_,
               const GaborSet& g) :
  snake(s),
  sizeX(sizeX_),
  sizeY(sizeY_),
  halfX(0.5*sizeX),
  halfY(0.5*sizeY),
  backgIniSpacing(backgIniSpacing_),
  backgMinSpacing(backgMinSpacing_),
  backgMinSpacingSqr(backgMinSpacing*backgMinSpacing),
  insideNumber(0),
  totalNumber(0),
  gabors(g)
{
  // pull in elements from the snake
  for (int n = 0; n < snake.getLength(); ++n)
    {
      if (!tryPush(snake.getElement(n)))
        {
          printf("snake elements were too close together!\n");
          exit(1);
        }
    }

  hexGridElements();

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

void Ground::writePnm(const char* filename) const
{
  std::vector<double> win(sizeX*sizeY);

  for (int i = 0; i < sizeX*sizeY; ++i)
    win[i] = 0.0;

  for (int i = 0; i < totalNumber; ++i)
    {
      const double phi   = 2 * M_PI * drand48();
//       const double phi = 0.0;

      const double theta =
        (array[i].type == Element::CONTOUR)
        ? zerototwopi(array[i].theta + M_PI_2)
        : array[i].theta;

      const int xcenter = int(array[i].pos.x + sizeX / 2.0 + 0.5);
      const int ycenter = int(array[i].pos.y + sizeY / 2.0 + 0.5);

      const double* p = gabors.getPatch(theta, phi);

      // bottom left:
      const int x0 = xcenter - gabors.getPatchSize() / 2;
      const int y0 = ycenter - gabors.getPatchSize() / 2;
      // top right:
      const int x1 = x0 + gabors.getPatchSize();
      const int y1 = y0 + gabors.getPatchSize();

      for (int y = y0; y < y1; ++y)
        for (int x = x0; x < x1; ++x)
          {
            if (x >= 0 && x < sizeX && y >=0 && y < sizeY)
              win[x+y*sizeX] += p[x-x0+(y-y0)*gabors.getPatchSize()];
          }
    }

  FILE* fp = fopen(filename, "w");

  if (fp == 0)
    {
      printf(" %s: file not opened\n", filename);
      exit(1);
    }

  fprintf(fp, "P5\n%d %d\n%d\n", sizeX, sizeY, 255);

  // Write pixel data to file
  for (int k = 0; k < sizeX*sizeY; ++k)
    {
      const int val = int((win[k]+1.0)/2.0*255);
      assert(val >= 0);
      assert(val <= 255);
      fputc(val, fp);
    }

  fclose(fp);
}
