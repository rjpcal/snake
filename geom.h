#include <math.h>

inline double zerototwopi(double angle)
{
  while (angle < 0.0)     angle += 2*M_PI;
  while (angle >= 2*M_PI) angle -= 2*M_PI;

  return angle;
}

inline double minuspitopi(double angle)
{
  while (angle < -M_PI) angle += 2*M_PI;
  while (angle >= M_PI) angle -= 2*M_PI;

  return angle;
}

inline double zerotopi(double angle)
{
  while (angle < 0.0)   angle += M_PI;
  while (angle >= M_PI) angle -= M_PI;

  return angle;
}
