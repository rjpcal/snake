
#include "params.h"

#include "defs.h"
#include "gabor.h"
#include "main.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int   DISPLAY_X;
int   DISPLAY_Y;
int   FOREG_NUMBER;
int   PATCH_NUMBER;
int   BACKG_NUMBER;
int   TOTAL_NUMBER;
float FOREG_SPACING;
float FOREG_ECCENTRICITY;
float FOREG_JITTER;
int   FOREG_POSITIONS;
float FOREG_DIFFERENCE;
float BACKG_INI_SPACING;
float BACKG_MIN_SPACING;
float GABOR_PERIOD;
float GABOR_SIGMA;
int   GABOR_SIZE;
int   DISPLAY_NUMBER;
const char* FILENAME = "nofile";

namespace
{
  char line[STRINGSIZE], text[STRINGSIZE];
}

void ReadParams( char extension[] )
{
  FILE* fp;
  char sdummy[STRINGSIZE];
  int idummy;

  Openfile( &fp, READ, extension );

  if( fp == NULL )
    return;

  GETINT(   (DISPLAY_X) );
  GETINT(   (DISPLAY_Y) );
  GETINT(   (FOREG_NUMBER) );
  GETINT(   (idummy) );
  GETINT(   (idummy) );
  GETINT(   (idummy) );
  GETFLOAT( (FOREG_SPACING) );
  GETFLOAT( (FOREG_ECCENTRICITY) );
  GETFLOAT( (FOREG_JITTER) );
  GETINT(   (FOREG_POSITIONS) );
  GETFLOAT( (FOREG_DIFFERENCE) );
  GETFLOAT( (BACKG_INI_SPACING) );
  GETFLOAT( (BACKG_MIN_SPACING) );
  GETFLOAT( (GABOR_PERIOD) );
  GETFLOAT( (GABOR_SIGMA) );
  GETINT(   (GABOR_SIZE) );
  GETINT(   (DISPLAY_NUMBER) );
  GETTEXT(  (sdummy) );

  Closefile( fp );
}

void WriteParams( char extension[] )
{
  FILE *fp;

  Openfile( &fp, WRITE, extension );

  PUTINT(   (DISPLAY_X),          ("DISPLAY_X") );
  PUTINT(   (DISPLAY_Y),          ("DISPLAY_Y") );
  PUTINT(   (FOREG_NUMBER) ,      ("FOREG_NUMBER") );
  PUTINT(   (PATCH_NUMBER) ,      ("PATCH_NUMBER") );
  PUTINT(   (BACKG_NUMBER),       ("BACKG_NUMBER") );
  PUTINT(   (TOTAL_NUMBER),       ("TOTAL_NUMBER") );
  PUTFLOAT( (FOREG_SPACING) ,     ("FOREG_SPACING") );
  PUTFLOAT( (FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
  PUTFLOAT( (FOREG_JITTER),       ("FOREG_JITTER") );
  PUTINT(   (FOREG_POSITIONS),    ("FOREG_POSITIONS") );
  PUTFLOAT( (FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );
  PUTFLOAT( (BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
  PUTFLOAT( (BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );
  PUTFLOAT( (GABOR_PERIOD),       ("GABOR_PERIOD") );
  PUTFLOAT( (GABOR_SIGMA),        ("GABOR_SIGMA") );
  PUTINT(   (GABOR_SIZE),         ("GABOR_SIZE") );
  PUTINT(   (DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );
  PUTTEXT(  (FILENAME),           ("FILENAME") );

  Closefile( fp );
}

void Openfile( FILE **fp, char mode, char extension[] )
{
  char fname[STRINGSIZE];

  sprintf( fname, "%s.%s", FILENAME, extension );

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

void PrintParams( void )
{
  PRINTINT(   (DISPLAY_X),          ("DISPLAY_X") );
  PRINTINT(   (DISPLAY_Y),          ("DISPLAY_Y") );
  PRINTINT(   (FOREG_NUMBER) ,      ("FOREG_NUMBER") );
  PRINTINT(   (PATCH_NUMBER) ,      ("PATCH_NUMBER") );
  PRINTINT(   (BACKG_NUMBER),       ("BACKG_NUMBER") );
  PRINTINT(   (TOTAL_NUMBER),       ("TOTAL_NUMBER") );
  PRINTFLOAT( (FOREG_SPACING) ,     ("FOREG_SPACING") );
  PRINTFLOAT( (FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
  PRINTFLOAT( (FOREG_JITTER),       ("FOREG_JITTER") );
  PRINTINT(   (FOREG_POSITIONS),    ("FOREG_POSITIONS") );
  PRINTFLOAT( (FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );
  PRINTFLOAT( (BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
  PRINTFLOAT( (BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );
  PRINTFLOAT( (GABOR_PERIOD),       ("GABOR_PERIOD") );
  PRINTFLOAT( (GABOR_SIGMA),        ("GABOR_SIGMA") );
  PRINTINT(   (GABOR_SIZE),         ("GABOR_SIZE") );
  PRINTINT(   (DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );
  PRINTTEXT(  (FILENAME),           ("FILENAME") );
}

