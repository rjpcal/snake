
#include "file.h"

#include "defs.h"
#include "gabor.h"
#include "geom.h"
#include "ground.h"
#include "keydef.h"
#include "main.h"
#include "params.h"
#include "timing.h"

#include <stdlib.h>

long FILEMARKER;

void WriteHeader( void )
{
    FILE *fp;

    Openfile( &fp, WRITE, "snk" );

    PUTINT(   (DISPLAY_SET_NUMBER), ("DISPLAY_SET_NUMBER") );
    PUTINT(   (DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );
    PUTINT(   (DISPLAY_X),          ("DISPLAY_X") );
    PUTINT(   (DISPLAY_Y),          ("DISPLAY_Y") );
    PUTFLOAT( (GABOR_PERIOD),       ("GABOR_PERIOD") );
    PUTFLOAT( (GABOR_SIGMA),        ("GABOR_SIGMA") );
    PUTINT(   (GABOR_SIZE),         ("GABOR_SIZE") );
    PUTFLOAT( (FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
    PUTFLOAT( (FOREG_JITTER),       ("FOREG_JITTER") );
    PUTINT(   (FOREG_POSITIONS),    ("FOREG_POSITIONS") );
    PUTFLOAT( (FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );

    Closefile( fp );
}

void WriteArray( void )
{
    int i, x, y, o, s;
    FILE *fp;

    Openfile( &fp, APPEND, "snk" );

    PUTINT(   (DISPLAY_COUNT),      ("DISPLAY_COUNT") );
    PUTINT(   (TOTAL_NUMBER),       ("TOTAL_NUMBER") );
    PUTINT(   (FOREG_NUMBER),       ("FOREG_NUMBER") );
    PUTINT(   (PATCH_NUMBER),       ("PATCH_NUMBER") );
    PUTFLOAT( (FOREG_SPACING) ,     ("FOREG_SPACING") );
    PUTFLOAT( (theGround->BACKG_AVE_SPACING),  ("BACKG_AVE_SPACING") );
    PUTFLOAT( (BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
    PUTFLOAT( (BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );

    for( i=0; i<TOTAL_NUMBER; i++ )
    {
        if( theGround->array[i].flag )
        {
            o = (int)( RAD2DEG * theGround->array[i].theta + 0.5 );

            x = (int)( theGround->array[i].xpos + 0.5 );

            y = (int)( theGround->array[i].ypos + 0.5 );

            s = theGround->array[i].flag;

            fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

    for( i=0; i<TOTAL_NUMBER; i++ )
    {
        if( !theGround->array[i].flag )
        {
            o = (int)( RAD2DEG * theGround->array[i].theta + 0.5 );

            x = (int)( theGround->array[i].xpos + 0.5 );

            y = (int)( theGround->array[i].ypos + 0.5 );

            s = theGround->array[i].flag;

            fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

    Closefile( fp );
}


void Map2Array( int npts )
{
    int i;
    float theta, phi, Theta[ MAX_FOREG_NUMBER ], Phi[ MAX_FOREG_NUMBER ];

    for( i=0; i<npts && i<FOREG_NUMBER; i++ )
    {
        Phi[i]   = Zerototwopi( TWOPI * drand48() );
        Theta[i] = Zerototwopi( TWOPI * drand48() );
    }

    for( i=0; i<npts; i++ )
    {
        phi   = TWOPI * drand48();
        theta = TWOPI * drand48();

        if( i < FOREG_NUMBER )
        {
            phi   = Phi[ i ];
            theta = Zerototwopi( theGround->array[i].theta + M_PI_2 );
        }

        theGround->XPatch[ i ] = (int)( theGround->array[ i ].xpos + DISPLAY_X / 2.0 + 0.5 );
        theGround->YPatch[ i ] = (int)( theGround->array[ i ].ypos + DISPLAY_Y / 2.0 + 0.5 );

        theGround->PPatch[ i ] = GetPatch( theta, phi );
    }

    theGround->NPatch = npts;

    SeedRand();
}
