
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
  // let's clear our fake window:
  for (int i = 0; i < DISPLAY_X*DISPLAY_Y; ++i)
    wind->data[i]=Grey;

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

void Window2Raster(const FakeWindow* wind)
{
  FILE *fp;
  char fname[STRINGSIZE];
  unsigned char map[256];
  int i, k, x, y, dx, dy, size, header[8];
  Colorindex *ptr, *pt;
  unsigned char *ctr, *ct;

  static int count = 0;

  sprintf( fname, "%s_%d.ras", FILENAME, count++ );
  if( ( fp = fopen( fname, "w" ) ) == NULL )
    {
      printf( " %s: file not opened\n", fname );
    }

  header[0] = 0x59a66a95;
  header[1] = DISPLAY_X;
  header[2] = DISPLAY_Y;
  header[3] = 8;
  header[4] = DISPLAY_X * DISPLAY_Y;
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

  dx = DISPLAY_X;
  dy = DISPLAY_Y / 4;

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
          if (xx>=0 && xx<DISPLAY_X && yy>=0 && yy<DISPLAY_Y)
            ptr[xx - x + dx*(yy-y)] = wind->data[xx + yy*DISPLAY_X];

      pt = ptr;
      ct = ctr;

      for( k=0; k<size; k++ )
        *ct++ = I2Char( *pt++ );

      fwrite( ctr, sizeof(unsigned char), size, fp );
    }

  free( ptr );

  fclose( fp );
}
