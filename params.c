#include "params.h"

#include "gabor.h"
#include "ground.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

Params::Params(const char* fbase, const char* extension) :
  pmFilestem(fbase)
{
  int idummy;

  char fname[STRINGSIZE];
  sprintf( fname, "%s.%s", this->pmFilestem, extension );

  FILE* fp = fopen(fname, "r");
  if (fp == 0)
    {
      printf("cannot read from %s\n", fname);
      exit(0);
    }

  if( fp == NULL )
    return;

  this->pmSizeX = getint(fp);
  this->pmSizeY = getint(fp);
  this->pmForegNumber = getint(fp);
  idummy = getint(fp);
  idummy = getint(fp);
  idummy = getint(fp);
  this->pmForegSpacing = getdouble(fp);
  this->pmForegEccentricity = getdouble(fp);
  this->pmForegJitter = getdouble(fp);
  this->pmForegPositions = getint(fp);
  this->pmForegDifference = getdouble(fp);
  this->pmBackgIniSpacing = getdouble(fp);
  this->pmBackgMinSpacing = getdouble(fp);
  this->pmGaborPeriod = getdouble(fp);
  this->pmGaborSigma = getdouble(fp);
  this->pmGaborSize = getint(fp);
  this->pmDisplayNumber = getint(fp);
  // GETTEXT(  (sdummy) );

  fclose(fp);
}

void Params::write(char extension[]) const
{
  char fname[STRINGSIZE];
  sprintf( fname, "%s.%s", this->pmFilestem, extension );

  FILE* fp = fopen(fname, "w");
  if (fp == 0)
    {
      printf("cannot write to %s\n", fname);
      exit(0);
    }

  putint(fp, this->pmSizeX, "DISPLAY_X");
  putint(fp, this->pmSizeY, "DISPLAY_Y");
  putint(fp, this->pmForegNumber, "FOREG_NUMBER");
  putint(fp, -1, "PATCH_NUMBER");
  putint(fp, -1, "BACKG_NUMBER");
  putint(fp, -1, "TOTAL_NUMBER");
  putdouble(fp, this->pmForegSpacing, "FOREG_SPACING");
  putdouble(fp, this->pmForegEccentricity, "FOREG_ECCENTRICITY");
  putdouble(fp, this->pmForegJitter, "FOREG_JITTER");
  putint(fp, this->pmForegPositions, "FOREG_POSITIONS");
  putdouble(fp, this->pmForegDifference, "FOREG_DIFFERENCE");
  putdouble(fp, this->pmBackgIniSpacing, "BACKG_INI_SPACING");
  putdouble(fp, this->pmBackgMinSpacing, "BACKG_MIN_SPACING");
  putdouble(fp, this->pmGaborPeriod, "GABOR_PERIOD");
  putdouble(fp, this->pmGaborSigma, "GABOR_SIGMA");
  putint(fp, this->pmGaborSize, "GABOR_SIZE");
  putint(fp, this->pmDisplayNumber, "DISPLAY_NUMBER");
  puttext(fp, this->pmFilestem, "FILENAME");

  fclose(fp);
}

void Params::print() const
{
  putint(stdout, this->pmSizeX, "DISPLAY_X");
  putint(stdout, this->pmSizeY, "DISPLAY_Y");
  putint(stdout, this->pmForegNumber, "FOREG_NUMBER");
  putdouble(stdout, this->pmForegSpacing, "FOREG_SPACING");
  putdouble(stdout, this->pmForegEccentricity, "FOREG_ECCENTRICITY");
  putdouble(stdout, this->pmForegJitter, "FOREG_JITTER");
  putint(stdout, this->pmForegPositions, "FOREG_POSITIONS");
  putdouble(stdout, this->pmForegDifference, "FOREG_DIFFERENCE");
  putdouble(stdout, this->pmBackgIniSpacing, "BACKG_INI_SPACING");
  putdouble(stdout, this->pmBackgMinSpacing, "BACKG_MIN_SPACING");
  putdouble(stdout, this->pmGaborPeriod, "GABOR_PERIOD");
  putdouble(stdout, this->pmGaborSigma, "GABOR_SIGMA");
  putint(stdout, this->pmGaborSize, "GABOR_SIZE");
  putint(stdout, this->pmDisplayNumber, "DISPLAY_NUMBER");
  puttext(stdout, this->pmFilestem, "FILENAME");
}

void Params::writeHeader() const
{
  char fname[STRINGSIZE];
  sprintf( fname, "%s.snk", this->pmFilestem);

  FILE* fp = fopen(fname, "w");
  if (fp == 0)
    {
      printf("cannot write to %s\n", fname);
      exit(0);
    }

  putint(fp, this->pmDisplayNumber, "DISPLAY_NUMBER");
  putint(fp, this->pmSizeX, "DISPLAY_X");
  putint(fp, this->pmSizeY, "DISPLAY_Y");
  putdouble(fp, this->pmGaborPeriod, "GABOR_PERIOD");
  putdouble(fp, this->pmGaborSigma, "GABOR_SIGMA");
  putint(fp, this->pmGaborSize, "GABOR_SIZE");
  putdouble(fp, this->pmForegEccentricity, "FOREG_ECCENTRICITY");
  putdouble(fp, this->pmForegJitter, "FOREG_JITTER");
  putint(fp, this->pmForegPositions, "FOREG_POSITIONS");
  putdouble(fp, this->pmForegDifference, "FOREG_DIFFERENCE");

  fclose(fp);
}
