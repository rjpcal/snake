
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

  const int* px             = g->XPatch;
  const int* py             = g->YPatch;
  Colorindex* const* src    = g->PPatch;

  for(int i = 0; i < g->NPatch; ++i, ++px, ++py, ++src)
    {
      int xbotleft  = *px - GABOR_SIZE / 2;
      int ybotleft  = *py - GABOR_SIZE / 2;
      int xtopright = xbotleft + GABOR_SIZE - 1;
      int ytopright = ybotleft + GABOR_SIZE - 1;

      for (int yy = ybotleft; yy <= ytopright; ++yy)
        for (int xx = xbotleft; xx <= xtopright; ++xx)
          if (xx>=0 && xx<DISPLAY_X && yy>=0 && yy<DISPLAY_Y)
            {
              if (fabs(wind->data[xx+yy*DISPLAY_X] - Grey) < fabs((*src)[xx-xbotleft+(yy-ybotleft)*(xtopright-xbotleft+1)] - Grey))
                wind->data[xx+yy*DISPLAY_X]=((*src)[xx-xbotleft+(yy-ybotleft)*(xtopright-xbotleft+1)]);
            }
    }
}

void FakeWindow::writeRaster(const char* fname) const
{
  FILE* fp = fopen( fname, "w" );
  if( fp == NULL )
    {
      printf( " %s: file not opened\n", fname );
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

  unsigned char cmap[256];

  for(int i = 0; i < 256; ++i)
    cmap[i] = (unsigned char) i;

  fwrite( cmap, sizeof(unsigned char), 256, fp );
  fwrite( cmap, sizeof(unsigned char), 256, fp );
  fwrite( cmap, sizeof(unsigned char), 256, fp );

  const int dx = sizeX;
  const int dy = sizeY / 4;

  const int size = dx*dy;

  Colorindex* ptr = new Colorindex[dx*dy];
  unsigned char* ctr = new unsigned char[dx*dy];

  for(int i = 0; i < 4; ++i)
    {
      const int x = 0;
      const int y = i*dy;

      for (int yy = y; yy <= y+dy-1; ++yy)
        for (int xx = x; xx <= x+dx-1; ++xx)
          if (xx>=0 && xx<sizeX && yy>=0 && yy<sizeY)
            ptr[xx - x + dx*(yy-y)] = data[xx + yy*sizeX];

      Colorindex* pt = ptr;
      unsigned char* ct = ctr;

      for(int k = 0; k < size; ++k)
        *ct++ = I2Char( *pt++ );

      fwrite( ctr, sizeof(unsigned char), size, fp );
    }

  delete [] ctr;
  delete [] ptr;

  fclose( fp );
}
