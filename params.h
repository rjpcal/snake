
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

#define      READ                  'r'
#define      WRITE                 'w'
#define      APPEND                'a'
#define      OPTIONAL              'o'

class Ground;

extern void WriteArray(const char* filestem, const Ground* g);
