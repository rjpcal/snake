
#include "params.h"

#include "defs.h"
#include "gabor.h"
#include "ground.h"
#include "main.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

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

void Params::read(char extension[])
{
  int idummy;

  char fname[STRINGSIZE];
  sprintf( fname, "%s.%s", this->FILENAME, extension );

  FILE* fp = fopen(fname, "r");
  if (fp == 0)
    {
      printf("cannot read from %s\n", fname);
      exit(0);
    }

  if( fp == NULL )
    return;

  this->DISPLAY_X = getint(fp);
  this->DISPLAY_Y = getint(fp);
  this->FOREG_NUMBER = getint(fp);
  idummy = getint(fp);
  idummy = getint(fp);
  idummy = getint(fp);
  this->FOREG_SPACING = getfloat(fp);
  this->FOREG_ECCENTRICITY = getfloat(fp);
  this->FOREG_JITTER = getfloat(fp);
  this->FOREG_POSITIONS = getint(fp);
  this->FOREG_DIFFERENCE = getfloat(fp);
  this->BACKG_INI_SPACING = getfloat(fp);
  this->BACKG_MIN_SPACING = getfloat(fp);
  this->GABOR_PERIOD = getfloat(fp);
  this->GABOR_SIGMA = getfloat(fp);
  this->GABOR_SIZE = getint(fp);
  this->DISPLAY_NUMBER = getint(fp);
  // GETTEXT(  (sdummy) );

  fclose(fp);
}

void Params::write(char extension[]) const
{
  char fname[STRINGSIZE];
  sprintf( fname, "%s.%s", this->FILENAME, extension );

  FILE* fp = fopen(fname, "w");
  if (fp == 0)
    {
      printf("cannot write to %s\n", fname);
      exit(0);
    }

  putint(fp, this->DISPLAY_X, "DISPLAY_X");
  putint(fp, this->DISPLAY_Y, "DISPLAY_Y");
  putint(fp, this->FOREG_NUMBER, "FOREG_NUMBER");
  putint(fp, -1, "PATCH_NUMBER");
  putint(fp, -1, "BACKG_NUMBER");
  putint(fp, -1, "TOTAL_NUMBER");
  putfloat(fp, this->FOREG_SPACING, "FOREG_SPACING");
  putfloat(fp, this->FOREG_ECCENTRICITY, "FOREG_ECCENTRICITY");
  putfloat(fp, this->FOREG_JITTER, "FOREG_JITTER");
  putint(fp, this->FOREG_POSITIONS, "FOREG_POSITIONS");
  putfloat(fp, this->FOREG_DIFFERENCE, "FOREG_DIFFERENCE");
  putfloat(fp, this->BACKG_INI_SPACING, "BACKG_INI_SPACING");
  putfloat(fp, this->BACKG_MIN_SPACING, "BACKG_MIN_SPACING");
  putfloat(fp, this->GABOR_PERIOD, "GABOR_PERIOD");
  putfloat(fp, this->GABOR_SIGMA, "GABOR_SIGMA");
  putint(fp, this->GABOR_SIZE, "GABOR_SIZE");
  putint(fp, this->DISPLAY_NUMBER, "DISPLAY_NUMBER");
  puttext(fp, this->FILENAME, "FILENAME");

  fclose(fp);
}

void Params::print() const
{
  putint(stdout, this->DISPLAY_X, "DISPLAY_X");
  putint(stdout, this->DISPLAY_Y, "DISPLAY_Y");
  putint(stdout, this->FOREG_NUMBER, "FOREG_NUMBER");
  putfloat(stdout, this->FOREG_SPACING, "FOREG_SPACING");
  putfloat(stdout, this->FOREG_ECCENTRICITY, "FOREG_ECCENTRICITY");
  putfloat(stdout, this->FOREG_JITTER, "FOREG_JITTER");
  putint(stdout, this->FOREG_POSITIONS, "FOREG_POSITIONS");
  putfloat(stdout, this->FOREG_DIFFERENCE, "FOREG_DIFFERENCE");
  putfloat(stdout, this->BACKG_INI_SPACING, "BACKG_INI_SPACING");
  putfloat(stdout, this->BACKG_MIN_SPACING, "BACKG_MIN_SPACING");
  putfloat(stdout, this->GABOR_PERIOD, "GABOR_PERIOD");
  putfloat(stdout, this->GABOR_SIGMA, "GABOR_SIGMA");
  putint(stdout, this->GABOR_SIZE, "GABOR_SIZE");
  putint(stdout, this->DISPLAY_NUMBER, "DISPLAY_NUMBER");
  puttext(stdout, this->FILENAME, "FILENAME");
}

void Params::writeHeader() const
{
  char fname[STRINGSIZE];
  sprintf( fname, "%s.snk", this->FILENAME);

  FILE* fp = fopen(fname, "w");
  if (fp == 0)
    {
      printf("cannot write to %s\n", fname);
      exit(0);
    }

  putint(fp, DISPLAY_SET_NUMBER, "DISPLAY_SET_NUMBER");
  putint(fp, this->DISPLAY_NUMBER, "DISPLAY_NUMBER");
  putint(fp, this->DISPLAY_X, "DISPLAY_X");
  putint(fp, this->DISPLAY_Y, "DISPLAY_Y");
  putfloat(fp, this->GABOR_PERIOD, "GABOR_PERIOD");
  putfloat(fp, this->GABOR_SIGMA, "GABOR_SIGMA");
  putint(fp, this->GABOR_SIZE, "GABOR_SIZE");
  putfloat(fp, this->FOREG_ECCENTRICITY, "FOREG_ECCENTRICITY");
  putfloat(fp, this->FOREG_JITTER, "FOREG_JITTER");
  putint(fp, this->FOREG_POSITIONS, "FOREG_POSITIONS");
  putfloat(fp, this->FOREG_DIFFERENCE, "FOREG_DIFFERENCE");

  fclose(fp);
}

void WriteArray(const char* filestem, const Ground* g)
{
  int x, y, o, s;

  char fname[STRINGSIZE];
  sprintf( fname, "%s.snk", filestem);

  FILE* fp = fopen(fname, "a");
  if (fp == 0)
    {
      printf("cannot append to %s\n", fname);
      exit(0);
    }

  putint(fp, DISPLAY_COUNT, "DISPLAY_COUNT");
  putint(fp, g->totalNumber(), "TOTAL_NUMBER");
  putint(fp, g->getForegNumber(), "FOREG_NUMBER");
  putint(fp, g->getPatchNumber(), "PATCH_NUMBER");
  putfloat(fp, g->getForegSpacing(), "FOREG_SPACING");
  putfloat(fp, g->backgAveSpacing(), "BACKG_AVE_SPACING");
  putfloat(fp, g->getBackgIniSpacing(), "BACKG_INI_SPACING");
  putfloat(fp, g->getBackgMinSpacing(), "BACKG_MIN_SPACING");

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

  fclose(fp);
}
