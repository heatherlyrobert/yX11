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

extern Display       *YX_DISP;
extern int            YX_SCRN;
extern Window         YX_BASE;
extern Window         YX_ROOT;
extern Window         YX_FOCU;
extern XEvent         YX_EVNT;
extern XKeyEvent      YX_SKEY;


#define    YX_FOCUSABLE   'y'
#define    YX_HIDDEN      'n'

#define    YX_SIZEABLE    'y'
#define    YX_FIXED       'n'

#define    YX_VERBOSE     'y'
#define    YX_SILENT      'n'

typedef    unsigned long   ulong;
typedef    unsigned int    uint;
typedef    unsigned char   uchar;
typedef    struct dirent   tDIRENT;

/*===[[ PROTOTYPES ]]=========================================================*/
char       *yX11_version            (void);
char        yX11_reset              (void);
char        yX11_start              (char*, int, int, char, char, char);
char        yX11_screensize         (int *a_wide, int *a_tall, int *a_deep);
char        yX11_resize             (int a_wide, int a_tall);
char        yX11_move               (int a_xpos, int a_ypos);
char        yX11_end                (void);
ulong       yX11_focus              (void);


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
char        yX11_desk_current       (char *r_name);
char        yX11_desk_cursor        (char a_move, char *r_name);
char        yX11_desk_find          (char *a_regex, char *r_name, int *r_count);
char        yX11_desk_verify        (char a_desk);
/*---(updating)-----------------------*/
char        yX11_desk_goto          (char a_move);







/*---(act on current)-----------------*/
long        yX11_term               (char a_desk, char a_abbr, char a_scrn, char *a_color);
/*---(positioning)--------------------*/
char        yX11_win_place          (long a_win, char a_desk, char a_abbr, char a_scrn);
char        yX11_win_exact          (long a_win, char d, int x, int y, int w, int t);
char        yX11_win_where          (long a_win, char *d, int *x, int *y, int *w, int *t); 

char        yX11_win_push           (int x, int y, int w, int t);
char        yX11_win_pop            (void);

char        yX11_win_by_cursor      (char a_move, long *r_id, char *r_desk, char *r_title, char *r_type, char *r_curr, short *r_left, short *r_topp, char *a_scrn, char *a_locn, short *r_wide, short *r_tall, char *a_size);
long        yX11_win_current        (char *a_name, char *a_desk);
char        yX11_win_verify         (long a_win);
long        yX11_win_find           (char *a_regex, char *a_name, int *a_count);

char        yX11_win_bring          (long a_win);
char        yX11_win_goto           (long a_win);
char        yX11_win_send           (long a_win, char a_desk);
char        yX11_win_take           (long a_win, char a_desk);


char        yX11_keys_send          (long a_window, uchar *a_keys);
char        yX11_keys_send_current  (uchar *a_keys);
char        yX11_keys_blitz         (long a_window, uchar *a_keys);


char        yX11_yvikeys_term       (uchar *a_string);
char        yX11_yvikeys_sendkeys   (uchar *a_string);
char        yX11_yvikeys_blitzkeys  (uchar *a_string);

char        yX11_yvikeys_winexact   (uchar a_abbr, char d, int x, int y, int w, int t);
char        yX11_yvikeys_winplace   (char *a_string);
char        yX11_yvikeys_winbring   (uchar a_abbr);
char        yX11_yvikeys_wingoto    (uchar a_abbr);
char        yX11_yvikeys_winsend    (uchar a_abbr, char a_desk);
char        yX11_yvikeys_wintake    (uchar a_abbr, char a_desk);

char        yX11_yvikeys_mydesk     (char d);
char        yX11_yvikeys_myhome     (int x, int y);
char        yX11_yvikeys_mysize     (int w, int t);
char        yX11_yvikeys_mysizer    (int w, int t, int m, int s);

char        yX11_yvikeys_name       (uchar *a_string);

#endif


