
#include "params.h"

#include "defs.h"
#include "gabor.h"
#include "ground.h"
#include "main.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

Params PM;

namespace
{
  char line[STRINGSIZE], text[STRINGSIZE];

  const double RAD2DEG = (180./M_PI);

  inline int getint(FILE* fp)
  {
    int result = -1;
    fgets(line,120,fp);
    sscanf(line,"%s %d",text,&result);
    return result;
  }

  inline float getfloat(FILE* fp)
  {
    float result = -1.0;
    fgets(line,120,fp);
    sscanf(line,"%s %f",text,&result);
    return result;
  }

  inline void putint(FILE* fp, int val, const char* name)
  {
    fprintf(fp, "%-19s %d\n", name, val);
  }

  inline void putfloat(FILE* fp, float val, const char* name)
  {
    fprintf(fp, "%-19s %.2f\n", name, val);
  }

  inline void puttext(FILE* fp, const char* val, const char* name)
  {
    fprintf(fp, "%-19s %s\n", name, val);
  }
}

void ReadParams( char extension[] )
{
  FILE* fp;
  int idummy;

  Openfile( &fp, READ, extension );

  if( fp == NULL )
    return;

  PM.DISPLAY_X = getint(fp);
  PM.DISPLAY_Y = getint(fp);
  PM.FOREG_NUMBER = getint(fp);
  idummy = getint(fp);
  idummy = getint(fp);
  idummy = getint(fp);
  PM.FOREG_SPACING = getfloat(fp);
  PM.FOREG_ECCENTRICITY = getfloat(fp);
  PM.FOREG_JITTER = getfloat(fp);
  PM.FOREG_POSITIONS = getint(fp);
  PM.FOREG_DIFFERENCE = getfloat(fp);
  PM.BACKG_INI_SPACING = getfloat(fp);
  PM.BACKG_MIN_SPACING = getfloat(fp);
  PM.GABOR_PERIOD = getfloat(fp);
  PM.GABOR_SIGMA = getfloat(fp);
  PM.GABOR_SIZE = getint(fp);
  PM.DISPLAY_NUMBER = getint(fp);
  // GETTEXT(  (sdummy) );

  Closefile( fp );
}

void WriteParams(char extension[])
{
  FILE *fp;

  Openfile( &fp, WRITE, extension );

  putint(fp, PM.DISPLAY_X, "DISPLAY_X");
  putint(fp, PM.DISPLAY_Y, "DISPLAY_Y");
  putint(fp, PM.FOREG_NUMBER, "FOREG_NUMBER");
  putint(fp, -1, "PATCH_NUMBER");
  putint(fp, -1, "BACKG_NUMBER");
  putint(fp, -1, "TOTAL_NUMBER");
  putfloat(fp, PM.FOREG_SPACING, "FOREG_SPACING");
  putfloat(fp, PM.FOREG_ECCENTRICITY, "FOREG_ECCENTRICITY");
  putfloat(fp, PM.FOREG_JITTER, "FOREG_JITTER");
  putint(fp, PM.FOREG_POSITIONS, "FOREG_POSITIONS");
  putfloat(fp, PM.FOREG_DIFFERENCE, "FOREG_DIFFERENCE");
  putfloat(fp, PM.BACKG_INI_SPACING, "BACKG_INI_SPACING");
  putfloat(fp, PM.BACKG_MIN_SPACING, "BACKG_MIN_SPACING");
  putfloat(fp, PM.GABOR_PERIOD, "GABOR_PERIOD");
  putfloat(fp, PM.GABOR_SIGMA, "GABOR_SIGMA");
  putint(fp, PM.GABOR_SIZE, "GABOR_SIZE");
  putint(fp, PM.DISPLAY_NUMBER, "DISPLAY_NUMBER");
  puttext(fp, PM.FILENAME, "FILENAME");

  Closefile( fp );
}

void Openfile( FILE **fp, char mode, char extension[] )
{
  char fname[STRINGSIZE];

  sprintf( fname, "%s.%s", PM.FILENAME, extension );

  if( mode==WRITE )
    {
      if( ( *fp = fopen( fname, "w") ) == NULL )
        {
          printf( "cannot write %s\n", fname);
          Exit();
        }
    }
  else
    if( mode==APPEND )
      {
        if( ( *fp = fopen( fname, "a") ) == NULL )
          {
            printf( "cannot append to %s\n", fname);
            Exit();
          }
      }
    else
      if( mode==READ )
        {
          if( ( *fp = fopen( fname, "r") ) == NULL )
            {
              printf( "cannot read from %s\n", fname );
              Exit();
            }
        }

  chmod( fname, 0666 );
}

