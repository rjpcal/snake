#include "geom.h"

#include <math.h>

double Zerototwopi(double angle)
{
  while (angle < 0.)
    angle += (2*M_PI);

  while (angle >= (2*M_PI))
    angle -= (2*M_PI);

  return angle;
}

double Minuspitopi(double angle)
{
  while (angle < -M_PI)
    angle += (2*M_PI);

  while (angle >= M_PI)
    angle -= (2*M_PI);

  return angle;
}

double Zerotopi(double angle)
{
  while (angle < 0.)
    angle += M_PI;

  while (angle >= M_PI)
    angle -= M_PI;

  return angle;
}

