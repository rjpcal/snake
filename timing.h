
extern void CheckFrameTime( void );
extern void SeedRand( void );
extern void ZeroRand( int seed );
extern void SetVideoCount( void );
extern void CheckVideoCount( int number );
extern void SetTime( void );
extern void CheckTime( float aim_for_seconds );
extern void SetOne( void );
extern void SetZero( void );
extern void ZeroOne( void );
extern void SetTwo( void );
extern void SetThree( void );
extern void TwoThree( void );
extern void GetTime( struct timeval *tp );
extern double DeltaTime( struct timeval *tp0, struct timeval *tp1 );
