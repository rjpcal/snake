#define      READ                  'r'
#define      WRITE                 'w'
#define      APPEND                'a'
#define      OPTIONAL              'o'

#define GETINT(name) {fgets(line,120,fp);sscanf(line,"%s %d",text,&(name));}
// #define GETCHAR(name) {fgets(line,120,fp);sscanf(line,"%s %c",text,&(name));}
#define GETFLOAT(name) {fgets(line,120,fp);sscanf(line,"%s %f",text,&(name));}
#define GETTEXT(name) {fgets(line,120,fp);sscanf(line,"%s %s",text,(name));}
// #define GETINTL(name) {fgets(line,120,fp);sscanf(line,"%s %d %d %d %d",text,&(name)[0],&(name)[1],&(name)[2],&(name)[3]);}
// #define GETTEXTL(name) {fgets(line,120,fp);sscanf(line,"%s %s %s %s %s",text,(name)[0],(name)[1],(name)[2],(name)[3]);}

#define PUTINT(name,text) {fprintf(fp,"%-19s %d\n",(text),(name));}
// #define PUTCHAR(name,text) {fprintf(fp,"%-19s %c\n",(text),(name));}
#define PUTFLOAT(name,text) {fprintf(fp,"%-19s %.2f\n",(text),(name));}
#define PUTTEXT(name,text) {fprintf(fp,"%-19s %s\n",(text),(name));}
// #define PUTINTL(name,text) {fprintf(fp,"%-19s %d %d %d %d\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}
// #define PUTTEXTL(name,text) {fprintf(fp,"%-19s %s %s %s %s\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}

#define PRINTINT(name,text) {printf( "%-19s %d\n",(text),(name));}
// #define PRINTCHAR(name,text) {printf( "%-19s %c\n",(text),(name));}
#define PRINTFLOAT(name,text) {printf( "%-19s %.2f\n",(text),(name));}
#define PRINTTEXT(name,text) {printf( "%-19s %s\n",(text),(name));}
// #define PRINTINTL(name,text) {printf( "%-19s %d %d %d %d\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}
// #define PRINTTEXTL(name,text) {printf( "%-19s %s %s %s %s\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}

extern char line[], text[];

