#ifndef DEFS_H_DEFINED
#define DEFS_H_DEFINED

#include <math.h>

typedef int Colorindex;
typedef struct { unsigned char r, g, b; } cmapval;
extern Colorindex *win;

#define PI                  M_PI
#define TWOPI               (2*M_PI)
#define SQRT3               1.7320508075
#define RAD2DEG             (180./M_PI)

#define STRINGSIZE           120

#define MAX_GABOR_NUMBER    1800
#define GABOR_MAX_ORIENT      64
#define GABOR_MAX_PHASE        8

#define MAX_DISPLAY_NUMBER  1000
#define MAX_FOREG_NUMBER     128

#endif // !DEFS_H_DEFINED
