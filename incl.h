
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <math.h>

///#include <gl/gl.h>
///#include <gl/device.h>
///#include <fmclient.h>

/// new types:
//typedef unsigned int Colorindex;
typedef int Colorindex;
typedef struct { unsigned char r, g, b; } cmapval;

//1 for snake only 0 for regular
#define BLANK_SNAKE         0

#define mapcolor(ci,cr,cg,cb) colormap[ci].r=cr;colormap[ci].g=cg;\
colormap[ci].b=cb;
extern Colorindex *win;

#define rectwrite(xbleft,ybleft,xtright,ytright,sr,grey) \
/*printf("rw: (%d,%d) - (%d,%d)\n",xbleft,ybleft,xtright,ytright);*/ \
{ int xx,yy; for (yy=ybleft;yy<=ytright;yy++) \
for (xx=xbleft;xx<=xtright;xx++) \
if (xx>=0 && xx<DISPLAY_X && yy>=0 && yy<DISPLAY_Y) \
if(BLANK_SNAKE == 0){\
if (fabs(win[xx+yy*DISPLAY_X] - grey) < fabs((sr)[xx-xbleft+(yy-ybleft)*(xtright-xbleft+1)] - grey)) \
win[xx+yy*DISPLAY_X]=((sr)[xx-xbleft+(yy-ybleft)*(xtright-xbleft+1)]);}\
else{\
if (fabs(win[xx+yy*DISPLAY_X]) < fabs((sr)[xx-xbleft+(yy-ybleft)*(xtright-xbleft+1)] - 595)){\
if (fabs((sr)[xx-xbleft+(yy-ybleft)*(xtright-xbleft+1)] - 595) > 50)\
win[xx+yy*DISPLAY_X]=((sr)[xx-xbleft+(yy-ybleft)*(xtright-xbleft+1)] - 595) ;}}\
} 

#define rectread(xbleft,ybleft,xtright,ytright,ds) \
/*printf("rr: (%d,%d) - (%d,%d)\n",xbleft,ybleft,xtright,ytright);*/ \
{ int xx,yy; for (yy=ybleft;yy<=ytright;yy++) \
for (xx=xbleft;xx<=xtright;xx++) \
if (xx>=0 && xx<DISPLAY_X && yy>=0 && yy<DISPLAY_Y) \
(ds)[xx-xbleft+(yy-ybleft)*(xtright-xbleft+1)]=win[xx+yy*DISPLAY_X]; }

/// done

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

#include "keydef.h"

#include "main.h"
#include "window.h"
#include "gamma.h"
#include "applic.h"
#include "gabor.h"
#include "snake.h"
#include "ground.h"
#include "params.h"
#include "file.h"
#include "timing.h"
#include "geom.h"

double sin(), cos(), acos(), atan2(), exp(), fabs(), drand48(), 
       floor(), fmod();


