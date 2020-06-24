/*===[[ START HDOC ]]=========================================================*/


/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YX11   
#define YX11   loaded



#include <X11/X.h>                   /* main header                           */
#include <X11/Xlib.h>                /* c-api (xlib) header                   */

/*---(moves)------------------------*/
#define  YX_NEXTWIN    'h'
#define  YX_PREVWIN    'd'
#define  YX_NEXTDESK   'k'
#define  YX_PREVDESK   'b'
#define  YX_NEXTWITH   'K'
#define  YX_PREVWITH   'B'
/*---(positions)--------------------*/


Display       *YX_DISP;
int            YX_SCRN;
Window         YX_BASE;
Window         YX_ROOT;
Window         YX_FOCU;
XEvent         YX_EVNT;
XKeyEvent      YX_SKEY;

#define    YX_FOCUSABLE   'y'
#define    YX_HIDDEN      'n'

#define    YX_SIZEABLE    'y'
#define    YX_FIXED       'n'

#define    YX_VERBOSE     'y'
#define    YX_SILENT      'n'

typedef    unsigned long   ulong;
typedef    unsigned int    uint;
typedef    unsigned char   uchar;

/*===[[ PROTOTYPES ]]=========================================================*/
char       *yX11_version            (void);
char        yX11_start              (char*, int, int, char, char, char);
char        yX11_screensize         (int *a_wide, int *a_tall, int *a_deep);
char        yX11_resize             (int a_wide, int a_tall);
char        yX11_move               (int a_xpos, int a_ypos);
char        yX11_end                (void);
ulong       yX11_focus            (void);


/*345678901-12345678901-12345678901-12345678901-12345678901-12345678901-123456*/
char        yX11_window_names       (int a_max, int a_len, char *a_names []);

/*
 * rational limits...
 *   desktop names truncated at LEN_LABEL (20)
 *   window names truncated at  LEN_HUND  (100)
 *
 * all functions return index of current, and name in return a_name
 *
 *
 */

/*---(informational-only)-------------*/
char        yX11_desktop_current    (char *a_name);
char        yX11_desktop_cursor     (char a_move, char *a_name);
char        yX11_desktop_exists     (char *a_regex);
/*---(updating)-----------------------*/
char        yX11_desktop_move       (char a_move, char *a_name);
char        yX11_desktop_activate   (char *a_regex, char *a_name, int *a_count);


char        yX11_window_current     (char *a_name, char *a_desk);
char        yX11_window_cursor      (char *a_desk, char a_move, char *a_name);
char        yX11_window_exists      (char *a_desk, char *a_regex);

int         yX11_window_regex       (char *a_desk, char *a_regex, char *a_name);
int         yX11_window_exist       (char *a_desk, char *a_regex, char *a_name);
int         yX11_window_title       (char *a_desk, char *a_regex, char *a_name);



char        yX11_window_verify      (char *a_regex);
char        yX11_window_activate    (char *a_regex);
char        yX11_window_id          (char *a_regex);

/*---(act on current)-----------------*/
long        yX11_term               (char a_desk, char a_abbr, char a_scrn, char *a_color);

char        yX11_place              (long a_win, char a_desk, char a_abbr, char a_scrn);
char        yX11_exact              (long a_win, char d, int x, int y, int w, int t);
char        yX11_where              (long a_win, char *d, int *x, int *y, int *w, int *t); 


char        yX11_keys_send          (long a_window, uchar *a_keys);

#endif


