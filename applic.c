
#include "incl.h"

int   DISPLAY_SET_NUMBER;
int   DISPLAY_COUNT;

void InitApplication( void )
{
    int stimT;

    struct timeval tp;
    struct timezone tzp;

    SeedRand();

    ReadParams( "sta" );

    PrintParams();

    InitGabor();
}

void SwitchApplication( char c )
{
    switch( c )
    {
        case 'q':
	    Exit();
	    break;

	case 'v':
            ViewApplication();
	    break;

	case 'w':
            WriteApplication();
	    break;

	case 'r':
            ReadApplication();
	    break;

	case '1':
            SetParameters1();
	    break;

	case '2':
            SetParameters2();
	    break;

	case '3':
            SetParameters3();
	    break;

	case 'l':
            ListParams();
	    break;

	default:
            MakeMenu();
	    break;
    }
}

void WrapApplication( void )
{
    WrapGabor();

    WriteParams( "sta" );
}

void MakeMenu( void )
{
    int nmenu;
    char menu[10][STRINGSIZE];

    strcpy( menu[0], "v     view snakes");
    strcpy( menu[1], "w     write snakes");
    strcpy( menu[2], "r     read snakes");
    strcpy( menu[3], "l     list parameters");
    strcpy( menu[4], "1     set params 1");
    strcpy( menu[5], "2     set params 2");
    strcpy( menu[6], "3     set params 3");
    strcpy( menu[7], "q     quit program");

    nmenu = 8;

    ClearWindow();

    ShowMenu( menu, nmenu );
}

void ViewApplication( void )
{
    int n;

    ZeroRand( 0.0 );

    ClearWindow();

    MakeArray();

    ShowArray();

}

void WriteApplication( void )
{
    int n;
  
    DISPLAY_COUNT = 0;

    DISPLAY_SET_NUMBER = 1;

    ClearWindow();

    WriteHeader();

    for( n=0; n<DISPLAY_NUMBER; n++ )
    {
        ZeroRand( n );

        MakeArray();

        WriteArray();

        ShowArray();

	///	sginap( 100 );

	Window2Raster();

	DISPLAY_COUNT++;
    }

    MakeMenu();
}

void ReadApplication( void )
{
    int i, j;

    ClearWindow();

    ReadHeader();

    for( i=0; i<DISPLAY_NUMBER*DISPLAY_SET_NUMBER; i++ )
    {
        ReadArray();

	Map2Array( TOTAL_NUMBER );

        ShowArray();

        SetTime();
 
        CheckTime( 0.2 );
    }

    MakeMenu();
}

void MakeArray( void )
{
    MakeForeg();

    MakeGround();
}



