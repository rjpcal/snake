
#include "geom.h"

#include "defs.h"

float Zerototwopi( float angle )
{
    while( angle < 0. )
        angle += TWOPI;

    while( angle >= TWOPI )
        angle -= TWOPI;

    return( angle );
}

float Minuspitopi( float angle )
{
    while( angle < -M_PI )
        angle += TWOPI;

    while( angle >= M_PI )
        angle -= TWOPI;

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

