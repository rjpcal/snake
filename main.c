
#include "defs.h"
#include "file.h"
#include "gabor.h"
#include "gamma.h"
#include "ground.h"
#include "params.h"
#include "timing.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

void Exit();

int DISPLAY_SET_NUMBER;
int DISPLAY_COUNT;

int main( int argc, char** argv )
{
  if( argc<2 )
    {
      printf(" You forgot to supply a filename!\n");
      Exit();
    }

  FILENAME = argv[1];

  SeedRand();

  ReadParams( "sta" );

  PrintParams();

  MakeColormap();

  // alloc mem for our fake window:
  FakeWindow fakewin;
  fakewin.data = new Colorindex[DISPLAY_X*DISPLAY_Y];

  SeedRand();

  InitGabor();

  DISPLAY_COUNT = 0;

  DISPLAY_SET_NUMBER = 1;

  WriteHeader();

  for(int n = 0; n < DISPLAY_NUMBER; ++n)
    {
      srand48(n);

      MakeForeg();

      Ground* g = Ground::make();

      WriteArray(g);

      ShowArray(g, &fakewin);

      Window2Raster(&fakewin);

      DISPLAY_COUNT++;

      delete g;
    }

  Exit();

  return 0;
}

void Exit( void )
{
  WrapGabor();

  WriteParams( "sta" );

  exit(0);
}
