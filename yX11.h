/*===[[ START HDOC ]]=========================================================*/


/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YX11   
#define YX11   loaded



#include <X11/X.h>                   /* main header                           */
#include <X11/Xlib.h>                /* c-api (xlib) header                   */



Display       *YX_DISP;
int            YX_SCRN;
Window         YX_BASE;
Window         YX_ROOT;
XEvent         YX_EVNT;
XKeyEvent      YX_SKEY;

#define    YX_FOCUSABLE   'y'
#define    YX_HIDDEN      'n'

#define    YX_SIZEABLE    'y'
#define    YX_FIXED       'n'

#define    YX_VERBOSE     'y'
#define    YX_SILENT      'n'

typedef    unsigned long   ulong;

/*===[[ PROTOTYPES ]]=========================================================*/
char      *yX11_version            (void);
char       yX11_start              (char*, int, int, char, char, char);
char       yX11_screensize         (int *a_wide, int *a_tall, int *a_deep);
char       yX11_resize             (int a_wide, int a_tall);
char       yX11_move               (int a_xpos, int a_ypos);
char       yX11_end                (void);
ulong      yXINIT_focus            (void);


#endif


