
#include "incl.h"

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

float Zerotoone( float value )
{
    while( value < 0. )
        value += 1.;

    while( value >= 1. )
        value -= 1.;

    return( value );
}

float Threesixty( float angle )
{
    while( angle < 0. )
        angle += 360.;

    while( angle >= 360. )
        angle -= 360.;

    return( angle );
}

float AngleDiff( float alpha, float beta )
{
    return( Minustoplushalfpi( alpha - beta ) );
}

float Minustoplushalfpi( float angle )
{
    while( angle <= -PI/2. )
	angle += PI;

    while( angle > PI/2. )
	angle -= PI;

    return( angle );
}

int Round( float x )
{
    int ix;

    if( x < 0 )
        ix = (int)( x - 0.5 );
    else
        ix = (int)( x + 0.5 );

    return( ix );
}

