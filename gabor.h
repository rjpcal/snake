
#include "defs.h"

extern Colorindex *Patch[][GABOR_MAX_PHASE];

extern void AllocGabor( void );
extern void WrapGabor( void );
extern void InitGabor( void );
extern void InitPatch( Colorindex *ptr, float sigma, float omega, float theta, float phi, float contrast, int size );
extern Colorindex *GetPatch( float theta, float phi );
extern float Index2Theta( int index );
extern int Theta2Index( float theta );
extern float Index2Phi( int index );
extern int Phi2Index( float phi );

