#include "snake.h"

#include "geom.h"

#include <assert.h>
#include <math.h>
#include <stdlib.h>

namespace
{
  const double HIDELTA     = (M_PI/4.);
  const double LODELTA     = (-M_PI/4.);
  const double TEMPERATURE = 0.05;
  const double INCREMENT   = 0.1;

  const int    FOREGROUND_ITERS = 400;

  float increment;

  struct Tuple4
  {
    Tuple4() {}

    Tuple4(float a0, float a1, float a2, float a3)
    { arr[0] = a0; arr[1] = a1; arr[2] = a2; arr[3] = a3; }

    float  operator[](int i) const { return arr[i]; }
    float& operator[](int i)       { return arr[i]; }

    float arr[4];
  };

  void swap2(int& a, int& b) { int a1 = a; a = b; b = a1;}

  void sort2(int& a, int& b) { if (a > b) swap2(a,b); }

  void pickRandom4(int length, int i[])
  {
    i[0] = int(length * drand48());

    do
      {
        i[1] = int(length * drand48());
      }
    while(i[1] == i[0]);

    do
      {
        i[2] = int(length * drand48());
      }
    while(i[2] == i[0] || i[2] == i[1]);

    do
      {
        i[3] = int(length * drand48());
      }
    while(i[3] == i[0] || i[3] == i[1] || i[3] == i[2]);

    sort2(i[0], i[1]);
    sort2(i[0], i[2]);
    sort2(i[0], i[3]);
    sort2(i[1], i[2]);
    sort2(i[1], i[3]);
    sort2(i[2], i[3]);
  }

  float distance(const Vector& n1, const Vector& n2)
  {
    const float dx = n1.x - n2.x;
    const float dy = n1.y - n2.y;
    return sqrt(dx*dx + dy*dy);
  }

  Tuple4 getEdgeLengths(const Vector no[4])
  {
    return Tuple4(distance(no[0], no[1]),
                  distance(no[1], no[2]),
                  distance(no[2], no[3]),
                  distance(no[3], no[0]));
  }

  Tuple4 getThetas(const Vector no[4])
  {
    const float dx0     = no[1].x - no[0].x;
    const float dy0     = no[1].y - no[0].y;
    const float dx1     = no[2].x - no[1].x;
    const float dy1     = no[2].y - no[1].y;
    const float dx2     = no[3].x - no[2].x;
    const float dy2     = no[3].y - no[2].y;
    const float dx3     = no[0].x - no[3].x;
    const float dy3     = no[0].y - no[3].y;

    return Tuple4(atan2(dy0, dx0),
                  atan2(dy1, dx1),
                  atan2(dy2, dx2),
                  atan2(dy3, dx3));
  }

  Tuple4 getAlphas(const Tuple4& theta)
  {
    return Tuple4(Zerototwopi(M_PI - theta[0] + theta[3]),
                  Zerototwopi(M_PI - theta[1] + theta[0]),
                  Zerototwopi(M_PI - theta[2] + theta[1]),
                  Zerototwopi(M_PI - theta[3] + theta[2]));
  }

  int newApex(const Vector* no1, Vector* no2, const Vector* no3,
              float b, float c)
  {
    /*                                                    */
    /*   x'      dx/e   dy/e     x - no1->x               */
    /* (   ) = (             ) (            )             */
    /*   y'     -dy/e   dx/e     y - no1->y               */
    /*                                                    */

    /*                                                    */
    /*   x' =          aleph + bet                        */
    /*   y' = +- sqrt( gimel - bet**2 - aleph**2 )        */
    /*                                                    */
    /*   aleph = (b**2-c**2)/(2*e)                        */
    /*   bet   = e/2                                      */
    /*   gimel = (b**2+c**2)/2                            */
    /*                                                    */

    /*                                                    */
    /*   x       no1->x       dx/e  -dy/e     x'          */
    /* (   ) = (        ) + (             ) (   )         */
    /*   y       no1->y       dy/e   dx/e     y'          */
    /*                                                    */

    const float dx    = no3->x - no1->x;
    const float dy    = no3->y - no1->y;

    const float e     = sqrt(dx*dx + dy*dy);

    const float aleph = (b*b-c*c)/(2.*e);
    const float bet   = e/2.;
    const float gimel = (b*b+c*c)/2.;

    if (gimel < bet*bet + aleph*aleph)
      return 0;

    const float xp = aleph + bet;
    const float yp = sqrt(gimel - bet*bet - aleph*aleph);

    Vector no2_one, no2_two;

    no2_one.x = no1->x + xp*dx/e - yp*dy/e;
    no2_one.y = no1->y + xp*dy/e + yp*dx/e;

    no2_two.x = no1->x + xp*dx/e + yp*dy/e;
    no2_two.y = no1->y + xp*dy/e - yp*dx/e;

    const float dis_one = distance(no2_one, *no2);
    const float dis_two = distance(no2_two, *no2);

    *no2 = (dis_one < dis_two) ? no2_one : no2_two;

    return 1;
  }

