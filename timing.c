
#include "timing.h"

#include <stdlib.h>
#include <sys/time.h>

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
