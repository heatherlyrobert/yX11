/*============================----beg-of-source---============================*/ #ifndef yVIKEYS_PRIV
#ifndef yX11_PRIV
#define yX11_PRIV loaded



/*===[[ BEG_HEADER ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

/*===[[ ONE_LINERS ]]=========================================================*/

#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "xw (x-windows)"
#define     P_PURPOSE   "provide consistent and simple X11/OpenGL setup/teardown"

#define     P_NAMESAKE  ""
#define     P_HERITAGE  ""
#define     P_IMAGERY   ""

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (less than 1,000 slocl)"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2009-05"
#define     P_DEPENDS   "xlib/glx"

#define     P_VERMAJOR  "1.--  working for everyday use, evolving but stable"
#define     P_VERMINOR  "1.0-  production fixes and small enhancements"
#define     P_VERNUM    "1.0e"
#define     P_VERTXT    "added screensize function to return width and height (useful)"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*===[[ END_HEADER ]]=========================================================*/



/*===[[ XLIB HEADERS ]]=======================================================*/
#include <X11/X.h>                   /* main header                           */
#include <X11/Xlib.h>                /* c-api (xlib) header                   */
#include <X11/Xutil.h>
#include <X11/Xatom.h>               /* for setting up new protocol messages  */
#include <X11/keysym.h>              /* for resolving keycodes/keysyms        */


/*===[[ GLX HEADERS ]]========================================================*/
#include <GL/gl.h>                   /* main header for opengl                */
#include <GL/glx.h>                  /* for xlib/opengl integration           */


/*===[[ CLIB HEADERS ]]=======================================================*/
#include <stdio.h>



/*===[[ TYPEDEFS ]]===========================================================*/
typedef    unsigned long ulong;
typedef    unsigned int  uint;
typedef    unsigned char uchar;


/*===[[ GLOBALS ]]============================================================*/
struct cXINIT   {
   char           name[300];
   /*---(from xlib_connect)------------*/
   Display       *disp;
   int            scrn;
   Screen        *scrn_ptr;
   Window         root;  /* Window = unsigned long  */
   Window         focu;
   char           focu_name[300];
   char           focusable;
   char           resizeable;
   /*---(from glx_connect)-------------*/
   XVisualInfo   *visu;
   /*---(from xlib_setup)--------------*/
   Colormap       cmap;
   Window         base;
   GC             con1;
   GC             con2;
   XWMHints       wmhints;
   /*---(from glx_setup)---------------*/
   GLXContext     gcon;
   /*---(from main loop)---------------*/
   XEvent         evnt;
   XKeyEvent      skey;
   int            x, y;
   uint           w, h;
   /*---(other)------------------------*/
   char           verbose;
};
typedef struct cXINIT tXINIT;
extern  tXINIT  gXINIT;



/*===[[ DATA MACROS ]]========================================================*/
#define    VERBOSE         gXINIT.verbose
#define    DEBUG_YXINIT    if (VERBOSE == 'y')

#define    DISP       gXINIT.disp
#define    SCRN       gXINIT.scrn
#define    SCRN_PTR   gXINIT.scrn_ptr
#define    ROOT       gXINIT.root
#define    FOCU       gXINIT.focu
#define    FOCU_NAME  gXINIT.focu_name
#define    CAN_FOCUS  gXINIT.focusable
#define    CAN_SIZE   gXINIT.resizeable
#define    VISU       gXINIT.visu
#define    CMAP       gXINIT.cmap
#define    BASE       gXINIT.base
#define    EVNT       gXINIT.evnt
#define    SKEY       gXINIT.skey
#define    GCON       gXINIT.gcon
#define    HINTS      gXINIT.wmhints

#define    TITLE      gXINIT.name
#define    X          gXINIT.x
#define    Y          gXINIT.y
#define    WIDTH      gXINIT.w
#define    HEIGHT     gXINIT.h






#endif