  int squashQuadrangle(const Vector* no0, const Vector* no1,
                       const Vector* no2, const Vector* no3,
                       Vector* new_no0, Vector* new_no1,
                       Vector* new_no2, Vector* new_no3,
                       float theta0, float incr)
  {
    Vector no[4];
    no[0] = *new_no0 = *no0;
    no[1] =            *no1;
    no[2] = *new_no2 = *no2;
    no[3] = *new_no3 = *no3;

    const Tuple4 a = getEdgeLengths(no);

    new_no1->x = new_no0->x + a[0] * cos(theta0-incr);
    new_no1->y = new_no0->y + a[0] * sin(theta0-incr);

    return newApex(new_no1, new_no2, new_no3, a[1], a[2]);
  }

  bool monteCarlo(const Tuple4& old_alpha, const Tuple4& new_alpha,
                  const Tuple4& lo_alpha, const Tuple4& hi_alpha)
  {
    bool zero_probability = false;

    float energy_difference = 0.0f;

    for (int n = 0; n < 4; ++n)
      {
        const float zero_point = 0.5 * (hi_alpha[n] + lo_alpha[n]);

        const float oldval = fabs(old_alpha[n] - zero_point);
        const float newval = fabs(new_alpha[n] - zero_point);

        if (newval > HIDELTA)
          {
            zero_probability = true;
            increment = 0.8 * increment;
            break;
          }
        else
          {
            energy_difference += newval*newval - oldval*oldval;
          }
      }

    float probability = 0.0f;

    if (!zero_probability)
      if (energy_difference < 0.)
        {
          probability = 1.0f;
        }
      else
        {
          probability = exp(-energy_difference/TEMPERATURE);
        }

    return drand48() <= probability;
  }
}

Snake::Snake(int l, float sp) :
  itsLength(l),
  itsSpacing(sp),
  itsElem(new Element[itsLength])
{
  const float radius = (float)(itsLength * itsSpacing) / (2*M_PI);

  const float alpha_off = 2 * M_PI * drand48();

  for (int n = 0; n < itsLength; ++n)
    {
      const float alpha = 2 * M_PI * n / itsLength;

      itsElem[n].xpos   =  radius * cos(alpha+alpha_off);
      itsElem[n].ypos   = -radius * sin(alpha+alpha_off);
    }

  this->computeDeltaTheta();

  for (int count = 0; count < FOREGROUND_ITERS; ++count)
    {
      this->jiggle();
    }

  this->center();
}

Snake::~Snake()
{
  delete [] itsElem;
}

void Snake::getElement(int n, float* x, float* y, float* theta) const
{
  assert(n < itsLength);

  *x = 0.5 * (itsElem[n].xpos + itsElem[(n+1)%itsLength].xpos);
  *y = 0.5 * (itsElem[n].ypos + itsElem[(n+1)%itsLength].ypos);
  *theta = Zerototwopi(-itsElem[n].theta);
}

void Snake::center()
{
  float xc = 0.0f;
  float yc = 0.0f;

  for (int n = 0; n < itsLength; ++n)
    {
      xc += itsElem[n].xpos;
      yc += itsElem[n].ypos;
    }

  xc /= itsLength;
  yc /= itsLength;

  for (int n = 0; n < itsLength; ++n)
    {
      itsElem[n].xpos -= xc;
      itsElem[n].ypos -= yc;
    }
}

void Snake::computeDeltaTheta()
{
  for (int n0 = 0; n0 < itsLength; ++n0)
    {
      const int n1 = (n0+1)%itsLength;

      const float dx = itsElem[n1].xpos - itsElem[n0].xpos;
      const float dy = itsElem[n1].ypos - itsElem[n0].ypos;

      itsElem[n0].theta = Zerototwopi(atan2(dy, dx));

    }

  for (int n0 = 0; n0 < itsLength; ++n0)
    {
      const int n1 = (n0+1)%itsLength;

      itsElem[n1].delta = Minuspitopi(itsElem[n1].theta - itsElem[n0].theta);
    }
}

