#include "gabor.h"
#include "ground.h"
#include "params.h"
#include "snake.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main( int argc, char** argv )
{
  if( argc<2 )
    {
      printf(" You forgot to supply a filename!\n");
      return 1;
    }

  const Params pm(argv[1], "sta");

  pm.print();

  // alloc mem for our fake window:
  FakeWindow fakewin(pm.pmSizeX, pm.pmSizeY);

  GaborSet gabors(pm.pmGaborPeriod, pm.pmGaborSigma, pm.pmGaborSize);

  pm.writeHeader();

  for(int n = 0; n < pm.pmDisplayNumber; ++n)
    {
#if 0
      struct timeval tp;
      gettimeofday( &tp, (timezone*) 0 );
      srand48( tp.tv_sec );
#else
      srand48(n);
#endif

      Snake s(pm.pmForegNumber, pm.pmForegSpacing);

      Ground g(s, pm.pmSizeX, pm.pmSizeY,
               pm.pmBackgIniSpacing,
               pm.pmBackgMinSpacing);

      g.writeArray(pm.pmFilestem, n);

      g.renderInto(&fakewin, gabors);

      char fname[256];
      snprintf(fname, 256, "%s_%d.pnm", pm.pmFilestem, n);
      fakewin.writePnm(fname);
    }

  pm.write("sta");

  return 0;
}
