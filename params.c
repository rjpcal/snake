
#include "incl.h"

int   DISPLAY_X;
int   DISPLAY_Y;
int   FOREG_NUMBER;
int   PATCH_NUMBER;
int   BACKG_NUMBER;
int   TOTAL_NUMBER;
float FOREG_SPACING;
float FOREG_ECCENTRICITY;
float FOREG_JITTER;
int   FOREG_POSITIONS;
float FOREG_DIFFERENCE;
float BACKG_INI_SPACING;
float BACKG_MIN_SPACING;
float GABOR_PERIOD;
float GABOR_SIGMA;
int   GABOR_SIZE;
int   DISPLAY_NUMBER;
char  FILENAME[STRINGSIZE];

char  line[STRINGSIZE], text[STRINGSIZE];

void ReadParams( char extension[] )
{
    FILE *fp;
    char c, sdummy[STRINGSIZE];
    int   idummy;

    Openfile( &fp, READ, extension );

    if( fp == NULL )
        return;

    GETINT(   (DISPLAY_X) );
    GETINT(   (DISPLAY_Y) );
    GETINT(   (FOREG_NUMBER) );
    GETINT(   (idummy) );
    GETINT(   (idummy) );
    GETINT(   (idummy) );
    GETFLOAT( (FOREG_SPACING) );
    GETFLOAT( (FOREG_ECCENTRICITY) );
    GETFLOAT( (FOREG_JITTER) );
    GETINT(   (FOREG_POSITIONS) );
    GETFLOAT( (FOREG_DIFFERENCE) );
    GETFLOAT( (BACKG_INI_SPACING) );
    GETFLOAT( (BACKG_MIN_SPACING) );
    GETFLOAT( (GABOR_PERIOD) );
    GETFLOAT( (GABOR_SIGMA) );
    GETINT(   (GABOR_SIZE) );
    GETINT(   (DISPLAY_NUMBER) );            
    GETTEXT(  (sdummy) );

    Closefile( fp );
}

void CheckParams( void )
{
    InitGabor();
}