/*        (xpos[n],ypos[n])     position n                                   */
/*        (xpos[n+1],ypos[n+1]) position n+1                                 */
/*        (0.5*(xpos[n]+xpos[n+1]), 0.5*(ypos[n]+ypos[n+1]))                 */
/*                              location of patch n                          */
/*        theta                 angle between pos n and pos n+1              */
/*                              and orientation of patch n                   */
/*        delta                 angle difference between patch n and n+1     */
/*                                                                           */
/*                      theta[0]                                             */
/*                             no[0]__________                               */
/*                           .     .                                         */
/*                        .  alpha[0] .                                      */
/*                     .                 .                                   */
/*             a[0] .                       .  a[3]                          */
/*               .                             .                             */
/*            .                                   .                          */
/*         .                                         .                       */
/*      .                                               .     theta[3]       */
/*no[1] alpha[1]                                           .                 */
/*    .                                                     no[3]_________   */
/*     .                                              alpha[3].              */
/*      .                                                  .                 */
/*       .                                             .                     */
/*        .                                        .                         */
/*         .                                   .                             */
/*      a[1].                              .                                 */
/*           .                         .                                     */
/*            .                    .     a[2]                                */
/*             .               .                                             */
/*              . alpha[2] .                                                 */
/*               .     .  theta[2]                                           */
/*              no[2]__________                                              */

void Snake::jiggle()
{
  int i[4];
  pickRandom4(itsLength, i);

  increment = INCREMENT;

  Vector no[4];
  float delta[4];
  for (int n = 0; n < 4; ++n)
    {
      no[n].x  = itsElem[i[n]].xpos;
      no[n].y  = itsElem[i[n]].ypos;
      delta[n] = itsElem[i[n]].delta;
    }

  const Tuple4 theta = getThetas(no);
  const Tuple4 alpha = getAlphas(theta);

  Tuple4 lo_alpha;
  Tuple4 hi_alpha;
  for (int n = 0; n < 4; ++n)
    {
      lo_alpha[n] = alpha[n] - (HIDELTA - delta[n]);
      hi_alpha[n] = alpha[n] + (delta[n] - LODELTA);
    }

  Vector new_no[4];

  for (;;)
    {
      const float incr = (drand48()<0.5) ? -increment : increment;

      const int r = int(4 * drand48());

      const int ok =
        squashQuadrangle(&no[(r+0)%4], &no[(r+1)%4],
                         &no[(r+2)%4], &no[(r+3)%4],
                         &new_no[(r+0)%4], &new_no[(r+1)%4],
                         &new_no[(r+2)%4], &new_no[(r+3)%4],
                         theta[r], incr);

      if (!ok)
        {
          increment /= 2.0;
          continue;
        }

      const Tuple4 new_theta = getThetas(new_no);
      const Tuple4 new_alpha = getAlphas(new_theta);

      if (monteCarlo(alpha, new_alpha, lo_alpha, hi_alpha))
        break;
    }

  for (int n = 0; n < 4; ++n)
    this->replaceNodes(i[n], new_no[n],
                       i[(n+1)%4], new_no[(n+1)%4]);

  for (int n = 0; n < 4; ++n)
    {
      itsElem[i[n]].xpos = new_no[n].x;
      itsElem[i[n]].ypos = new_no[n].y;
    }

  this->computeDeltaTheta();
}

void Snake::replaceNodes(int i1, const Vector& new1,
                         int i2, const Vector& new2)
{
  /*                                              */
  /*   x'      c1        a11   a12     x - b1     */
  /* (   ) = (    ) + (             ) (      )    */
  /*   y'      c2        a21   a12     y - b2     */
  /*                                              */

  const float old1x  = itsElem[i1].xpos;
  const float old1y  = itsElem[i1].ypos;

  const float old2x  = itsElem[i2].xpos;
  const float old2y  = itsElem[i2].ypos;

  const float old_dx  = old2x - old1x;
  const float old_dy  = old2y - old1y;

  const float new_dx = new2.x - new1.x;
  const float new_dy = new2.y - new1.y;

  const float l_2 = old_dx*old_dx + old_dy*old_dy;

  const float a11 = (new_dx*old_dx + new_dy*old_dy)/l_2;
  const float a12 = (new_dx*old_dy - new_dy*old_dx)/l_2;

  const float a21 = (new_dy*old_dx - new_dx*old_dy)/l_2;
  const float a22 = (new_dy*old_dy + new_dx*old_dx)/l_2;

  for (int n = (i1+1)%itsLength; n != i2; n = (n+1)%itsLength)
    {
      const float diffx = itsElem[n].xpos - old1x;
      const float diffy = itsElem[n].ypos - old1y;

      itsElem[n].xpos = new1.x + a11*diffx + a12*diffy;
      itsElem[n].ypos = new1.y + a21*diffx + a22*diffy;
    }
}
