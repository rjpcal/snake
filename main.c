#include "main.h"

#include "defs.h"
#include "gabor.h"
#include "ground.h"
#include "params.h"
#include "snake.h"
#include "timing.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

int DISPLAY_SET_NUMBER;
int DISPLAY_COUNT;

int main( int argc, char** argv )
{
  if( argc<2 )
    {
      printf(" You forgot to supply a filename!\n");
      Exit();
    }

  SeedRand();

  Params pm;

  pm.FILENAME = argv[1];

  pm.read("sta");
  pm.print();

  // alloc mem for our fake window:
  FakeWindow fakewin(pm.DISPLAY_X, pm.DISPLAY_Y);

  SeedRand();

  GaborSet gabors(pm.GABOR_PERIOD, pm.GABOR_SIGMA, pm.GABOR_SIZE);

  DISPLAY_COUNT = 0;

  DISPLAY_SET_NUMBER = 1;

  pm.writeHeader();

  for(int n = 0; n < pm.DISPLAY_NUMBER; ++n)
    {
      srand48(n);

      Snake s(pm.FOREG_NUMBER, pm.FOREG_SPACING);

      Ground* g = new Ground(s, pm.DISPLAY_X, pm.DISPLAY_Y,
                             pm.BACKG_INI_SPACING,
                             pm.BACKG_MIN_SPACING);

      WriteArray(pm.FILENAME, g);

      g->renderInto(&fakewin, gabors);

      char fname[256];
      snprintf(fname, 256, "%s_%d.pnm", pm.FILENAME, DISPLAY_COUNT);
      fakewin.writePnm(fname);

      DISPLAY_COUNT++;

      delete g;
    }

  pm.write("sta");

  return 0;
}

void Exit( void )
{
  exit(0);
}
