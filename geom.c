
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
    while( angle < -PI )
        angle += TWOPI;

    while( angle >= PI )
        angle -= TWOPI;

    return( angle );
}

float Zerotopi( float angle )
{
    while( angle < 0. )
        angle += PI;

    while( angle >= PI )
        angle -= PI;

    return( angle );
}

