
#include "applic.h"
#include "defs.h"
#include "params.h"
#include "timing.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>

void Exit();

char PROGRAM[ STRINGSIZE ];

/// memory for fake window:
Colorindex *win;

int main( int argc, char** argv )
{
#ifdef DUMMY
    DONE("main");
#endif

    if( argc<2 )
    {
        printf(" You forgot to supply a filename!\n");
        Exit();
    }

    sprintf( PROGRAM, "%s", argv[0]);
    sprintf( FILENAME,"%s", argv[1]);

    SeedRand();

    ReadParams( "sta" );

    PrintParams();

    /** invoke AFTER ReadParams **/

    InitScreen();

    /// alloc mem for our fake window:
    win = (Colorindex*)
      (malloc(DISPLAY_X * DISPLAY_Y * sizeof(Colorindex)));


    /** invoke AFTER InitScreen **/

    InitApplication();

    WriteApplication();

    Exit();

    return 0;
}

void Exit( void )
{
#ifdef DUMMY
    DONE("Exit");
#endif

    WrapApplication();

    exit(0);
}

