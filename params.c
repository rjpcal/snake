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
  this->pmForegSpacing = getfloat(fp);
  this->pmForegEccentricity = getfloat(fp);
  this->pmForegJitter = getfloat(fp);
  this->pmForegPositions = getint(fp);
  this->pmForegDifference = getfloat(fp);
  this->pmBackgIniSpacing = getfloat(fp);
  this->pmBackgMinSpacing = getfloat(fp);
  this->pmGaborPeriod = getfloat(fp);
  this->pmGaborSigma = getfloat(fp);
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
  putfloat(fp, this->pmForegSpacing, "FOREG_SPACING");
  putfloat(fp, this->pmForegEccentricity, "FOREG_ECCENTRICITY");
  putfloat(fp, this->pmForegJitter, "FOREG_JITTER");
  putint(fp, this->pmForegPositions, "FOREG_POSITIONS");
  putfloat(fp, this->pmForegDifference, "FOREG_DIFFERENCE");
  putfloat(fp, this->pmBackgIniSpacing, "BACKG_INI_SPACING");
  putfloat(fp, this->pmBackgMinSpacing, "BACKG_MIN_SPACING");
  putfloat(fp, this->pmGaborPeriod, "GABOR_PERIOD");
  putfloat(fp, this->pmGaborSigma, "GABOR_SIGMA");
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
  putfloat(stdout, this->pmForegSpacing, "FOREG_SPACING");
  putfloat(stdout, this->pmForegEccentricity, "FOREG_ECCENTRICITY");
  putfloat(stdout, this->pmForegJitter, "FOREG_JITTER");
  putint(stdout, this->pmForegPositions, "FOREG_POSITIONS");
  putfloat(stdout, this->pmForegDifference, "FOREG_DIFFERENCE");
  putfloat(stdout, this->pmBackgIniSpacing, "BACKG_INI_SPACING");
  putfloat(stdout, this->pmBackgMinSpacing, "BACKG_MIN_SPACING");
  putfloat(stdout, this->pmGaborPeriod, "GABOR_PERIOD");
  putfloat(stdout, this->pmGaborSigma, "GABOR_SIGMA");
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
  putfloat(fp, this->pmGaborPeriod, "GABOR_PERIOD");
  putfloat(fp, this->pmGaborSigma, "GABOR_SIGMA");
  putint(fp, this->pmGaborSize, "GABOR_SIZE");
  putfloat(fp, this->pmForegEccentricity, "FOREG_ECCENTRICITY");
  putfloat(fp, this->pmForegJitter, "FOREG_JITTER");
  putint(fp, this->pmForegPositions, "FOREG_POSITIONS");
  putfloat(fp, this->pmForegDifference, "FOREG_DIFFERENCE");

  fclose(fp);
}
