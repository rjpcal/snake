#ifndef DEFS_H_DEFINED
#define DEFS_H_DEFINED

#include <math.h>

typedef int Colorindex;
typedef struct { unsigned char r, g, b; } cmapval;

#define mapcolor(ci,cr,cg,cb) colormap[ci].r=cr;colormap[ci].g=cg;\
colormap[ci].b=cb;
extern Colorindex *win;

#define rectread(xbleft,ybleft,xtright,ytright,ds) \
/*printf("rr: (%d,%d) - (%d,%d)\n",xbleft,ybleft,xtright,ytright);*/ \
{ int xx,yy; for (yy=ybleft;yy<=ytright;yy++) \
for (xx=xbleft;xx<=xtright;xx++) \
if (xx>=0 && xx<DISPLAY_X && yy>=0 && yy<DISPLAY_Y) \
(ds)[xx-xbleft+(yy-ybleft)*(xtright-xbleft+1)]=win[xx+yy*DISPLAY_X]; }

#define PI                  M_PI
#define TWOPI               (2*M_PI)
#define HALFPI              M_PI_2
#define FOURPI              (4*M_PI)
#define SQRT2               M_SQRT2
#define SQRT3               1.7320508075
#define DEG2RAD             (M_PI/180.)
#define RAD2DEG             (180./M_PI)

#define STRINGSIZE           120

#define MAX_GABOR_NUMBER    1800
#define GABOR_MAX_ORIENT      64
#define GABOR_MAX_PHASE        8

#define MAX_DISPLAY_NUMBER  1000
#define MAX_FOREG_NUMBER     128

#define DONE(name) {printf( "%s\n",name); fflush(stdout);}

#endif // !DEFS_H_DEFINED
