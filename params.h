#include <stdio.h>

struct Params
{
  Params(const char* fbase, const char* extension);

  const char* const pmFilestem;
  int   pmSizeX;
  int   pmSizeY;
  int   pmForegNumber;
  double pmForegSpacing;
  double pmForegEccentricity;    // FIXME unused
  double pmForegJitter;          // FIXME unused
  int   pmForegPositions;
  double pmForegDifference;
  double pmBackgIniSpacing;
  double pmBackgMinSpacing;
  double pmGaborPeriod;
  double pmGaborSigma;
  int   pmGaborSize;
  int   pmDisplayNumber;

  void write(char extension[]) const;
  void print() const;
  void writeHeader() const;
};

namespace
{
  const int STRINGSIZE = 120;

  char line[STRINGSIZE], text[STRINGSIZE];

  inline int getint(FILE* fp)
  {
    int result = -1;
    fgets(line, 120, fp);
    sscanf(line, "%s %d", text, &result);
    return result;
  }

  inline double getdouble(FILE* fp)
  {
    double result = -1.0;
    fgets(line, 120, fp);
    sscanf(line, "%s %lf", text, &result);
    return result;
  }

  inline void putint(FILE* fp, int val, const char* name)
  {
    fprintf(fp, "%-19s %d\n", name, val);
  }

  inline void putdouble(FILE* fp, double val, const char* name)
  {
    fprintf(fp, "%-19s %.2f\n", name, val);
  }

  inline void puttext(FILE* fp, const char* val, const char* name)
  {
    fprintf(fp, "%-19s %s\n", name, val);
  }
}
