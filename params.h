#include <stdio.h>

struct Params
{
  Params(const char* fbase, const char* extension);

  const char* const pmFilestem;
  int    pmSizeX;
  int    pmSizeY;
  int    pmForegNumber;
  double pmForegSpacing;
  double pmForegEccentricity;    // FIXME unused
  double pmForegJitter;          // FIXME unused
  int    pmForegPositions;
  double pmForegDifference;
  double pmBackgIniSpacing;
  double pmBackgMinSpacing;
  double pmGaborPeriod;
  double pmGaborSigma;
  int    pmGaborSize;
  int    pmDisplayNumber;

  void write(char extension[]) const;
  void print() const;
  void writeHeader() const;
};
