
#include "incl.h"

/************************* requires ********************

     SeedRandom()
     ReadStatus()
     PrintStatus()
     InitScreen()
     InitApplication()
     SwitchApplication( char c )
     WrapApplication()

********************************************************/

char PROGRAM[ STRINGSIZE ];

/// memory for fake window:
Colorindex *win;

main( argc, argv )
int argc;
char **argv;
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
    win = malloc(DISPLAY_X * DISPLAY_Y * sizeof(Colorindex));


    /** invoke AFTER InitScreen **/

    InitApplication();

    ///EventLoop();
    WriteApplication();

    Exit();

}

void EventLoop( void )
{
    long dev;
    short val;

#ifdef DUMMY
    DONE("Eventloop");
#endif

    ///qdevice( KEYBD );
    ///qdevice( ESCKEY );

    while( 1 )
    {
      /*##
        switch( dev = qread( &val ) )
	{

	    case ESCKEY:
	    case WINQUIT:
	        break;

	    case KEYBD:
	    ##*/
  SwitchApplication( (char)('\0'+val) );
  /*##
		break;

	    default:
  	        break;
	}
	##*/
    }
}

void Exit( void )
{  
#ifdef DUMMY
    DONE("Exit");
#endif

    WrapApplication();

    ///    scrnselect(0); /* Return to control screen */
  
    ///gexit();
    
    exit(0);
}

int KeyPress( void )
{
    long dev;
    short val;
    
    dev = 0;
    
    ///if( qtest() )
    ///{
    ///    dev = qread( &val );
    ///    qreset();
    ///}

    ///return( dev==KEYBD );
    return 1;
} 

char GetReady( void )
{
    char c;
    /*##
    do
    {
        c = GetChar();
    }
    while( c != ' ' && c != 'q' );

    return( c == ' ' );
    ##*/
    return 1;
}

char GetChar( void )
{
    char c;
    long dev;
    short val;
    /*##
    do
    {
        dev=qread(&val);
    }
    while(dev!=KEYBD);

    c=(char) ('\0'+val);
    
    return( c );
    ##*/
    return 32;
}

char GetKeystroke( void )
{
    char c;
    long dev;
    short val;
    /*##
    do
    {
        dev=qread(&val);
    }
    while(dev!=KEYBD);

    c=(char) ('\0'+val);

    return( c );
    ##*/
    return 32;
}

