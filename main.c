
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

char PROGRAM[ STRINGSIZE ];

/// memory for fake window:
Colorindex *win;

int   DISPLAY_SET_NUMBER;
int   DISPLAY_COUNT;

int main( int argc, char** argv )
{
  if( argc<2 )
    {
      printf(" You forgot to supply a filename!\n");
      Exit();
    }

  sprintf( PROGRAM, "%s", argv[0]);
  sprintf( FILENAME,"%s", argv[1]);

  SeedRand();

  ReadParams( "sta" );

  PrintParams();

  MakeColormap();

  // alloc mem for our fake window:
  win = (Colorindex*)
    (malloc(DISPLAY_X * DISPLAY_Y * sizeof(Colorindex)));

  SeedRand();

  InitGabor();

  DISPLAY_COUNT = 0;

  DISPLAY_SET_NUMBER = 1;

  WriteHeader();

  int n;

  for( n=0; n<DISPLAY_NUMBER; n++ )
    {
      ZeroRand( n );

      MakeForeg();

      Ground* g = Ground::make();

      WriteArray(g);

      ShowArray(g);

      Window2Raster();

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

