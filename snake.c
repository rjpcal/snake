#include "snake.h"

#include "geom.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

namespace
{
  const double HIDELTA     = (M_PI/4.);
  const double TEMPERATURE = 0.05;
  const double INCREMENT   = 0.1;

  const int    FOREGROUND_ITERS = 400;

  double increment;

  struct Tuple4
  {
    Tuple4() {}

    Tuple4(double a0, double a1, double a2, double a3)
    { arr[0] = a0; arr[1] = a1; arr[2] = a2; arr[3] = a3; }

    double  operator[](int i) const { return arr[i]; }
    double& operator[](int i)       { return arr[i]; }

    double arr[4];
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

  double distance(const Vector& n1, const Vector& n2)
  {
    const double dx = n1.x - n2.x;
    const double dy = n1.y - n2.y;
    return sqrt(dx*dx + dy*dy);
  }

  Tuple4 getEdgeLengths(const Vector no[4])
  {
    return Tuple4(distance(no[0], no[1]),
                  distance(no[1], no[2]),
                  distance(no[2], no[3]),
                  distance(no[3], no[0]));
  }

  double getTheta(const Vector& n1, const Vector& n2)
  {
    return atan2(n1.y - n2.y, n1.x - n2.x);
  }

  Tuple4 getThetas(const Vector no[4])
  {
    return Tuple4(getTheta(no[1], no[0]),
                  getTheta(no[2], no[1]),
                  getTheta(no[3], no[2]),
                  getTheta(no[0], no[3]));
  }

  Tuple4 getAlphas(const Tuple4& theta)
  {
    return Tuple4(zerototwopi(M_PI - theta[0] + theta[3]),
                  zerototwopi(M_PI - theta[1] + theta[0]),
                  zerototwopi(M_PI - theta[2] + theta[1]),
                  zerototwopi(M_PI - theta[3] + theta[2]));
  }

  int newApex(const Vector* no1, Vector* no2, const Vector* no3,
              double b, double c)
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

    const double dx    = no3->x - no1->x;
    const double dy    = no3->y - no1->y;

    const double e     = sqrt(dx*dx + dy*dy);

    const double aleph = (b*b-c*c)/(2.*e);
    const double bet   = e/2.;
    const double gimel = (b*b+c*c)/2.;

    if (gimel < bet*bet + aleph*aleph)
      return 0;

    const double xp = aleph + bet;
    const double yp = sqrt(gimel - bet*bet - aleph*aleph);

    Vector no2_one, no2_two;

    no2_one.x = no1->x + xp*dx/e - yp*dy/e;
    no2_one.y = no1->y + xp*dy/e + yp*dx/e;

    no2_two.x = no1->x + xp*dx/e + yp*dy/e;
    no2_two.y = no1->y + xp*dy/e - yp*dx/e;

    const double dis_one = distance(no2_one, *no2);
    const double dis_two = distance(no2_two, *no2);

    *no2 = (dis_one < dis_two) ? no2_one : no2_two;

    return 1;
  }

  int squashQuadrangle(const Vector* no0, const Vector* no1,
                       const Vector* no2, const Vector* no3,
                       Vector* new_no0, Vector* new_no1,
                       Vector* new_no2, Vector* new_no3,
                       double theta0, double incr)
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
                  const Tuple4& delta)
  {
    bool zero_probability = false;

    double energy_difference = 0.0f;

    for (int n = 0; n < 4; ++n)
      {
        const double oldval = fabs(delta[n]);
        const double newval = fabs(new_alpha[n] - old_alpha[n] - delta[n]);

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

    double probability = 0.0f;

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

Snake::Snake(int l, double sp) :
  itsLength(l),
  itsSpacing(sp),
  itsElem(new Element[itsLength])
{
  const double radius = (double)(itsLength * itsSpacing) / (2*M_PI);

  const double alpha_off = 2 * M_PI * drand48();

  for (int n = 0; n < itsLength; ++n)
    {
      const double alpha = 2 * M_PI * n / itsLength;

      itsElem[n].xpos   =  radius * cos(alpha+alpha_off);
      itsElem[n].ypos   = -radius * sin(alpha+alpha_off);
    }

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

Element Snake::getElement(int n) const
{
  assert(n < itsLength);

  Element result;

  result.type = Element::CONTOUR;
  result.xpos = 0.5 * (itsElem[n].xpos + itsElem[(n+1)%itsLength].xpos);
  result.ypos = 0.5 * (itsElem[n].ypos + itsElem[(n+1)%itsLength].ypos);
  result.theta = zerototwopi(-itsElem[n].theta);

  return result;
}

void Snake::center()
{
  double xc = 0.0f;
  double yc = 0.0f;

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

void Snake::computeTheta()
{
  for (int n0 = 0; n0 < itsLength; ++n0)
    {
      const int n1 = (n0+1)%itsLength;

      const double dx = itsElem[n1].xpos - itsElem[n0].xpos;
      const double dy = itsElem[n1].ypos - itsElem[n0].ypos;

      itsElem[n0].theta = zerototwopi(atan2(dy, dx));
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
  this->computeTheta();

  int i[4];
  pickRandom4(itsLength, i);

  increment = INCREMENT;

  Vector no[4];
  for (int n = 0; n < 4; ++n)
    {
      no[n].x  = itsElem[i[n]].xpos;
      no[n].y  = itsElem[i[n]].ypos;
    }

  const Tuple4 theta = getThetas(no);
  const Tuple4 alpha = getAlphas(theta);

  double deltas[itsLength];
  for (int n1 = 0; n1 < itsLength; ++n1)
    {
      const int n0 = (n1+itsLength-1)%itsLength;

      deltas[n1] = minuspitopi(itsElem[n1].theta - itsElem[n0].theta);
    }

  const Tuple4 delta(deltas[i[0]],
                     deltas[i[1]],
                     deltas[i[2]],
                     deltas[i[3]]);

  Vector new_no[4];

  for (;;)
    {
      const double incr = (drand48()<0.5) ? -increment : increment;

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

      if (monteCarlo(alpha, new_alpha, delta))
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

  this->computeTheta();
}

void Snake::replaceNodes(int i1, const Vector& new1,
                         int i2, const Vector& new2)
{
  /*                                              */
  /*   x'      c1        a11   a12     x - b1     */
  /* (   ) = (    ) + (             ) (      )    */
  /*   y'      c2        a21   a12     y - b2     */
  /*                                              */

  const double old1x  = itsElem[i1].xpos;
  const double old1y  = itsElem[i1].ypos;

  const double old2x  = itsElem[i2].xpos;
  const double old2y  = itsElem[i2].ypos;

  const double old_dx  = old2x - old1x;
  const double old_dy  = old2y - old1y;

  const double new_dx = new2.x - new1.x;
  const double new_dy = new2.y - new1.y;

  const double l_2 = old_dx*old_dx + old_dy*old_dy;

  const double a11 = (new_dx*old_dx + new_dy*old_dy)/l_2;
  const double a12 = (new_dx*old_dy - new_dy*old_dx)/l_2;

  const double a21 = (new_dy*old_dx - new_dx*old_dy)/l_2;
  const double a22 = (new_dy*old_dy + new_dx*old_dx)/l_2;

  for (int n = (i1+1)%itsLength; n != i2; n = (n+1)%itsLength)
    {
      const double diffx = itsElem[n].xpos - old1x;
      const double diffy = itsElem[n].ypos - old1y;

      itsElem[n].xpos = new1.x + a11*diffx + a12*diffy;
      itsElem[n].ypos = new1.y + a21*diffx + a22*diffy;
    }
}
