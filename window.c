#include "window.h"

#include "gamma.h"

#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h> // for conversion to big endian:


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
