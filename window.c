
#include "window.h"

#include "defs.h"
#include "gamma.h"
#include "ground.h"
#include "params.h"

#include <math.h>
#include <stdlib.h>
#include <netinet/in.h> // for conversion to big endian:


void ShowArray(const Ground* g, FakeWindow* wind)
{
  wind->clear(Grey);

  const int* px         = g->XPatch;
  const int* py         = g->YPatch;
  Colorindex* const* pp = g->PPatch;

  for(int i = 0; i < g->NPatch; ++i, ++px, ++py, ++pp)
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
              if (fabs(wind->data[xx+yy*DISPLAY_X] - Grey)
                  < fabs((*pp)[xx-x0+(yy-y0)*GABOR_SIZE] - Grey))
                wind->data[xx+yy*DISPLAY_X] =
                  (*pp)[xx-x0+(yy-y0)*GABOR_SIZE];
            }
    }
}

void FakeWindow::writeRaster(const char* fname) const
{
  FILE* fp = fopen( fname, "w" );
  if( fp == NULL )
    {
      printf( " %s: file not opened\n", fname );
      exit(1);
    }

  int header[8];

  header[0] = 0x59a66a95;
  header[1] = sizeX;
  header[2] = sizeY;
  header[3] = 8;
  header[4] = sizeX * sizeY;
  header[5] = 1;
  header[6] = 1;
  header[7] = 768;

  // convert to big endian:
  for (int i = 0; i < 8; ++i) header[i]=htonl(header[i]);

  fwrite( header, sizeof(int), 8, fp );

  // Write colormap to file
  for (int c = 0; c < 3; ++c) // 3 iterations: red, green, blue
    for(int i = 0; i < 256; ++i)
      fputc((unsigned char) i, fp);

  // Write pixel data to file
  for(int k = 0; k < sizeX*sizeY; ++k)
    fputc(I2Char(data[k]), fp);

  fclose( fp );
}
