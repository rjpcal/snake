
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

#define GETINT(name) name = getint(fp);
#define GETFLOAT(name) name = getfloat(fp);

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

#define PUTINT(name,text) putint(fp, name, text);
#define PUTFLOAT(name,text) putfloat(fp, name, text);
#define PUTTEXT(name,text) puttext(fp, name, text);

#define PRINTINT(name,text) putint(stdout, name, text);
#define PRINTFLOAT(name,text) putfloat(stdout, name, text);
#define PRINTTEXT(name,text) puttext(stdout, name, text);
}

void ReadParams( char extension[] )
{
  FILE* fp;
  int idummy;

  Openfile( &fp, READ, extension );

  if( fp == NULL )
    return;

  GETINT(   (PM.DISPLAY_X) );
  GETINT(   (PM.DISPLAY_Y) );
  GETINT(   (PM.FOREG_NUMBER) );
  GETINT(   (idummy) );
  GETINT(   (idummy) );
  GETINT(   (idummy) );
  GETFLOAT( (PM.FOREG_SPACING) );
  GETFLOAT( (PM.FOREG_ECCENTRICITY) );
  GETFLOAT( (PM.FOREG_JITTER) );
  GETINT(   (PM.FOREG_POSITIONS) );
  GETFLOAT( (PM.FOREG_DIFFERENCE) );
  GETFLOAT( (PM.BACKG_INI_SPACING) );
  GETFLOAT( (PM.BACKG_MIN_SPACING) );
  GETFLOAT( (PM.GABOR_PERIOD) );
  GETFLOAT( (PM.GABOR_SIGMA) );
  GETINT(   (PM.GABOR_SIZE) );
  GETINT(   (PM.DISPLAY_NUMBER) );
  // GETTEXT(  (sdummy) );

  Closefile( fp );
}

void WriteParams(char extension[])
{
  FILE *fp;

  Openfile( &fp, WRITE, extension );

  PUTINT(   (PM.DISPLAY_X),          ("DISPLAY_X") );
  PUTINT(   (PM.DISPLAY_Y),          ("DISPLAY_Y") );
  PUTINT(   (PM.FOREG_NUMBER) ,      ("FOREG_NUMBER") );
  PUTINT(   (-1),                    ("PATCH_NUMBER") );
  PUTINT(   (-1),                    ("BACKG_NUMBER") );
  PUTINT(   (-1),                    ("TOTAL_NUMBER") );
  PUTFLOAT( (PM.FOREG_SPACING) ,     ("FOREG_SPACING") );
  PUTFLOAT( (PM.FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
  PUTFLOAT( (PM.FOREG_JITTER),       ("FOREG_JITTER") );
  PUTINT(   (PM.FOREG_POSITIONS),    ("FOREG_POSITIONS") );
  PUTFLOAT( (PM.FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );
  PUTFLOAT( (PM.BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
  PUTFLOAT( (PM.BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );
  PUTFLOAT( (PM.GABOR_PERIOD),       ("GABOR_PERIOD") );
  PUTFLOAT( (PM.GABOR_SIGMA),        ("GABOR_SIGMA") );
  PUTINT(   (PM.GABOR_SIZE),         ("GABOR_SIZE") );
  PUTINT(   (PM.DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );
  PUTTEXT(  (PM.FILENAME),           ("FILENAME") );

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
  PRINTINT(   (PM.DISPLAY_X),          ("DISPLAY_X") );
  PRINTINT(   (PM.DISPLAY_Y),          ("DISPLAY_Y") );
  PRINTINT(   (PM.FOREG_NUMBER) ,      ("FOREG_NUMBER") );
  PRINTFLOAT( (PM.FOREG_SPACING) ,     ("FOREG_SPACING") );
  PRINTFLOAT( (PM.FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
  PRINTFLOAT( (PM.FOREG_JITTER),       ("FOREG_JITTER") );
  PRINTINT(   (PM.FOREG_POSITIONS),    ("FOREG_POSITIONS") );
  PRINTFLOAT( (PM.FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );
  PRINTFLOAT( (PM.BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
  PRINTFLOAT( (PM.BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );
  PRINTFLOAT( (PM.GABOR_PERIOD),       ("GABOR_PERIOD") );
  PRINTFLOAT( (PM.GABOR_SIGMA),        ("GABOR_SIGMA") );
  PRINTINT(   (PM.GABOR_SIZE),         ("GABOR_SIZE") );
  PRINTINT(   (PM.DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );
  PRINTTEXT(  (PM.FILENAME),           ("FILENAME") );
}

void WriteHeader( void )
{
  FILE *fp;

  Openfile( &fp, WRITE, "snk" );

  PUTINT(   (DISPLAY_SET_NUMBER), ("DISPLAY_SET_NUMBER") );
  PUTINT(   (PM.DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );
  PUTINT(   (PM.DISPLAY_X),          ("DISPLAY_X") );
  PUTINT(   (PM.DISPLAY_Y),          ("DISPLAY_Y") );
  PUTFLOAT( (PM.GABOR_PERIOD),       ("GABOR_PERIOD") );
  PUTFLOAT( (PM.GABOR_SIGMA),        ("GABOR_SIGMA") );
  PUTINT(   (PM.GABOR_SIZE),         ("GABOR_SIZE") );
  PUTFLOAT( (PM.FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
  PUTFLOAT( (PM.FOREG_JITTER),       ("FOREG_JITTER") );
  PUTINT(   (PM.FOREG_POSITIONS),    ("FOREG_POSITIONS") );
  PUTFLOAT( (PM.FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );

  Closefile( fp );
}

void WriteArray(const Ground* g)
{
  int x, y, o, s;
  FILE *fp;

  Openfile( &fp, APPEND, "snk" );

  PUTINT(   (DISPLAY_COUNT),         ("DISPLAY_COUNT") );
  PUTINT(   (g->totalNumber()),      ("TOTAL_NUMBER") );
  PUTINT(   (PM.FOREG_NUMBER),       ("FOREG_NUMBER") );
  PUTINT(   (g->getPatchNumber()),   ("PATCH_NUMBER") );
  PUTFLOAT( (PM.FOREG_SPACING) ,     ("FOREG_SPACING") );
  PUTFLOAT( (g->backgAveSpacing()),  ("BACKG_AVE_SPACING") );
  PUTFLOAT( (PM.BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
  PUTFLOAT( (PM.BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );

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
