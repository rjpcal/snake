
#include <stdio.h>

struct Params
{
  int   DISPLAY_X;
  int   DISPLAY_Y;
  int   FOREG_NUMBER;
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
  const char* FILENAME;

  void read(char extension[]);
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
