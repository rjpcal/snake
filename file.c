
#include "file.h"

#include "applic.h"
#include "defs.h"
#include "gabor.h"
#include "geom.h"
#include "ground.h"
#include "keydef.h"
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
    PUTFLOAT( (BACKG_AVE_SPACING),  ("BACKG_AVE_SPACING") );
    PUTFLOAT( (BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );
    PUTFLOAT( (BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );

    for( i=0; i<TOTAL_NUMBER; i++ )
    {
        if( array[i].flag )
        {
            o = (int)( RAD2DEG * array[i].theta + 0.5 );

            x = (int)( array[i].xpos + 0.5 );

            y = (int)( array[i].ypos + 0.5 );

            s = array[i].flag;

            fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

    for( i=0; i<TOTAL_NUMBER; i++ )
    {
        if( !array[i].flag )
        {
            o = (int)( RAD2DEG * array[i].theta + 0.5 );

            x = (int)( array[i].xpos + 0.5 );

            y = (int)( array[i].ypos + 0.5 );

            s = array[i].flag;

            fprintf( fp, "%-5d %-5d %-5d %-5d\n", x, y, o, s );
        }
    }

    Closefile( fp );
}

void ReadHeader( void )
{
    FILE *fp;

    Openfile( &fp, READ, "snk" );

    GETINT(   (DISPLAY_SET_NUMBER) );
    GETINT(   (DISPLAY_NUMBER) );
    GETINT(   (DISPLAY_X) );
    GETINT(   (DISPLAY_Y) );
    GETFLOAT( (GABOR_PERIOD) );
    GETFLOAT( (GABOR_SIGMA) );
    GETINT(   (GABOR_SIZE) );
    GETFLOAT( (FOREG_ECCENTRICITY) );
    GETFLOAT( (FOREG_JITTER) );
    GETINT(   (FOREG_POSITIONS) );
    GETFLOAT( (FOREG_DIFFERENCE) );

    FILEMARKER = ftell( fp );

    Closefile( fp );
}


void ReadArray( void )
{
    int i, x, y, o, s;
    FILE *fp;

    Openfile( &fp, READ, "snk" );

    fseek( fp, FILEMARKER, SEEK_SET );

    GETINT(   (DISPLAY_COUNT) );
    GETINT(   (TOTAL_NUMBER) );
    GETINT(   (FOREG_NUMBER) );
    GETINT(   (PATCH_NUMBER) );
    GETFLOAT( (FOREG_SPACING) );
    GETFLOAT( (BACKG_AVE_SPACING) );
    GETFLOAT( (BACKG_INI_SPACING) );
    GETFLOAT( (BACKG_MIN_SPACING) );

    BACKG_AVE_SPACING = sqrt((double)(2.0*DISPLAY_X*DISPLAY_Y)/(SQRT3*TOTAL_NUMBER));

    printf( " TOTAL NUMBER %d   AVE SPACING %f\n",
              TOTAL_NUMBER, BACKG_AVE_SPACING );

    for( i=0; i<TOTAL_NUMBER; i++ )
    {
        fgets( line, STRINGSIZE, fp);
        sscanf(line, "%d %d %d %d",  &x, &y, &o, &s );

        array[i].xpos = (float) x;

        array[i].ypos = (float) y;

        array[i].theta = (float) DEG2RAD * o;
    }

    FILEMARKER = ftell( fp );

    Closefile( fp );

    Map2Array( TOTAL_NUMBER );
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
            theta = Zerototwopi( array[i].theta + HALFPI );
        }

        XPatch[ i ] = (int)( array[ i ].xpos + DISPLAY_X / 2.0 + 0.5 );
        YPatch[ i ] = (int)( array[ i ].ypos + DISPLAY_Y / 2.0 + 0.5 );

        PPatch[ i ] = GetPatch( theta, phi );
    }

    NPatch = npts;

    SeedRand();
}
