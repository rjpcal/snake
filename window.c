#include "window.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void FakeWindow::writePnm(const char* fname) const
{
  FILE* fp = fopen( fname, "w" );
  if( fp == NULL )
    {
      printf( " %s: file not opened\n", fname );
      exit(1);
    }

  fprintf(fp, "P5\n%d %d\n%d\n", sizeX, sizeY, 255);

  // Write pixel data to file
  for(int k = 0; k < sizeX*sizeY; ++k)
    {
      const int val = int((data[k]+1.0)/2.0*255);
      assert(val >= 0);
      assert(val <= 255);
      fputc(val, fp);
    }

  fclose( fp );
}
