
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
  FILE *fp;
  unsigned char map[256];
  int i, k, x, y, dx, dy, size, header[8];
  Colorindex *ptr, *pt;
  unsigned char *ctr, *ct;

  if( ( fp = fopen( fname, "w" ) ) == NULL )
    {
      printf( " %s: file not opened\n", fname );
    }

  header[0] = 0x59a66a95;
  header[1] = sizeX;
  header[2] = sizeY;
  header[3] = 8;
  header[4] = sizeX * sizeY;
  header[5] = 1;
  header[6] = 1;
  header[7] = 768;

  //convert to big endian:
  for (i=0;i<8;i++) header[i]=htonl(header[i]);

  fwrite( header, sizeof(int), 8, fp );

  for( i=0; i<256; i++ )
    map[i] = (unsigned char) i;

  fwrite( map, sizeof(unsigned char), 256, fp );
  fwrite( map, sizeof(unsigned char), 256, fp );
  fwrite( map, sizeof(unsigned char), 256, fp );

  dx = sizeX;
  dy = sizeY / 4;

  size = dx*dy;

  ptr = (Colorindex*) malloc( dx*dy*sizeof(Colorindex) );
  ctr = (unsigned char*) malloc( dx*dy*sizeof(unsigned char) );

  for( i=0; i<4; i++ )
    {
      x  = 0;
      y  = i*dy;

      int xx,yy;
      for (yy = y; yy <= y+dy-1; ++yy)
        for (xx = x; xx <= x+dx-1; ++xx)
          if (xx>=0 && xx<sizeX && yy>=0 && yy<sizeY)
            ptr[xx - x + dx*(yy-y)] = data[xx + yy*sizeX];

      pt = ptr;
      ct = ctr;

      for( k=0; k<size; k++ )
        *ct++ = I2Char( *pt++ );

      fwrite( ctr, sizeof(unsigned char), size, fp );
    }

  free( ptr );

  fclose( fp );
}
