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

  PM.FILENAME = argv[1];

  SeedRand();

  ReadParams( "sta" );

  PrintParams();

  // alloc mem for our fake window:
  FakeWindow fakewin(PM.DISPLAY_X, PM.DISPLAY_Y);

  SeedRand();

  GaborSet gabors(PM.GABOR_PERIOD, PM.GABOR_SIGMA, PM.GABOR_SIZE);

  DISPLAY_COUNT = 0;

  DISPLAY_SET_NUMBER = 1;

  WriteHeader();

  for(int n = 0; n < PM.DISPLAY_NUMBER; ++n)
    {
      srand48(n);

      Snake s(PM.FOREG_NUMBER);

      Ground* g = new Ground(s, PM.DISPLAY_X, PM.DISPLAY_Y);

      WriteArray(g);

      g->renderInto(&fakewin, gabors);

      char fname[256];
      snprintf(fname, 256, "%s_%d.pnm", PM.FILENAME, DISPLAY_COUNT);
      fakewin.writePnm(fname);

      DISPLAY_COUNT++;

      delete g;
    }

  Exit();

  return 0;
}

void Exit( void )
{
  WriteParams( "sta" );

  exit(0);
}
