#include <stdio.h>

struct Params
{
  Params(const char* fbase, const char* extension);

  const char* const pmFilestem;
  int   pmSizeX;
  int   pmSizeY;
  int   pmForegNumber;
  float pmForegSpacing;
  float pmForegEccentricity;    // FIXME unused
  float pmForegJitter;          // FIXME unused
  int   pmForegPositions;
  float pmForegDifference;
  float pmBackgIniSpacing;
  float pmBackgMinSpacing;
  float pmGaborPeriod;
  float pmGaborSigma;
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
