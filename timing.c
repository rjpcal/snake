
#include "incl.h"

double FRAMETIME;

void CheckFrameTime( void )
{
    struct timeval tp[2];

    ClearWindow();

    FrameCount( 1 );

    GetTime( &tp[0] );

    FrameCount( 99 );

    GetTime( &tp[1] );

    FRAMETIME = DeltaTime( &tp[0], &tp[1] ) / 100.0;

    printf( " Video frame time %7.4lf ms\n", FRAMETIME );
}

void SeedRand( void )
{
    struct timeval tp;
    struct timezone tzp;

    gettimeofday( &tp, &tzp );

//     srand48( tp.tv_sec );
    srand48( 0 );
}

void ZeroRand( int seed )
{
    srand48( seed );
}

/*************************************************/

double sec_video, usec_video;

void SetVideoCount( void )
{
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday( &tp, &tzp );

    sec_video  = tp.tv_sec;

    usec_video = tp.tv_usec;
}

void CheckVideoCount( int number )
{
    struct timeval  tp;
    struct timezone tzp;
    double aim_for_time, actual_time;
    long aim_for_secs, aim_for_usecs;
    long sec_stop, usec_stop;

    aim_for_time  = number*FRAMETIME/1000.;
    aim_for_secs  = (long)             floor( aim_for_time );
    aim_for_usecs = (long)( 1000000. *  fmod( aim_for_time, 1.0 ) );

    sec_stop  = aim_for_secs  + sec_video;
    usec_stop = aim_for_usecs + usec_video;

    if( usec_stop > 1000000L )
    {
        sec_stop  += 1L;
        usec_stop -= 1000000L;
    }

    do
    {
        gettimeofday( &tp, &tzp );
    }
    while( tp.tv_sec < sec_stop );

    do
    {
        gettimeofday( &tp, &tzp );
    }
    while( tp.tv_usec < usec_stop );

    sec_video  = tp.tv_sec;

    usec_video = tp.tv_usec;
}

/*************************************************/

double sec_time, usec_time;

void SetTime( void )
{
    struct timeval  tp;
    struct timezone tzp;

    gettimeofday( &tp, &tzp );

    sec_time  = tp.tv_sec;

    usec_time = tp.tv_usec;
}

void CheckTime( float aim_for_seconds )
{
    struct timeval  tp;
    struct timezone tzp;
    double actual_time;
    long aim_for_secs, aim_for_usecs;
    long sec_stop, usec_stop;

    aim_for_secs  = (long)             floor( aim_for_seconds );
    aim_for_usecs = (long)( 1000000. *  fmod( aim_for_seconds, 1.0 ) );

    sec_stop  = aim_for_secs  + sec_time;
    usec_stop = aim_for_usecs + usec_time;

    if( usec_stop > 1000000L )
    {
        sec_stop  += 1L;
        usec_stop -= 1000000L;
    }

    do
    {
        gettimeofday( &tp, &tzp );
    }
    while( tp.tv_sec < sec_stop );

    do
    {
        gettimeofday( &tp, &tzp );
    }
    while( tp.tv_usec < usec_stop );

    sec_time  = tp.tv_sec;

    usec_time = tp.tv_usec;
}

/*************************************************/

struct timeval TP[4];

void SetOne( void )
{
    GetTime( &TP[1] );
}

void SetZero( void )
{
    GetTime( &TP[0] );
}

void ZeroOne( void )
{
    double msec_lapsed;

    msec_lapsed = DeltaTime( &TP[0], &TP[1] );

    printf( " %7.1lfms\n", msec_lapsed );

}

void SetTwo( void )
{
    GetTime( &TP[2] );
}

void SetThree( void )
{
    GetTime( &TP[3] );
}

void TwoThree( void )
{
    double msec_lapsed;

    msec_lapsed = DeltaTime( &TP[2], &TP[3] );

    printf( " %7.1lfms\n", msec_lapsed );

}

/*************************************************/

void GetTime( struct timeval *tp )
{
    struct timezone tzp;
    gettimeofday( tp, &tzp );
}

double DeltaTime( struct timeval *tp0, struct timeval *tp1 )
{
    double sec_lapsed, msec_lapsed, delta;

    sec_lapsed  = (double)( tp1->tv_sec  - tp0->tv_sec );

    msec_lapsed = (double)( tp1->tv_usec - tp0->tv_usec ) / 1000.;

    delta       = sec_lapsed * 1000. + msec_lapsed;

    return( delta );
}

