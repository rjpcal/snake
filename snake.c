#include "snake.h"

#include "geom.h"

#include <algorithm>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

namespace
{
  const double MAX_DELTA         = M_PI/4.0; // == 45 degrees
  const double TEMPERATURE       = 0.05;
  const double INITIAL_INCREMENT = 0.1;

  struct Tuple4
  {
    Tuple4() {}

    Tuple4(double a0, double a1, double a2, double a3)
    { arr[0] = a0; arr[1] = a1; arr[2] = a2; arr[3] = a3; }

    double  operator[](int i) const { return arr[i]; }
    double& operator[](int i)       { return arr[i]; }

    double arr[4];
  };

  void pickRandom4(int length, int i[])
  {
    i[0] = i[1] = i[2] = i[3] = int(length * drand48());

    while (i[1] == i[0])
    {
      i[1] = int(length * drand48());
    }

    while (i[2] == i[0] || i[2] == i[1])
    {
      i[2] = int(length * drand48());
    }

    while (i[3] == i[0] || i[3] == i[1] || i[3] == i[2])
    {
      i[3] = int(length * drand48());
    }

    std::sort(i, i+4);
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

  // Must return "true" in order to proceed with new nodes in jiggle().
  bool squashQuadrangle(const Vector& old_0, const Vector& old_1,
                        const Vector& old_2, const Vector& old_3,
                        Vector* new_0, Vector* new_1,
                        Vector* new_2, Vector* new_3,
                        double new_theta)
  {
    {
      // Here we adjust the position of node-1 by jiggling its angle with
      // respect to node-0:

      const double d = distance(old_0, old_1);

      new_1->x = old_0.x + d * cos(new_theta);
      new_1->y = old_0.y + d * sin(new_theta);
    }

    /*                                                     */
    /*                             . (2)                   */
    /*                           ./|                       */
    /*                  b      ./  `.                      */
    /*                       ./     |                      */
    /*                     ./       `.                     */
    /*                   ./          |                     */
    /*                 ./            `.  c                 */
    /*       (NEW 1) ./_________      |                    */
    /*                `--.  ang       `.                   */
    /*                    `--.         |                   */
    /*                        `--.     `.                  */
    /*                      a     `--.  |                  */
    /*                                `--. (3)             */
    /*                                                     */


    /*

    OK, we've picked a new node-1... now we want to hold node-3 fixed and
    try to find a node-2 that will keep the other distances constant. In
    trigonometric terms, we're trying to solve for the missing vertex of a
    triangle, given (1) the positions of the other two vertices, and (2)
    the lengths of all three sides.

    We do this in two steps:

      (1) assume node-1 is at (0,0), the origin
             and node-3 is at (a,0)

          then find node-2 in this unrotated coord system

      (2) now take the rotation angle (ang) required to put node-3 back in
          its proper position, and rotate node-2 by that angle as well

    From http://mathworld.wolfram.com/Triangle.html:

    A triangle with sides a, b, and c can be constructed by selecting
    vertices (0,0), (a,0), and (x,y), then solving:

        x^2   + y^2  =  b^2
      (x-a)^2 + y^2  =  c^2

    to obtain:

           a^2 + b^2 - c^2
       x = ---------------
                 2a

                    ,------------------------------
                   /          (a^2 + b^2 - c^2)^2
       y = +/- _  /  b^2  -  ---------------------
                \/                   4a^2

                   ,-----------------------------------------------
                -\/  2(a^2b^2 + b^2c^2 + a^2c^2) - a^4 - b^4 - c^4
         = +/- -----------------------------------------------------
                                       2a

    (note that the website has an incorrect formula for the solved y!)

    */

    const double a = distance(old_3, *new_1);
    const double b = distance(old_1, old_2);
    const double c = distance(old_2, old_3);

    const double cos_ang = (old_3.x - new_1->x) / a;  // adjac / hypot
    const double sin_ang = (old_3.y - new_1->y) / a;  // oppos / hypot

    const double a_sq = a*a;
    const double b_sq = b*b;
    const double c_sq = c*c;

    const double det =
      2*(a_sq*b_sq + b_sq*c_sq + a_sq*c_sq)
      - a_sq*a_sq - b_sq*b_sq - c_sq*c_sq;

    if (det < 0)
      return false;

    // OK, here's the unrotated coords of node-2:

    const double xp = (a*a + b*b - c*c) / (2.0*a);
    const double yp = sqrt(det) / (2.0*a);

    // Here's the two candidate re-rotated coords:

    const Vector new_2a
      (new_1->x + xp*cos_ang - yp*sin_ang,
       new_1->y + xp*sin_ang + yp*cos_ang);

    const Vector new_2b
      (new_1->x + xp*cos_ang + yp*sin_ang,
       new_1->y + xp*sin_ang - yp*cos_ang);

    const double d_2_2a = distance(new_2a, old_2);
    const double d_2_2b = distance(new_2b, old_2);

    *new_0 = old_0;
    *new_2 = (d_2_2a < d_2_2b) ? new_2a : new_2b;
    *new_3 = old_3;

    return true;
  }

  // This function must return true in order to accept the new set of nodes
  // in jiggle().
  bool monteCarlo(const Tuple4& newdelta, const Tuple4& olddelta,
                  double& increment)
  {
    double energy_diff = 0.0f;

    for (int n = 0; n < 4; ++n)
      {
        if (fabs(newdelta[n]) > MAX_DELTA)
          {
            increment *= 0.8;
            return false;
          }

        energy_diff += newdelta[n]*newdelta[n] - olddelta[n]*olddelta[n];
      }

    // Note, if energy_diff is < 0, then the probability is > 0, so
    // as expected drand48() will always be < probability.
    const double probability = exp(-energy_diff/TEMPERATURE);

    return drand48() <= probability;
  }
}

Snake::Snake(int l, double sp) :
  itsLength(l),
  itsSpacing(sp),
  itsElem(new Element[itsLength])
{
  const double radius = (itsLength * itsSpacing) / (2*M_PI);

  const double alpha_off = 2 * M_PI * drand48();

  for (int n = 0; n < itsLength; ++n)
    {
      const double alpha = 2 * M_PI * n / itsLength;

      itsElem[n].pos.x =  radius * cos(alpha+alpha_off);
      itsElem[n].pos.y = -radius * sin(alpha+alpha_off);
    }

  const int ITERS = 400;

  for (int count = 0; count < ITERS; ++count)
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
  result.pos.x = 0.5 * (elem(n).pos.x + elem(n+1).pos.x);
  result.pos.y = 0.5 * (elem(n).pos.y + elem(n+1).pos.y);
  result.theta = zerototwopi(-itsElem[n].theta);

  return result;
}

void Snake::center()
{
  Vector c;

  for (int n = 0; n < itsLength; ++n)
    {
      c.x += itsElem[n].pos.x;
      c.y += itsElem[n].pos.y;
    }

  c.x /= itsLength;
  c.y /= itsLength;

  for (int n = 0; n < itsLength; ++n)
    {
      itsElem[n].pos.x -= c.x;
      itsElem[n].pos.y -= c.y;
    }
}

void Snake::computeTheta()
{
  for (int n = 0; n < itsLength; ++n)
    {
      const double dx = elem(n+1).pos.x - elem(n).pos.x;
      const double dy = elem(n+1).pos.y - elem(n).pos.y;

      itsElem[n].theta = zerototwopi(atan2(dy, dx));
    }
}

/*        (pos.x[n],ypos[n])     position n                               */
/*        (xpos[n+1],ypos[n+1]) position n+1                              */
/*        (0.5*(xpos[n]+xpos[n+1]), 0.5*(ypos[n]+ypos[n+1]))              */
/*                              location of patch n                       */
/*        theta                 angle between pos n and pos n+1           */
/*                              and orientation of patch n                */
/*        delta                 angle difference between patch n and n+1  */
/*                                                                        */
/*                      theta[0]                                          */
/*                             no[0]__________                            */
/*                           ,-~ `-.                                      */
/*                        ,-~       `-.                                   */
/*                     ,-~   alpha[0]  `-.                                */
/*             a[0] ,-~                   `-.  a[3]                       */
/*               ,-~                         `-.                          */
/*            ,-~                               `-.                       */
/*         ,-~                                     `-.                    */
/*      ,-~                                           `-.                 */
/*no[1]~                                                 `-.   theta[3]   */
/*    \   alpha[1]                                          `-.           */
/*     \                                          alpha[3]    no[3]------ */
/*      \                                                 _,-~            */
/*       \                                            _,-~                */
/*        \                                       _,-~                    */
/*         \                                  _,-~                        */
/*      a[1]\                             _,-~                            */
/*           \                        _,-~                                */
/*            \                   _,-~   a[2]                             */
/*             \  alpha[2]    _,-~                                        */
/*              \         _,-~                                            */
/*               \    _,-~   theta[2]                                     */
/*              no[2]~_________                                           */

void Snake::jiggle()
{
  this->computeTheta();

  int i[4];
  pickRandom4(itsLength, i);

  const Vector old_pos[4] = {
    itsElem[i[0]].pos,
    itsElem[i[1]].pos,
    itsElem[i[2]].pos,
    itsElem[i[3]].pos
  };

  const Tuple4 old_theta = getThetas(old_pos);
  const Tuple4 old_alpha = getAlphas(old_theta);

  // The .theta member is the angle to the *next* element
  const Tuple4 old_delta
    (minuspitopi(elem(i[0]).theta - elem(i[0]-1).theta),
     minuspitopi(elem(i[1]).theta - elem(i[1]-1).theta),
     minuspitopi(elem(i[2]).theta - elem(i[2]-1).theta),
     minuspitopi(elem(i[3]).theta - elem(i[3]-1).theta));

  Vector new_pos[4];

  double increment = INITIAL_INCREMENT;

  for (;;)
    {
      const double incr = (drand48()<0.5) ? -increment : increment;

      const int r = int(4 * drand48());

      const double incr_theta = old_theta[r] - incr;

      const bool ok =
        squashQuadrangle(old_pos[(r+0)%4], old_pos[(r+1)%4],
                         old_pos[(r+2)%4], old_pos[(r+3)%4],
                         &new_pos[(r+0)%4], &new_pos[(r+1)%4],
                         &new_pos[(r+2)%4], &new_pos[(r+3)%4],
                         incr_theta);

      if (!ok)
        {
          increment /= 2.0;
          continue;
        }

      const Tuple4 new_theta = getThetas(new_pos);
      const Tuple4 new_alpha = getAlphas(new_theta);
      const Tuple4 new_delta
        (old_delta[0] + (old_alpha[0]-new_alpha[0]),
         old_delta[1] + (old_alpha[1]-new_alpha[1]),
         old_delta[2] + (old_alpha[2]-new_alpha[2]),
         old_delta[3] + (old_alpha[3]-new_alpha[3]));

      if (monteCarlo(new_delta, old_delta, increment))
        break;
    }

  for (int n = 0; n < 4; ++n)
    this->transformPath(i[n], new_pos[n],
                        i[(n+1)%4], new_pos[(n+1)%4]);

  for (int n = 0; n < 4; ++n)
    {
      itsElem[i[n]].pos = new_pos[n];
    }

  this->computeTheta();
}

void Snake::transformPath(int i1, const Vector& new1,
                          int i2, const Vector& new2)
{
  const Vector old1 = itsElem[i1].pos;
  const Vector old2 = itsElem[i2].pos;

  // OK, our jiggling algorithm has determined new locations for node-1 and
  // node-2. Now, we want to adjust all the intermediate points
  // accordingly. This amounts to (1) a translation (move node-1 from its
  // old to new location), (2) a rotation (rotate node-2 around node-1 to
  // its new location), and (3) a scaling (contract or expand to put node-2
  // in the right spot). We can then apply these same transformations to
  // the intermediate points.

  /*                                                                   */
  /*               scale by      rotate by (a'-a)                      */
  /*               distance         degrees                            */
  /*                ratio              |                               */
  /*                  |                |                               */
  /*                  |                v                               */
  /*                  v                                                */
  /*    a11  a12              cos a'-a   -sin a'-a                     */
  /*  (          ) = d'/d * (                      )                   */
  /*    a21  a12              sin a'-a    cos a'-a                     */
  /*                                                                   */

  // Now we can (1) compute this directly:
#if 1
  const double d = distance(old2, old1);
  const double a = getTheta(old2, old1);

  const double dp = distance(new2, new1);
  const double ap = getTheta(new2, new1);

  const double diff_a = ap - a;

  const double ratio_d = (dp/d);

  const double cos_diff_a = cos(diff_a);
  const double sin_diff_a = sin(diff_a);

  const double a11 =   ratio_d * cos_diff_a;
  const double a12 = - ratio_d * sin_diff_a;
  const double a21 =   ratio_d * sin_diff_a;
  const double a22 =   ratio_d * cos_diff_a;
#else

  // or (2) simplify it to avoid cos+sin:

  /*                                                                   */
  /*    a11  a12              cos a'  -sin a'       cos -a   -sin -a   */
  /*  (          ) = d'/d * (                 ) * (                  ) */
  /*    a21  a12              sin a'   cos a'       sin -a    cos -a   */
  /*                                                                   */
  /*                                                                   */
  /*                          cos a'  -sin a'        cos a   sin a     */
  /*               = d'/d * (                 ) * (                )   */
  /*                          sin a'   cos a'       -sin a   cos a     */
  /*                                                                   */
  /*                                                                   */
  /*                          dx'/d' -dy'/d'        dx/d   dy/d        */
  /*               = d'/d * (                ) * (              )      */
  /*                          dy'/d'  dx'/d'       -dy/d   dx/d        */
  /*                                                                   */
  /*                                                                   */
  /*                          dx'/d  -dy'/d         dx/d   dy/d        */
  /*               =        (                ) * (              )      */
  /*                          dy'/d   dx'/d        -dy/d   dx/d        */
  /*                                                                   */
  /*                                                                   */
  /*                   dx'*dx+dy'*dy   dx'*dy-dy'*dx                   */
  /*               = (                               ) / d^2           */
  /*                   dy'*dx-dx'*dy   dy'*dy+dx'*dx                   */
  /*                                                                   */

  const double dx  = old2.x - old1.x;
  const double dy  = old2.y - old1.y;

  const double d_sq = dx*dx + dy*dy;

  const double dxp = new2.x - new1.x;
  const double dyp = new2.y - new1.y;

  const double a11 = (dxp*dx + dyp*dy)/d_sq;
  const double a12 = (dxp*dy - dyp*dx)/d_sq;
  const double a21 = (dyp*dx - dxp*dy)/d_sq;
  const double a22 = (dyp*dy + dxp*dx)/d_sq;
#endif

  // Loop over all the nodes in between the nodes given by i1 and i2
  for (int n = (i1+1)%itsLength; n != i2; n = (n+1)%itsLength)
    {
      /*                                              */
      /*   x'      c1       a11   a12     x - b1      */
      /* (   ) = (    ) + (           ) (        )    */
      /*   y'      c2       a21   a12     y - b2      */
      /*                                              */

      const double diffx = itsElem[n].pos.x - old1.x;
      const double diffy = itsElem[n].pos.y - old1.y;

      itsElem[n].pos.x = new1.x + a11*diffx + a12*diffy;
      itsElem[n].pos.y = new1.y + a21*diffx + a22*diffy;
    }
}
