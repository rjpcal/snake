
#include "file.h"
#include "gabor.h"
#include "ground.h"
#include "params.h"
#include "snake.h"
#include "timing.h"
#include "window.h"

int   DISPLAY_SET_NUMBER;
int   DISPLAY_COUNT;

void InitApplication( void )
{
    SeedRand();

    ReadParams( "sta" );

    PrintParams();

    InitGabor();
}

void WrapApplication( void )
{
    WrapGabor();

    WriteParams( "sta" );
}

void WriteApplication( void )
{
    DISPLAY_COUNT = 0;

    DISPLAY_SET_NUMBER = 1;

    WriteHeader();

    int n;

    for( n=0; n<DISPLAY_NUMBER; n++ )
    {
        ZeroRand( n );

        MakeForeg();

        MakeGround();

        WriteArray();

        ShowArray();

        Window2Raster();

        DISPLAY_COUNT++;
    }
}