void Closefile( FILE *fp )
{
  if( fp != NULL)
    fclose( fp);
}

void PrintParams()
{
  putint(stdout, PM.DISPLAY_X, "DISPLAY_X");
  putint(stdout, PM.DISPLAY_Y, "DISPLAY_Y");
  putint(stdout, PM.FOREG_NUMBER, "FOREG_NUMBER");
  putfloat(stdout, PM.FOREG_SPACING, "FOREG_SPACING");
  putfloat(stdout, PM.FOREG_ECCENTRICITY, "FOREG_ECCENTRICITY");
  putfloat(stdout, PM.FOREG_JITTER, "FOREG_JITTER");
  putint(stdout, PM.FOREG_POSITIONS, "FOREG_POSITIONS");
  putfloat(stdout, PM.FOREG_DIFFERENCE, "FOREG_DIFFERENCE");
  putfloat(stdout, PM.BACKG_INI_SPACING, "BACKG_INI_SPACING");
  putfloat(stdout, PM.BACKG_MIN_SPACING, "BACKG_MIN_SPACING");
  putfloat(stdout, PM.GABOR_PERIOD, "GABOR_PERIOD");
  putfloat(stdout, PM.GABOR_SIGMA, "GABOR_SIGMA");
  putint(stdout, PM.GABOR_SIZE, "GABOR_SIZE");
  putint(stdout, PM.DISPLAY_NUMBER, "DISPLAY_NUMBER");
  puttext(stdout, PM.FILENAME, "FILENAME");
}

void WriteHeader( void )
{
  FILE *fp;

  Openfile( &fp, WRITE, "snk" );

  putint(fp, DISPLAY_SET_NUMBER, "DISPLAY_SET_NUMBER");
  putint(fp, PM.DISPLAY_NUMBER, "DISPLAY_NUMBER");
  putint(fp, PM.DISPLAY_X, "DISPLAY_X");
  putint(fp, PM.DISPLAY_Y, "DISPLAY_Y");
  putfloat(fp, PM.GABOR_PERIOD, "GABOR_PERIOD");
  putfloat(fp, PM.GABOR_SIGMA, "GABOR_SIGMA");
  putint(fp, PM.GABOR_SIZE, "GABOR_SIZE");
  putfloat(fp, PM.FOREG_ECCENTRICITY, "FOREG_ECCENTRICITY");
  putfloat(fp, PM.FOREG_JITTER, "FOREG_JITTER");
  putint(fp, PM.FOREG_POSITIONS, "FOREG_POSITIONS");
  putfloat(fp, PM.FOREG_DIFFERENCE, "FOREG_DIFFERENCE");

  Closefile( fp );
}

void WriteArray(const Ground* g)
{
  int x, y, o, s;
  FILE *fp;

  Openfile( &fp, APPEND, "snk" );

  putint(fp, DISPLAY_COUNT, "DISPLAY_COUNT");
  putint(fp, g->totalNumber(), "TOTAL_NUMBER");
  putint(fp, PM.FOREG_NUMBER, "FOREG_NUMBER");
  putint(fp, g->getPatchNumber(), "PATCH_NUMBER");
  putfloat(fp, PM.FOREG_SPACING, "FOREG_SPACING");
  putfloat(fp, g->backgAveSpacing(), "BACKG_AVE_SPACING");
  putfloat(fp, PM.BACKG_INI_SPACING, "BACKG_INI_SPACING");
  putfloat(fp, PM.BACKG_MIN_SPACING, "BACKG_MIN_SPACING");

  for(int i = 0; i < g->totalNumber(); ++i)
    {
      if( g->flag(i) )
        {
          o = (int)( RAD2DEG * g->theta(i) + 0.5 );

          x = (int)( g->xpos(i) + 0.5 );

          y = (int)( g->ypos(i) + 0.5 );

          s = g->flag(i);

          fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

  for(int i = 0; i < g->totalNumber(); ++i)
    {
      if( !g->flag(i) )
        {
          o = (int)( RAD2DEG * g->theta(i) + 0.5 );

          x = (int)( g->xpos(i) + 0.5 );

          y = (int)( g->ypos(i) + 0.5 );

          s = g->flag(i);

          fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

  Closefile( fp );
}
