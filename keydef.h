#define      YES                   'y'
#define      NO                    'n'

#define      READY                 ' '
#define      QUIT                  'q'

#define      LEFTBUTTON            'l'
#define      RIGHTBUTTON           'r'
#define      MIDDLEBUTTON          'm'

#define      BOTRIGHT               0
#define      BOTLEFT                1
#define      TOPLEFT                2
#define      TOPRIGHT               3

#define      BOTRIGHTKEY           '3'
#define      BOTLEFTKEY            '1'
#define      TOPLEFTKEY            '7'
#define      TOPRIGHTKEY           '9'

#define      CONF_ONE               0
#define      CONF_TWO               1
#define      CONF_THREE             2
#define      CONF_FOUR              3
#define      CONF_FIVE              4
#define      CONF_SIX               5
#define      CONF_SEVEN             6
#define      CONF_EIGHT             7
#define      CONF_NINE              8
#define      CONF_TEN               9

#define      CONF_ONE_KEY          '0'
#define      CONF_TWO_KEY          '1'
#define      CONF_THREE_KEY        '2'
#define      CONF_FOUR_KEY         '3'
#define      CONF_FIVE_KEY         '4'
#define      CONF_SIX_KEY          '5'
#define      CONF_SEVEN_KEY        '6'
#define      CONF_EIGHT_KEY        '7'
#define      CONF_NINE_KEY         '8'
#define      CONF_TEN_KEY          '9'

#define      READ                  'r'
#define      WRITE                 'w'
#define      APPEND                'a'
#define      OPTIONAL              'o'

#define      RIVALROUS             'r'
#define      NONRIVALROUS          'n'
#define      MIXED                 'm'

#define      GOGGLEMODE            'g'
#define      SPLITMODE             's'
#define      NORMALMODE            'n'

#define      NOTARGET              'n'
#define      TARGET                't'

#define      NOTASK                'n'
#define      LANDOLTTASK           'l'
#define      POSITIONTASK          'p'
#define      CONFIDENCETASK        'c'
#define      ALLTASKS              'a'

#define      UNIFORM               'u'
#define      RANDOM                'r'

#define      ESSDISPLAY            'e'
#define      CEEDISPLAY            'c'

#define      CIRCLE_UP              1
#define      CIRCLE_DOWN            0

#define      CLOCK_ESS              1
#define      COUNTER_ESS            0

#define      RANDOM_FOREG_TEXTURE   "random"
#define      CONCENTRIC_FOREG_TEXTURE "concentric"
#define      RADIAL_FOREG_TEXTURE   "radial"

#define GETINT(name) {fgets(line,120,fp);sscanf(line,"%s %d",text,&(name));}
#define GETCHAR(name) {fgets(line,120,fp);sscanf(line,"%s %c",text,&(name));}
#define GETFLOAT(name) {fgets(line,120,fp);sscanf(line,"%s %f",text,&(name));}
#define GETTEXT(name) {fgets(line,120,fp);sscanf(line,"%s %s",text,(name));}
#define GETINTL(name) {fgets(line,120,fp);sscanf(line,"%s %d %d %d %d",text,&(name)[0],&(name)[1],&(name)[2],&(name)[3]);}
#define GETTEXTL(name) {fgets(line,120,fp);sscanf(line,"%s %s %s %s %s",text,(name)[0],(name)[1],(name)[2],(name)[3]);}

#define PUTINT(name,text) {fprintf(fp,"%-19s %d\n",(text),(name));}
#define PUTCHAR(name,text) {fprintf(fp,"%-19s %c\n",(text),(name));}
#define PUTFLOAT(name,text) {fprintf(fp,"%-19s %.2f\n",(text),(name));}
#define PUTTEXT(name,text) {fprintf(fp,"%-19s %s\n",(text),(name));}
#define PUTINTL(name,text) {fprintf(fp,"%-19s %d %d %d %d\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}
#define PUTTEXTL(name,text) {fprintf(fp,"%-19s %s %s %s %s\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}

#define PRINTINT(name,text) {printf( "%-19s %d\n",(text),(name));}
#define PRINTCHAR(name,text) {printf( "%-19s %c\n",(text),(name));}
#define PRINTFLOAT(name,text) {printf( "%-19s %.2f\n",(text),(name));}
#define PRINTTEXT(name,text) {printf( "%-19s %s\n",(text),(name));}
#define PRINTINTL(name,text) {printf( "%-19s %d %d %d %d\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}
#define PRINTTEXTL(name,text) {printf( "%-19s %s %s %s %s\n",(text),(name)[0],(name)[1],(name)[2],(name)[3]);}

extern char line[], text[];

