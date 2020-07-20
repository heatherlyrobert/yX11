/*============================----beg-of-source---============================*/
#ifndef yX11_PRIV
#define yX11_PRIV loaded



/*===[[ BEG_HEADER ]]=========================================================*/
/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-12345678901-*/

/*===[[ ONE_LINERS ]]=========================================================*/

#define     P_FOCUS     "RS (run-time support)"
#define     P_NICHE     "xw (x-windows)"
#define     P_PURPOSE   "provide consistent and simple X11/OpenGL setup/teardown"

#define     P_NAMESAKE  "terpsichore"
#define     P_HERITAGE  "greek muse of dance and chorus"
#define     P_IMAGERY   "beautiful woman sitting with a lyre"
#define     P_REASON    "the most complicated system dance is coordinating graphics"

#define     P_SYSTEM    "gnu/linux   (powerful, ubiquitous, technical, and hackable)"
#define     P_LANGUAGE  "ansi-c      (wicked, limitless, universal, and everlasting)"
#define     P_CODESIZE  "small       (less than 1,000 slocl)"

#define     P_AUTHOR    "heatherlyrobert"
#define     P_CREATED   "2009-05"
#define     P_DEPENDS   "xlib/glx"

#define     P_VERMAJOR  "1.--  working for everyday use, evolving but stable"
#define     P_VERMINOR  "1.1-  adding desktop control for zeus and scripting"
#define     P_VERNUM    "1.1c"
#define     P_VERTXT    "created windowing and keystroke functions for kairos"

#define     P_PRIORITY  "direct, simple, brief, vigorous, and lucid (h.w. fowler)"
#define     P_PRINCIPAL "[grow a set] and build your wings on the way down (r. bradbury)"
#define     P_REMINDER  "there are many better options, but i *own* every byte of this one"

/*===[[ END_HEADER ]]=========================================================*/

/*---[[ HEADER ]]-------------------------------------------------*

 *   niche         : xlib/opengl    (xwindows and opengl)
 *   application   : yX11           (standardize init and wrap-up)
 *   purpose       : provide consistent and simple X11/OpenGL setup/teardown
 *   base_system   : gnu/linux
 *   lang_name     : ansi-c         (because its right, just, and proper)
 *   created       : 2009-05        (near the start of my opengl training)
 *   author        : the_heatherlys
 *   dependencies  : xlib/glx
 *
 */
/*---[[ PURPOSE ]]------------------------------------------------*

 *   yXINIT is a set of library functions to provide highly reliable, testable,
 *   and consistent connection, setup, and destruction support for both XLIB for
 *   x-windows and GLX for opengl.  because every application has to accomplish
 *   these functions in a nearly consistent fashion, it is simple to ignore
 *   basic checks and safety precautions because it hasn't happened in a while.
 *   also, it is very easy to forget all the great things learned in this area
 *   as it is not a consistent focus.  the goal is to take a painful, tedious,
 *   onerous, haphazard, and error prone process and turn it a easy,
 *   flexible, predictable, rapid, and automated set of functions.
 *
 *   yXINIT will focus on...
 *      - establishing a connection with the x-server
 *      - gathering the key statistics that can be learned about that server
 *      - establishing a connection and visual for opengl
 *      - creating and mapping a window
 *      - getting default graphics contexts in place
 *      - destroying both the x-windows and opengl connections
 *      - putting a simple error handler in place
 *
 *   yXINIT will NOT provide...
 *      - event handling
 *      - any drawing
 *      - window handling outside of initial mapping
 *
 *   there are many existing libraries and utilities have been built by better
 *   programmers and are likely superior in speed, size, capability, and
 *   reliability; BUT, i would not have learned nearly as much using them,
 *   so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the basic idea is to take the great ideas from existing frameworks
 *   without taking all the unneeded complexity that comes from backwards
 *   compatiblilty, odd viewpoints, and traditional understandings.
 *
 */
/*---[[ END HDOC ]]-----------------------------------------------------------*/



/*===[[ XLIB HEADERS ]]=======================================================*/
#include    <X11/Xutil.h>
#include    <X11/Xatom.h>               /* for setting up new protocol messages  */
#include    <X11/keysym.h>              /* for resolving keycodes/keysyms        */


/*===[[ STANDARD ]]===========================================================*/
#include    <stdio.h>        /* C_ANSI : strcpy, strlen, strchr, strcmp, ...  */
#include    <string.h>       /* C_ANSI : printf, snprintf, fgets, fopen, ...  */
#include    <stdlib.h>       /* C_ANSI : exit                                 */
#include    <memory.h>       /* C_ANSI : malloc                               */
#include    <sys/time.h>     /* C_UNIX : gettimeofday                         */


