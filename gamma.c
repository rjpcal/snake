
#include "defs.h"
#include "gamma.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define ITMAX           100
#define EPS             3.0e-7
#define FPMIN           1.0e-30

#define Y_FAC           881.2
#define X_FAC           884.8
#define A_GAMMA         1.54

#define LUMI_LEVELS     10001
#define MAX_COLINDEX    1786
#define RRATIO          0.236
#define GRATIO          0.679
#define BRATIO          0.085
#define MAXCONTRAST     100.0

#define Red             0x0000ff

int White, Grey, Black;

Colorindex colindex[LUMI_LEVELS]; /* contains colindex values for each lumi */

double lumi[MAX_COLINDEX]; /*contains lumi val( 0 -> 1) for each index; */

cmapval colormap[MAX_COLINDEX];

void nrerror(char error_text[]);

Colorindex F2I(double fraction)
{
    return colindex[(int) rint((fraction+1.0)/2.0 * (LUMI_LEVELS-1))];
}

unsigned char I2Char( Colorindex index )
{
    return( (unsigned char)((int)(255.0*lumi[index]) % 256) );
}

void MakeColormap(void)
{
  double ratio;
  int i,j;
  int index;
  short rpattern[8] = {0,0,1,1,0,0,1};
  short gpattern[8] = {0,0,0,0,1,1,1};
  short bpattern[8] = {0,1,0,1,0,1,0};
  short r,g,b;

  index = 0;

  for (i=0; i < 255; ++i)
    {
      for (j=0; j < 7; ++j)
        {
          r = i + rpattern[j];
          g = i + gpattern[j];
          b = i + bpattern[j];

          colormap[index].r = r;
          colormap[index].g = g;
          colormap[index].b = b;

          lumi[index] = luminance(r,g,b)/luminance(255,255,255);

          ++index;
        }
    }

  colormap[index].r = 255;
  colormap[index].g = 255;
  colormap[index].b = 255;

  lumi[index] = 1.0;

  for (i=0; i<LUMI_LEVELS; i++) {

    ratio = ((double) i) / ((double) (LUMI_LEVELS-1));

    index = 0;
    while (lumi[index] <= ratio && index < MAX_COLINDEX-1) {
      index++;
    }

    if ( (lumi[index]-ratio) < (ratio - lumi[index-1]) ) {
      colindex[i] = index;
    } else {
      colindex[i] = index-1;
    }
  }

  Black = colindex[(LUMI_LEVELS-1)*0];
  Grey  = colindex[(LUMI_LEVELS-1)/2];
  White = colindex[(LUMI_LEVELS-1)*1];

  printf( " Black %d Grey %d White %d\n", Black, Grey, White );

}


float luminance(short r, short g, short b)
     /* returns value between 0.0 and 1.0 */
{

  float lumi;

  float gammp(float a, float x);

  lumi = gammp(A_GAMMA,(float) r /X_FAC) * Y_FAC*RRATIO +
    gammp(A_GAMMA,(float) g /X_FAC) * Y_FAC*GRATIO +
    gammp(A_GAMMA,(float) b /X_FAC) * Y_FAC*BRATIO;

   return lumi;
}

float gammln(float xx)
{
  double x,y,tmp,ser;
  static double cof[6] = {76.18009172947146,-86.50532032941677,
                          24.01409824083091,-1.231739572450155,
                          0.1208650973866179e-2,-0.5395239384953e-5};
  int j;

  y=x=xx;
  tmp=x+5.5;
  tmp -= (x+0.5)*log(tmp);
  ser=1.000000000190015;
  for (j=0;j<=5;j++) ser += cof[j]/++y;
  return -tmp+log(2.5066282746310005*ser/x);
}

float gammp(float a, float x)
{
  void gfc(float *gammcf, float a, float x, float *gln);
  void gser(float *gamser, float a, float s, float *gln);
  float gamser, gammcf, gln;

  if (x < 0.0 || a <= 0.0) nrerror("Invalid arguments in routine gammp");

  if(x < (a+1.0)) {
    gser(&gamser, a,x,&gln);
    return gamser;
  } else {
    gfc(&gammcf,a,x,&gln);
    return 1.0 - gammcf;
  }
}


void gser(float *gamser, float a, float x, float *gln)
{
  float gammln(float xx);
  int n;
  float sum,del,ap;

  *gln=gammln(a);
  if (x <= 0.0) {
    if (x < 0.0) nrerror("x less than 0 in routine gser");
    *gamser=0.0;
    return;
  } else {
    ap=a;
    del=sum=1.0/a;
    for (n=1;n<=ITMAX;n++) {
      ++ap;
      del *= x/ap;
      sum += del;
      if (fabs(del) < fabs(sum)*EPS){
        *gamser=sum*exp(-x+a*log(x)-(*gln));
        return;
      }
    }
    nrerror("a too large, ITMAX too small in routine gser");
    return;
  }
}


void gfc(float *gammcf,float a, float x, float *gln)
{
  float gammln(float xx);
  int i;
  float an, b,c,d,del,h;

  *gln=gammln(a);
  b=x=1.0-a;
  c=1.0/FPMIN;
  d=1.0/b;
  h=d;
  for (i=1; i < ITMAX; i++){
    an = -i*(i-a);
    b += 2.0;
    d = an*d+b;
    if (fabs(d) < FPMIN) d=FPMIN;
    c=b+an/c;
    if (fabs(c) < FPMIN) c=FPMIN;
    d=1.0/d;
    del=d*c;
    h *= del;
    if (fabs(del-1.0) < EPS) break;
  }
  if (i > ITMAX) nrerror("a too large, ITMAX too small in gcf");
  *gammcf=exp(-x+a*log(x)-(*gln))*h;
}

void nrerror(char error_text[])
{
  fprintf(stderr,"%s\n",error_text);
  exit(1);
}