void WriteParams( char extension[] )
{
    FILE *fp;

    Openfile( &fp, WRITE, extension );

    PUTINT(   (DISPLAY_X),          ("DISPLAY_X") );         
    PUTINT(   (DISPLAY_Y),          ("DISPLAY_Y") );         
    PUTINT(   (FOREG_NUMBER) ,      ("FOREG_NUMBER") );      
    PUTINT(   (PATCH_NUMBER) ,      ("PATCH_NUMBER") );      
    PUTINT(   (BACKG_NUMBER),       ("BACKG_NUMBER") );            
    PUTINT(   (TOTAL_NUMBER),       ("TOTAL_NUMBER") );            
    PUTFLOAT( (FOREG_SPACING) ,     ("FOREG_SPACING") );     
    PUTFLOAT( (FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
    PUTFLOAT( (FOREG_JITTER),       ("FOREG_JITTER") );      
    PUTINT(   (FOREG_POSITIONS),    ("FOREG_POSITIONS") );   
    PUTFLOAT( (FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );
    PUTFLOAT( (BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );     
    PUTFLOAT( (BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );     
    PUTFLOAT( (GABOR_PERIOD),       ("GABOR_PERIOD") );      
    PUTFLOAT( (GABOR_SIGMA),        ("GABOR_SIGMA") );       
    PUTINT(   (GABOR_SIZE),         ("GABOR_SIZE") );        
    PUTINT(   (DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );            
    PUTTEXT(  (FILENAME),           ("FILENAME") );          

    Closefile( fp );
}

void ListParams( void )
{
    FILE *fp;
    int nparams = 0;
    char params[60][STRINGSIZE];

    WriteParams( "sta" );

    Openfile( &fp, READ,   "sta" );

    while( fgets( params[nparams], STRINGSIZE, fp) !=  NULL && nparams < 60 )
    {    
        params[nparams][ strlen( params[nparams] ) - 1 ] = '\0';
	nparams++;
    }

    Closefile( fp );

    ClearWindow();

    ShowParams( params, nparams );
}

void Openfile( FILE **fp, char mode, char extension[] )
{
    FILE *fopen();

    char fname[STRINGSIZE];

    sprintf( fname, "%s.%s", FILENAME, extension );

    if( mode==WRITE )
    {
        if( ( *fp = fopen( fname, "w") ) == NULL )
        {
            printf( "cannot write %s\n", fname);
	    Exit();
        }
    }
    else
    if( mode==APPEND )
    {
        if( ( *fp = fopen( fname, "a") ) == NULL )
        {
            printf( "cannot append to %s\n", fname);
	    Exit();
        }
    }
    else
    if( mode==READ )
    {
        if( ( *fp = fopen( fname, "r") ) == NULL )
	{
	    printf( "cannot read from %s\n", fname );
	    Exit();
	}
    }

    chmod( fname, 0666 );
}

void Closefile( FILE *fp )
{
    if( fp != NULL)
        fclose( fp);
}

void PrintParams( void )
{
    PRINTINT(   (DISPLAY_X),          ("DISPLAY_X") );         
    PRINTINT(   (DISPLAY_Y),          ("DISPLAY_Y") );         
    PRINTINT(   (FOREG_NUMBER) ,      ("FOREG_NUMBER") );      
    PRINTINT(   (PATCH_NUMBER) ,      ("PATCH_NUMBER") );      
    PRINTINT(   (BACKG_NUMBER),       ("BACKG_NUMBER") );            
    PRINTINT(   (TOTAL_NUMBER),       ("TOTAL_NUMBER") );            
    PRINTFLOAT( (FOREG_SPACING) ,     ("FOREG_SPACING") );     
    PRINTFLOAT( (FOREG_ECCENTRICITY), ("FOREG_ECCENTRICITY") );
    PRINTFLOAT( (FOREG_JITTER),       ("FOREG_JITTER") );      
    PRINTINT(   (FOREG_POSITIONS),    ("FOREG_POSITIONS") );   
    PRINTFLOAT( (FOREG_DIFFERENCE),   ("FOREG_DIFFERENCE") );
    PRINTFLOAT( (BACKG_INI_SPACING),  ("BACKG_INI_SPACING") );     
    PRINTFLOAT( (BACKG_MIN_SPACING),  ("BACKG_MIN_SPACING") );     
    PRINTFLOAT( (GABOR_PERIOD),       ("GABOR_PERIOD") );      
    PRINTFLOAT( (GABOR_SIGMA),        ("GABOR_SIGMA") );       
    PRINTINT(   (GABOR_SIZE),         ("GABOR_SIZE") );        
    PRINTINT(   (DISPLAY_NUMBER),     ("DISPLAY_NUMBER") );            
    PRINTTEXT(  (FILENAME),           ("FILENAME") );          
}

/**********************************/

void SetParameters1( void )
{
    char word[STRINGSIZE], text[9][STRINGSIZE];

    ClearWindow();

    sprintf( text[0], " FOREG NUMBER     SPACING    DIFFERENC ECCENTRIC" );
    sprintf( text[1], "" );
    sprintf( text[2], "" );
    sprintf( text[3], "       %-10d %-10.2f %-10.2f %-10.2f", 
	     FOREG_NUMBER, FOREG_SPACING, FOREG_DIFFERENCE, FOREG_ECCENTRICITY );

    ShowMenu( text, 4);

    EnterInt( &FOREG_NUMBER );
    sprintf( word, "       %-10d", FOREG_NUMBER );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterFloat( &FOREG_SPACING );
    sprintf( word, " %-10.2f", FOREG_SPACING );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterFloat( &FOREG_DIFFERENCE );
    sprintf( word, " %-10.2f", FOREG_DIFFERENCE );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterFloat( &FOREG_ECCENTRICITY );
    sprintf( word, " %-10.2f", FOREG_ECCENTRICITY );
    strcat( text[1], word );
    ShowMenu( text, 4 );
}

void SetParameters2( void )
{
    char word[STRINGSIZE], text[4][STRINGSIZE];

    ClearWindow();

    sprintf( text[0], " BACKG INI_SPACING   MIN_SPACING   DISPLAY_X     DISPLAY_Y     DISPLAY_NUMBER" );
    sprintf( text[1], "" );
    sprintf( text[2], "" );
    sprintf( text[3], "       %-13.2f %-13.2f %-13d %-13d %-13d",
	    BACKG_INI_SPACING, BACKG_MIN_SPACING, DISPLAY_X, DISPLAY_Y, DISPLAY_NUMBER );

    ShowMenu( text, 4);

    EnterFloat( &BACKG_INI_SPACING );
    sprintf( word, "       %-13.2f", BACKG_INI_SPACING );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterFloat( &BACKG_MIN_SPACING );
    sprintf( word, " %-13.2f", BACKG_MIN_SPACING );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterInt( &DISPLAY_X );
    sprintf( word, " %-13d", DISPLAY_X );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterInt( &DISPLAY_Y );
    sprintf( word, " %-13d", DISPLAY_Y );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterInt( &DISPLAY_NUMBER );
    sprintf( word, " %-13d", DISPLAY_NUMBER );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    CheckParams();
}

void SetParameters3( void )
{
    char word[STRINGSIZE], text[4][STRINGSIZE];

    ClearWindow();

    sprintf( text[0], "       PERIOD     SIGMA      SIZE       FILENAME" );
    sprintf( text[1], "" );
    sprintf( text[2], "" );
    sprintf( text[3], "       %-10.2f %-10.2f %-10d %-20s",
	      GABOR_PERIOD, GABOR_SIGMA, GABOR_SIZE, FILENAME );

    ShowMenu( text, 4);

    EnterFloat( &GABOR_PERIOD );
    sprintf( word, "       %-10.2f", GABOR_PERIOD );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterFloat( &GABOR_SIGMA );
    sprintf( word, " %-10.2f", GABOR_SIGMA );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterInt(   &GABOR_SIZE );
    sprintf( word, " %-10d", GABOR_SIZE );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    EnterText(   FILENAME );
    sprintf( word, " %-20s", FILENAME );
    strcat( text[1], word );
    ShowMenu( text, 4 );

    CheckParams();
}

void EnterInt( int *pi )
{
    int n;
    char c, word[STRINGSIZE];

    n = 0;

    while( ( c = GetKeystroke() ) != ' '  && 
	     n                     < STRINGSIZE )
    {
         word[n++] = c;        
    }
    
    word[n] = '\0';

    sscanf( word, "%d", pi );
}

void EnterFloat( float *pf )
{
    int n;
    char c, word[STRINGSIZE];

    n = 0;

    while( ( c = GetKeystroke() ) != ' '  && 
	     n                     < STRINGSIZE )
    {
         word[n++] = c;        
    }
    
    word[n] = '\0';

    sscanf( word, "%f", pf );
}

void EnterText( char *ps )
{
    int n;
    char c, word[STRINGSIZE];

    n = 0;

    while( ( c = GetKeystroke() ) != ' '  && 
	     n                     < STRINGSIZE )
    {
         word[n++] = c;        
    }
    
    word[n] = '\0';

    sscanf( word, "%s", ps );
}


/*******************************/

void date( char *p )
{
    FILE *fp, *popen();

    if( ( fp =	popen( "date", "r")) ==	 NULL) 
    {
	printf( "cannot access date");
	return;
    }
    if( fgets( p, 50, fp) ==  NULL) 
    {
	printf( "cannot read date");
	return;
    }
    pclose(fp);
}

void process_id( char pid[] )
{
    FILE *fp, *popen();
    char line[ STRINGSIZE ];

    sprintf( line, "ps | grep %s", PROGRAM );

    if( ( fp =	popen( line, "r")) ==	 NULL) 
    {
	printf( "cannot access PID");
	return;
    }
    if( fgets( line, STRINGSIZE, fp) ==  NULL) 
    {
	printf( "cannot read PID");
	return;
    }
    pclose(fp);

    sscanf( line, "%s", pid );

    printf( " Process ID %s\n", pid );
    fflush( stdout );
}
