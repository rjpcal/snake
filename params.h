
#include <stdio.h>

extern int   DISPLAY_X;
extern int   DISPLAY_Y;
extern int   FOREG_NUMBER;
extern int   PATCH_NUMBER;
extern int   BACKG_NUMBER;
extern float FOREG_SPACING;
extern float FOREG_ECCENTRICITY;
extern float FOREG_JITTER;
extern int   FOREG_POSITIONS;
extern float FOREG_DIFFERENCE;
extern float BACKG_INI_SPACING;
extern float BACKG_MIN_SPACING;
extern float GABOR_PERIOD;
extern float GABOR_SIGMA;
extern int   GABOR_SIZE;
extern int   DISPLAY_NUMBER;
extern const char* FILENAME;

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
