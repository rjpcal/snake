
#include "incl.h"

/// for conversion to big endian:
#include <netinet/in.h>

void InitScreen(void)
{
    int planes;  
    ///    fmfonthandle font1, font2;

#ifdef DUMMY 
    DONE("InitScreen");
#endif

    /* Intialise font manager and check we can do double buffered RGB*/
    /*##
    fminit();
    font1=fmfindfont("Times-Roman");
    fmsetfont(font1);
    font2=fmscalefont(font1,144);
    fmsetfont(font2);
    if (getgdesc(GD_BITS_NORM_DBL_RED) == 0) {
        fprintf(stderr, "Double buffered RGB not available on this machine\n");
        return;
    }
    ##*/
    /* Initialise window and device drivers, make the colormap */
    ///noborder();

    ///prefposition(0, DISPLAY_X, 1280-DISPLAY_Y, 1280 ); 

    /**
    prefposition(-DISPLAY_X/2, DISPLAY_X/2, -DISPLAY_Y/2, DISPLAY_Y/2 );
    **/
    /*##foreground();
    winopen("surround");
    concave(TRUE);
    doublebuffer();
    gconfig();
    shademodel(FLAT);
    qdevice(ESCKEY);
    ortho2(-DISPLAY_X/2, DISPLAY_X/2, -DISPLAY_Y/2, DISPLAY_Y/2); 
    qdevice(KEYBD);
    ##*/
    MakeColormap();
    CheckCalibration();

    ///planes = (int) getplanes();

    ///printf( " %d planes %d colors\n", planes, (int)( pow(2.0,(double)planes)));

}

void ClearWindow(void)
{
  /*##
    color(Grey);
    clear();
    swapbuffers();
    clear();
    swapbuffers();
    ##*/
}

void ClearBuffer(void)
{
  /// color(Grey);
  /// clear();
}

void FrameCount( int number )
{
  /// while( number-- )
  ///   swapbuffers(); 
}

void ShowMenu( char menu[][STRINGSIZE], int nmenu )
{
    int i;

#ifdef DUMMY
    DONE("ShowMenu");
#endif
    /*##
    color(Grey);
    clear();
    swapbuffers();
    clear();

    color(Black);

    for( i=0; i<nmenu; i++ )
    {
        cmov2i( -300, 100 - i * 25 );
	charstr( menu[i] );
    }

    swapbuffers();
    ##*/
}


void ShowParams( char params[][STRINGSIZE], int nparams )
{
    int i;

#ifdef DUMMY
     DONE("ShowStatus");
#endif
     /*##
    color(Grey);
    clear;

    color( Black );

    for( i=0; i<((23<nparams)?23:nparams); i++ )
    {
        cmov2i( -350, 225 - i * 20 );
	charstr( params[i] );
    }

    for(    ; i<nparams; i++ )
    {
        cmov2i(    0, 685 - i * 20 );
	charstr( params[i] );
    }

    swapbuffers();
    ##*/
}

void ShowArray( void )
{
    int i, *px, *py;
    int xbotleft, ybotleft, xtopright, ytopright;
    int tempColor;
    Colorindex **src;

#ifdef DUMMY
    DONE("ShowArray");
#endif

    ///color( Grey );
    ///clear();

    /// let's clear our fake window:
    if(BLANK_SNAKE == 0)
    {
      tempColor = Grey;
      for (i=0;i<DISPLAY_X*DISPLAY_Y;i++) win[i]=Grey;
    }
    else
    {
      for (i=0;i<DISPLAY_X*DISPLAY_Y;i++) win[i]=0;
      tempColor = 0;
    }

    px     = XPatch;
    py     = YPatch;
    src    = PPatch;

    for( i=0; i<NPatch; i++, px++, py++, src++ )
    {
        xbotleft  = *px - GABOR_SIZE / 2;
	ybotleft  = *py - GABOR_SIZE / 2;
	xtopright = xbotleft + GABOR_SIZE - 1;
	ytopright = ybotleft + GABOR_SIZE - 1;

        rectwrite( xbotleft, ybotleft, xtopright, ytopright, *src, tempColor);    
    }

    ///swapbuffers();
}

void Window2Raster( void )
{
    FILE *fp, *fopen();
    unsigned char fname[STRINGSIZE], map[256];
    int i, k, x, y, dx, dy, size, header[8];
    Colorindex *ptr, *pt;
    unsigned char *ctr, *ct;

    static int count = 0;

#ifdef DUMMY
    DONE("Window2Raster");
#endif

    ///color( Black );
    ///clear();
    
    ///swapbuffers();
    if(BLANK_SNAKE == 0) 
      sprintf( fname, "%s_%d.ras", FILENAME, count++ );
    else
      sprintf( fname, "%s_%d_BLANK.ras", FILENAME, count++ );
    if( ( fp = fopen( fname, "w" ) ) == NULL )
    {
        printf( " %s: file not opened\n", fname );

    }

 
    header[0] = 0x59a66a95; 
    header[1] = DISPLAY_X;
    header[2] = DISPLAY_Y;
    header[3] = 8;
    header[4] = DISPLAY_X * DISPLAY_Y;
    header[5] = 1;
    header[6] = 1;
    header[7] = 768;

    ///convert to big endian:
    for (i=0;i<8;i++) header[i]=htonl(header[i]);



    fwrite( header, sizeof(int), 8, fp );

    for( i=0; i<256; i++ )
        map[i] = (unsigned char) i;

    fwrite( map, sizeof(unsigned char), 256, fp );
    fwrite( map, sizeof(unsigned char), 256, fp );
    fwrite( map, sizeof(unsigned char), 256, fp );

    dx = DISPLAY_X;
    dy = DISPLAY_Y / 4;

    size = dx*dy;

    ptr = (Colorindex*) malloc( dx*dy*sizeof(Colorindex) );
    ctr = (unsigned char*) malloc( dx*dy*sizeof(unsigned char) );

    for( i=0; i<4; i++ )
    {
        x  = 0;
        y  = i*dy;

        rectread( x, y, x+dx-1, y+dy-1, ptr );

        pt = ptr;
	ct = ctr;
	
	for( k=0; k<size; k++ )
	    *ct++ = I2Char( *pt++ );

	fwrite( ctr, sizeof(unsigned char), size, fp );
    }

    free( ptr );

    fclose( fp );

    ///swapbuffers();

}