/*===[[ GLX HEADERS ]]========================================================*/
#include <GL/gl.h>                   /* main header for opengl                */
#include <GL/glx.h>                  /* for xlib/opengl integration           */


/*===[[ CLIB HEADERS ]]=======================================================*/
#include    <yURG.h>         /* CUSTOM : heatherly urgent processing          */
#include    <yLOG.h>         /* CUSTOM : heatherly program logging            */
#include    <ySTR.h>         /* CUSTOM : heatherly string handling            */
#include    <yREGEX.h>       /* CUSTOM : heatherly regular expressions        */


/*===[[ TYPEDEFS ]]===========================================================*/
typedef    unsigned long ulong;
typedef    unsigned int  uint;
typedef    unsigned char uchar;

extern char   unit_answer [LEN_RECD];

/*===[[ GLOBALS ]]============================================================*/
struct cXINIT   {
   char           name[300];
   /*---(from xlib_connect)------------*/
   /*> Display       *disp;                                                           <*/
   /*> int            scrn;                                                           <*/
   Screen        *scrn_ptr;
   Window         focu;
   char           focu_name[300];
   char           focusable;
   char           resizeable;
   /*---(from glx_connect)-------------*/
   XVisualInfo   *visu;
   /*---(from xlib_setup)--------------*/
   Colormap       cmap;
   /*> Window         base;                                                           <*/
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

extern  int   s_ndesk;
extern  int   s_nwin;


/*===[[ DATA MACROS ]]========================================================*/
#define    VERBOSE         gXINIT.verbose
#define    DEBUG_YXINIT    if (VERBOSE == 'y')

#define    SCRN_PTR   gXINIT.scrn_ptr
#define    FOCU_NAME  gXINIT.focu_name
#define    CAN_FOCUS  gXINIT.focusable
#define    CAN_SIZE   gXINIT.resizeable
#define    VISU       gXINIT.visu
#define    CMAP       gXINIT.cmap
#define    GCON       gXINIT.gcon
#define    HINTS      gXINIT.wmhints

#define    TITLE      gXINIT.name
#define    X          gXINIT.x
#define    Y          gXINIT.y
#define    WIDTH      gXINIT.w
#define    HEIGHT     gXINIT.h



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        yx11_base_defense       (void);
char        yX11_base__connect      (void);
char        yX11_base__create       (void);
char        yX11_base__destroy      (void);

char        yX11_opengl__connect    (void);
char        yX11_opengl__create     (void);
char        yX11_opengl__destroy    (void);

int         yX11__error             (Display *a_disp, XErrorEvent *a_event);


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        yX11__unit_quiet        (void);
char        yX11__unit_loud         (void);
char        yX11__unit_end          (void);

/*---(counters)-------------*/
char        yx11_desk_addwin        (char a_desk);
char        yx11_desk_subwin        (char a_desk);
/*---(program)--------------*/
char        yx11_desk_purge         (void);
char        yx11_desk_init          (void);
/*---(inventory)------------*/
char        yx11_desk_inventory     (char a_real);
char        yx11_full_refresh       (char a_real);

char*       yX11__unit_desk         (char *a_question, int a_num);



/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
/*---(program)--------------*/
char        yx11_win_purge          (void);
char        yx11_win_init           (void);
/*---(inventory)------------*/
char        yx11_win_inventory      (char a_real);
char        yx11_win_current        (char a_real);
char        yx11_win__activate      (long a_win, char a_desk);
char        yx11_win__verify        (long a_win);
long        yx11_win__regex         (char *a_regex, char *a_name, int *a_count);
char*       yX11__unit_win          (char *a_question, long a_num);


char        yx11_keys__normal       (uchar a_ch);
char        yx11_keys__extended     (uchar a_ch);
char        yx11_keys__extra        (uchar a_ch);
char*       yX11__unit_keys         (char *a_question, int a_num);
char        yx11__unit_keys_read    (void);


char        yx11_loc_init           (void);
char        yx11_loc_sizing         (char a_abbr, char a_scrn, int *a_left, int *a_topp, int *a_wide, int *a_tall);
char*       yX11__unit_loc          (char *a_question, int a_num, char a_scrn);

char        yx11_yvikeys_purge      (void);
char        yx11_yvikeys_valid      (uchar a_abbr);
char        yx11_yvikeys__desktop   (char a_desk);
char*       yX11__unit_yvikeys      (char *a_question, int a_num);

#endif

