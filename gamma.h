
extern Colorindex F2I(double fraction);
extern unsigned char I2Char( Colorindex index );
extern void MakeColormap(void);
extern float luminance(short r, short g, short b);
extern float gammln(float xx);
extern float gammp(float a, float x);
extern void gser(float *gamser, float a, float x, float *gln);
extern void gfc(float *gammcf,float a, float x, float *gln);
extern void nrerror(char error_text[]);
extern void CheckCalibration(void);

extern int White, Grey, Black;


