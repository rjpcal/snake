
#include <stdio.h>

extern int   DISPLAY_X;
extern int   DISPLAY_Y;
extern int   FOREG_NUMBER;
extern int   PATCH_NUMBER;
extern int   BACKG_NUMBER;
extern int   TOTAL_NUMBER;
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
extern char  FILENAME[];

extern char  line[], text[];

extern void ReadParams( char extension[] );
extern void CheckParams( void );
extern void WriteParams( char extension[] );
extern void ListParams( void );
extern void Openfile( FILE **fp, char mode, char extension[] );
extern void Closefile( FILE *fp );
extern void PrintParams( void );
extern void date( char *p );
extern void process_id( char pid[] );
