
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
};

extern Params PM;

#define      READ                  'r'
#define      WRITE                 'w'
#define      APPEND                'a'
#define      OPTIONAL              'o'

#define GETINT(name) {fgets(line,120,fp);sscanf(line,"%s %d",text,&(name));}
#define GETFLOAT(name) {fgets(line,120,fp);sscanf(line,"%s %f",text,&(name));}
#define GETTEXT(name) {fgets(line,120,fp);sscanf(line,"%s %s",text,(name));}

#define PUTINT(name,text) {fprintf(fp,"%-19s %d\n",(text),(name));}
#define PUTFLOAT(name,text) {fprintf(fp,"%-19s %.2f\n",(text),(name));}
#define PUTTEXT(name,text) {fprintf(fp,"%-19s %s\n",(text),(name));}

#define PRINTINT(name,text) {printf( "%-19s %d\n",(text),(name));}
#define PRINTFLOAT(name,text) {printf( "%-19s %.2f\n",(text),(name));}
#define PRINTTEXT(name,text) {printf( "%-19s %s\n",(text),(name));}

class Ground;

extern void ReadParams( char extension[] );
extern void WriteParams(char extension[]);
extern void Openfile( FILE **fp, char mode, char extension[] );
extern void Closefile( FILE *fp );
extern void PrintParams();
extern void WriteHeader( void );
extern void WriteArray(const Ground* g);
