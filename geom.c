#include "geom.h"

#include <math.h>

float Zerototwopi( float angle )
{
  while( angle < 0. )
    angle += (2*M_PI);

  while( angle >= (2*M_PI) )
    angle -= (2*M_PI);

  return( angle );
}

float Minuspitopi( float angle )
{
  while( angle < -M_PI )
    angle += (2*M_PI);

  while( angle >= M_PI )
    angle -= (2*M_PI);

  return( angle );
}

float Zerotopi( float angle )
{
  while( angle < 0. )
    angle += M_PI;

  while( angle >= M_PI )
    angle -= M_PI;

  return( angle );
}

