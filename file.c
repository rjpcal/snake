
#include "file.h"

#include "defs.h"
#include "ground.h"
#include "keydef.h"
#include "main.h"
#include "params.h"

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

void WriteArray(const Ground* g)
{
    int i, x, y, o, s;
    FILE *fp;

    Openfile( &fp, APPEND, "snk" );

    PUTINT(   (DISPLAY_COUNT),      ("DISPLAY_COUNT") );
    PUTINT(   (TOTAL_NUMBER),       ("TOTAL_NUMBER") );
    PUTINT(   (FOREG_NUMBER),       ("FOREG_NUMBER") );
    PUTINT(   (PATCH_NUMBER),       ("PATCH_NUMBER") );
    PUTFLOAT( (FOREG_SPACING) ,     ("FOREG_SPACING") );
    PUTFLOAT( (g->BACKG_AVE_SPACING),  ("BACKG_AVE_SPACING") );
    PUTFLOAT( (BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
    PUTFLOAT( (BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );

    for( i=0; i<TOTAL_NUMBER; i++ )
    {
        if( g->array[i].flag )
        {
            o = (int)( RAD2DEG * g->array[i].theta + 0.5 );

            x = (int)( g->array[i].xpos + 0.5 );

            y = (int)( g->array[i].ypos + 0.5 );

            s = g->array[i].flag;

            fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

    for( i=0; i<TOTAL_NUMBER; i++ )
    {
        if( !g->array[i].flag )
        {
            o = (int)( RAD2DEG * g->array[i].theta + 0.5 );

            x = (int)( g->array[i].xpos + 0.5 );

            y = (int)( g->array[i].ypos + 0.5 );

            s = g->array[i].flag;

            fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

    Closefile( fp );
}
