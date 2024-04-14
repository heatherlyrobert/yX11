#include "yX11.h"
#include "yX11_priv.h"

Display       *YX_DISP;
int            YX_SCRN;
Window         YX_BASE;
Window         YX_ROOT;
Window         YX_FOCU;
XEvent         YX_EVNT;
XKeyEvent      YX_SKEY;

#define   DEBUG      if (gXINIT.verbose) 

tXINIT  gXINIT;

static char      yX11_ver [200] = "";

char*      /* ---- : return library versioning information -------------------*/
yX11_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built  ]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc    ]", 15);
#else
   strncpy (t, "[unknown    ]", 15);
#endif
   snprintf (yX11_ver, 100, "%s   %s : %s", t, P_VERNUM, P_VERTXT);
   return yX11_ver;
}

/* establish the X11/xlib connection to the server ---------------------------*
 *    0 = success
 *   -1 = xopendisplay failed
 *   -3 = rootwindow failed
 *   -4 = xgetinputfocus failed
 */

/*   LEAF    - internal use only, no user functions called
 *   PETAL   - external use, no user functions called
 *   ROOT    - external use, calls other user functions
 *   BRANCH  - internal use only, calls othe user functions
 */


char
yx11_base_defense       (void)
{
   if (YX_DISP == NULL)   return -1;
   return 0;
}

char
yX11_reset              (void)
{
   yx11_desk_init ();
   yx11_win_init  ();
   yx11_loc_init  ();
   yx11_full_refresh  ('y');
   return 0;
}

char
yX11_start_unit         (void)
{
   char        rc          =    0;
   /*---(header)---------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   DEBUG_YXINIT printf("   - start executing setup functions\n\n");
   rc = yX11_base__connect   ();
   DEBUG_DESK    yLOG_value   ("b connect" , rc);
   rc = yX11_opengl__connect ();
   DEBUG_DESK    yLOG_value   ("o connect" , rc);
   rc = yX11_reset           ();
   DEBUG_DESK    yLOG_value   ("reset"     , rc);
   /*---(complete)-------------------------*/
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_cmdline            (void)
{
   char        rc          =    0;
   /*---(header)---------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   DEBUG_YXINIT printf("   - start executing setup functions\n\n");
   rc = yX11_base__connect   ();
   DEBUG_DESK    yLOG_value   ("b connect" , rc);
   rc = yX11_reset           ();
   DEBUG_DESK    yLOG_value   ("reset"     , rc);
   /*---(complete)-------------------------*/
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}


char
yX11_start (
      char     *a_name,                /* main window name                    */
      int       a_width,               /* window width                        */
      int       a_height,              /* window height                       */
      char      a_focusable,           /* will/should it accept focus         */
      char      a_resizeable,          /* will/should it resize               */
      char      a_verbose)             /* debugging noise desired             */
{
   char        rc          =    0;
   /*---(header)---------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   /*---(start)----------------------------*/
   VERBOSE = 'n';
   if (a_verbose == 'y') VERBOSE = 'y';
   DEBUG_YXINIT printf("yXINIT -- heatherly xlib/glx setup ------------------------------------------ (start)\n\n");
   /*---(establish verbosity)--------------*/
   DEBUG_YXINIT printf("starting up...\n");
   DEBUG_YXINIT printf("   - setting verbosity . . . . . . . . . . %d\n", VERBOSE);
   DEBUG_DESK   yLOG_value   ("verbosity" , VERBOSE);
   DEBUG_YXINIT printf("   - record window name  . . . . . . . . . ");
   strncpy(TITLE, a_name, 100);
   DEBUG_YXINIT printf("%-30.30s\n", TITLE);
   DEBUG_DESK   yLOG_info    ("title"     , TITLE);
   DEBUG_YXINIT printf("   - record suggested size . . . . . . . . ");
   WIDTH  = a_width;
   HEIGHT = a_height;
   DEBUG_YXINIT printf("%dw, %dh\n", WIDTH, HEIGHT);
   DEBUG_YXINIT printf("   - focusable attribute . . . . . . . . . ");
   DEBUG_DESK   yLOG_complex ("size"      , "%4dw, %4dh", WIDTH, HEIGHT);
   CAN_FOCUS = a_focusable;
   CAN_SIZE  = a_resizeable;
   DEBUG_YXINIT printf("%c\n", CAN_FOCUS);
   DEBUG_DESK   yLOG_char    ("focus"     , CAN_FOCUS);
   /*---(reset summary counters)-------*/
   DEBUG_YXINIT printf("   - start executing setup functions\n\n");
   rc = yX11_base__connect   ();
   DEBUG_DESK    yLOG_value   ("b connect" , rc);
   rc = yX11_opengl__connect ();
   DEBUG_DESK    yLOG_value   ("o connect" , rc);
   rc = yX11_base__create    ();
   DEBUG_DESK    yLOG_value   ("b create"  , rc);
   rc = yX11_opengl__create  ();
   DEBUG_DESK    yLOG_value   ("o create"  , rc);
   rc = yX11_reset           ();
   DEBUG_DESK    yLOG_value   ("reset"     , rc);
   DEBUG_YXINIT printf("yXINIT -- heatherly xlib/glx setup -------------------------------------------- (end)\n\n");
   /*---(complete)-------------------------*/
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_screensize      (int *a_wide, int *a_tall, int *a_deep)
{
   if (YX_DISP == NULL)   return -1;
   if (a_wide != NULL)  *a_wide = DisplayWidth   (YX_DISP, YX_SCRN);
   if (a_tall != NULL)  *a_tall = DisplayHeight  (YX_DISP, YX_SCRN);
   if (a_deep != NULL)  *a_deep = DefaultDepth   (YX_DISP, YX_SCRN);
   return 0;
}

char
yX11_resize          (int a_wide, int a_tall)
{
   /*---(header)-------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   DEBUG_DESK    yLOG_value   ("a_wide"    , a_wide);
   DEBUG_DESK    yLOG_value   ("a_tall"    , a_tall);
   DEBUG_DESK    yLOG_point   ("YX_DIST"   , YX_DISP);
   DEBUG_DESK    yLOG_point   ("YX_BASE"   , YX_BASE);
   /*---(shut the old down)----------------*/
   XResizeWindow (YX_DISP, YX_BASE, a_wide, a_tall);
   /*---(complete)-------------------------*/
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_move            (int a_xpos, int a_ypos)
{
   /*---(shut the old down)----------------*/
   XMoveWindow (YX_DISP, YX_BASE, a_xpos, a_ypos);
   /*---(complete)-------------------------*/
   return 0;
}

char
yX11_end             (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   rc = yX11_opengl__destroy();
   DEBUG_DESK    yLOG_value   ("opengl"    , rc);
   rc = yX11_base__destroy();
   DEBUG_DESK    yLOG_value   ("base"      , rc);
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}

int
yX11__error       (Display *a_disp, XErrorEvent *a_event)
{
   return 0;
}

char
yX11_base__connect        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         trash;
   /*---(header)-------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   DEBUG_DESK    yLOG_note    ("x11/xlib connection (YX_DISP, YX_SCRN, YX_ROOT, FOCU)");
   /*---(connect)------------------------*/
   YX_DISP   = XOpenDisplay(NULL);      /* pull the default                 */
   DEBUG_DESK    yLOG_point   ("xserver"   , YX_DISP);
   --rce;  if (YX_DISP == NULL) {
      DEBUG_DESK    yLOG_note    ("server connection failed");
      DEBUG_DESK    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(display server info)------------*/
   YX_SCRN   = DefaultScreen(YX_DISP);
   SCRN_PTR  = DefaultScreenOfDisplay(YX_DISP);
   DEBUG_DESK    yLOG_value   ("number"    , ConnectionNumber(YX_DISP));
   DEBUG_DESK    yLOG_info    ("vendor"    , ServerVendor(YX_DISP));
   DEBUG_DESK    yLOG_value   ("release"   , VendorRelease(YX_DISP));
   DEBUG_DESK    yLOG_complex ("protocol"  , "%dv, %dr", ProtocolVersion(YX_DISP), ProtocolRevision(YX_DISP));
   DEBUG_DESK    yLOG_info    ("display"   , DisplayString(YX_DISP));
   DEBUG_DESK    yLOG_value   ("screens"   , ScreenCount(YX_DISP));
   DEBUG_DESK    yLOG_value   ("current"   , YX_SCRN);
   DEBUG_DESK    yLOG_point   ("pointer"   , SCRN_PTR);
   DEBUG_DESK    yLOG_complex ("size"      , "%dw, %dh",DisplayWidth(YX_DISP, YX_SCRN), DisplayHeight(YX_DISP, YX_SCRN));
   DEBUG_DESK    yLOG_value   ("depth"     , DefaultDepth(YX_DISP, YX_SCRN));
   /*---(show the root window)-----------*/
   YX_ROOT   = RootWindow(YX_DISP, YX_SCRN);
   DEBUG_DESK    yLOG_point   ("root"      , YX_ROOT);
   --rce;  if (YX_ROOT == 0) {
      DEBUG_DESK    yLOG_note    ("root window empty");
      DEBUG_DESK    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(show the colormap entries)------*/
   DEBUG_DESK    yLOG_value   ("colormap"  , DisplayCells (YX_DISP, YX_SCRN));
   DEBUG_DESK    yLOG_value   ("backing"   , DoesBackingStore (SCRN_PTR));
   /*---(current focus window)-----------*/
   XGetInputFocus(YX_DISP, &YX_FOCU, &trash);
   DEBUG_DESK    yLOG_point   ("focus"     , YX_FOCU);
   --rce;  if (YX_FOCU == 0) {
      DEBUG_DESK    yLOG_note    ("focus window empty");
      DEBUG_DESK    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*------------------------------------*/
   DEBUG_DESK    yLOG_note    ("setup error handler");
   XSetErrorHandler (yX11__error);
   /*---(complete)-----------------------*/
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_base__create        (void)
{
   /*---(assumptions)---------------------------*/
   /* only options are to set the width, height, title, and focusable
    * - top level windows should not fight WM about size or location (hint only)
    * - set standard window properties before mapping
    * these drops of wisdom come from Tronche
    */
   /*---(defense)-------------------------------*/
   if (yx11_base_defense () < 0)  return -1;
   /*---(header)-------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   DEBUG_YXINIT printf("x11/xlib main window (CMAP, YX_BASE, CON1, CON2)...¦");
   XSetWindowAttributes   attr;
   XColor         xc1, xc2;
   /*-------------------------------------------*/
   DEBUG_YXINIT printf("   - access default colormap . . .  (CMAP) ");
   CMAP       = DefaultColormap(YX_DISP, YX_SCRN);
   attr.colormap  = CMAP;
   DEBUG_YXINIT printf("success\n");
   /*-------------------------------------------*/
   DEBUG_YXINIT printf("   - assign background color . . . . . . . ");
   XAllocNamedColor(YX_DISP, CMAP, "black"      ,  &xc1, &xc2);
   attr.background_pixel = xc1.pixel;
   DEBUG_YXINIT printf("success\n");
   /*-------------------------------------------*/
   /*  display_name, parent_window,
    *  top_left_x, top_left_y, width, height, border_width,
    *  depth, class, visual, valuemask, attributes
    */
   DEBUG_YXINIT printf("   - create base window  . . . . (my.base) ");
   YX_BASE = XCreateWindow(YX_DISP, YX_ROOT,
         0, 0, WIDTH, HEIGHT, 0,
         CopyFromParent, InputOutput, CopyFromParent,
         CWBackPixel|CWColormap, &attr);
   if (YX_BASE == 0) {
      DEBUG_YXINIT printf("NULL, EXITING\naborted\n");
      return -2;
   }
   DEBUG_YXINIT printf("%ld¦", YX_BASE);
   /*-------------------------------------------*
    * this is the proper way to change the window title
    */
   DEBUG_YXINIT printf("   - change the title  . . . . . . . . . . ");
   XStoreName(YX_DISP, YX_BASE, TITLE);
   DEBUG_YXINIT printf("success\n");
   /*-------------------------------------------*
    * this section makes sure the petal window *NEVER* grabs the focus
    */
   DEBUG_YXINIT printf("   - set the WM hints  . . . . . . . . . . ");
   HINTS.flags  = InputHint;
   if (CAN_FOCUS == 'n')  HINTS.input  = 0;
   else                   HINTS.input  = 1;
   XSetWMHints(YX_DISP, YX_BASE, &HINTS);
   DEBUG_YXINIT printf("success\n");
   /*---(set the size hints)--------------------*/
   DEBUG_YXINIT printf("   - set the size hints  . . . . . . . . . ");
   XSizeHints  hints;
   hints.flags  = PSize|PMinSize|PMaxSize;
   hints.base_width  = WIDTH;
   hints.base_height = HEIGHT;
   if (CAN_SIZE == 'n') {
      hints.min_width  = hints.max_width  = hints.base_width  = WIDTH;
      hints.min_height = hints.max_height = hints.base_height = HEIGHT;
   }
   XSetWMNormalHints(YX_DISP, YX_BASE, &hints);
   DEBUG_YXINIT printf("success\n");
   /*---(mask events)---------------------------*/
   DEBUG_YXINIT printf("   - update the full event mask  . . . . . ");
   XSelectInput(YX_DISP, YX_BASE, KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonMotionMask|ButtonReleaseMask|ExposureMask|StructureNotifyMask|FocusChangeMask);
   /*  PointerMotionMask    -- movement regardless of button presses
    *  ButtonMotionMask     -- movement when a button is pressed
    *  Button<n>MotionMask  -- movement when button <n> is pressed
    */
   DEBUG_YXINIT printf("success\n");
   /*--------------------------s----------------*/
   DEBUG_YXINIT printf("   - map the window  . . . . . . . . . . . ");
   XMapWindow(YX_DISP, YX_BASE);  /* alternative is XMapRaised() */
   DEBUG_YXINIT printf("success\n");
   /*-------------------------------ss----------*/
   DEBUG_YXINIT printf("   - flush the event queue . . . . . . . . ");
   XFlush(YX_DISP);
   DEBUG_YXINIT printf("success\n");
   /*-------------------------------------------*/
   DEBUG_YXINIT printf("   - get the initial location  . . . . . . ");
   ulong       trash1;
   uint        trash2;
   XGetGeometry(YX_DISP, YX_BASE, &trash1, &X, &Y, &WIDTH, &HEIGHT, &trash2, &trash2);
   DEBUG_YXINIT printf("%dx, %dy, %dw, %dh\n", X, Y, WIDTH, HEIGHT);
   /*---(complete)------------------------------*/
   DEBUG_YXINIT printf("   - done\n\n");
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_base__destroy      (void)
{
   char        rce         =  -10;
   char        rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK    yLOG_enter   (__FUNCTION__);
   DEBUG_DESK    yLOG_note    ("wrapping up of xwindows connection");
   /*---(defense)-------------------------------*/
   rc = yx11_base_defense ();
   DEBUG_DESK    yLOG_value   ("defense"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK    yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(wipe window)---------------------------*/
   DEBUG_DESK    yLOG_note    ("unmap window");
   XUnmapWindow   (YX_DISP, YX_BASE);
   DEBUG_DESK    yLOG_note    ("destroy window");
   XDestroyWindow (YX_DISP, YX_BASE);
   /*---(disconnect)----------------------------*/
   DEBUG_DESK    yLOG_note    ("close connection");
   /*> XCloseDisplay  (YX_DISP);                                                      <*/
   /*---(complete)------------------------------*/
   DEBUG_DESK    yLOG_exit    (__FUNCTION__);
   return 0;
}


ulong            /* [------] find the currrently focused window -----------------------------*/
yX11_focus            (void)
{
   Window x_focus;
   int    x_waste;
   char  *x_name;
   static    int x_runs  = 0;
   /*---(defense)-------------------------------*/
   if (yx11_base_defense () < 0)  return -1;
   ++x_runs;
   DEBUG_YXINIT printf("F> START xlib_focus() : root=%ld, base=%ld, run=%d¦", YX_ROOT, YX_BASE, x_runs);
   XGetInputFocus (YX_DISP, &x_focus, &x_waste);
   DEBUG_YXINIT printf("F> FIND  xlib_focus() : focus window = %ld\n", YX_FOCU);
   YX_FOCU =  x_focus;
   strncpy (FOCU_NAME, "(null)", 100);
   if (YX_FOCU != YX_ROOT) {
      XFetchName(YX_DISP, YX_FOCU, &x_name);
      if (x_name == NULL) strncpy(FOCU_NAME, "(found)", 100);  /* gimp, etc.  */
      else                strncpy(FOCU_NAME, x_name,  50);     /* 99% of apps */
   }
   DEBUG_YXINIT printf("F> NAME <<%s>>\n", FOCU_NAME);
   return YX_FOCU;
}

/*> ulong                                                                              <* 
 *> xlib_focus(void)                                                                   <* 
 *> {                                                                                  <* 
 *>    Window x_focus;                                                                 <* 
 *>    x_focus = xlib_focus_find(0, 0, YX_ROOT, YX_ROOT);                                    <* 
 *>    x_focus = xlib_focus_find(0, 0, YX_ROOT, YX_ROOT);                                    <* 
 *>    XSetInputFocus(YX_DISP, x_focus, RevertToParent, CurrentTime);                     <* 
 *>    if (debug_xlib_foc) printf("   > KEY REQUEST -- focus window = %ld\n", YX_FOCU);   <* 
 *>    YX_FOCU =  x_focus;                                                                <* 
 *>    return YX_FOCU;                                                                    <* 
 *> }                                                                                  <*/


/* determine which window (under the app) is next in focus -------------------*
 *    returns the window_id number
 */
/*> ulong                                                                                                             <* 
 *> xlib_focus_find(int a_level, int a_order, Window a_top, Window a_focus)                                           <* 
 *> {                                                                                                                 <* 
 *>    /+---(local variables)---------------------------------+/                                                      <* 
 *>    ulong   x_focus = a_focus;                                                                                     <* 
 *>    int             i, j;                                                                                          <* 
 *>    uint            nchildren;                                                                                     <* 
 *>    Window         *children;                                                                                      <* 
 *>    Window          dummy;                                                                                         <* 
 *>    Atom            wm_state;                                                                                      <* 
 *>    Atom            type = None;                                                                                   <* 
 *>    ulong           nitems, after;                                                                                 <* 
 *>    uchar          *data;                                                                                          <* 
 *>    int             format;                                                                                        <* 
 *>    /+---(process the window)------------------------------+/                                                      <* 
 *>    if (debug_xlib_foc) {                                                                                          <* 
 *>       if (a_top == my.focu)      printf(" FOCUS ");                                                               <* 
 *>       else if (a_top == my.base) printf(" YX_BASE  ");                                                               <* 
 *>       else if (a_top == my.root) printf(" YX_ROOT  ");                                                               <* 
 *>       else                       printf("       ");                                                               <* 
 *>       for (j = 0; j < a_level; ++j) printf("   ");                                                                <* 
 *>    }                                                                                                              <* 
 *>    /+---(look for children)-------------------------------+/                                                      <* 
 *>    XQueryTree(YX_DISP, a_top, &dummy, &dummy, &children, &nchildren);                                                <* 
 *>    if (debug_xlib_foc) printf("%2d) %-10ld with %-2d children", a_order, a_top, nchildren);                       <* 
 *>    /+---(check the state)---------------------------------+/                                                      <* 
 *>    wm_state = XInternAtom(YX_DISP, "WM_STATE", 1);                                                                   <* 
 *>    XGetWindowProperty(YX_DISP, a_top, wm_state, 0, 0, 0, AnyPropertyType, &type, &format, &nitems, &after, &data);   <* 
 *>    /+---(find the last before me)-------------------------+/                                                      <* 
 *>    if (type == wm_state) {                                                                                        <* 
 *>       if (a_top != my.base) x_focus = a_top;                                                                      <* 
 *>       if (debug_xlib_foc) printf("   ZZZZZZZZZZZZZZZZZZZZZZZZZ\n");                                               <* 
 *>    }                                                                                                              <* 
 *>    else if (debug_xlib_foc) printf("\n");                                                                         <* 
 *>    for (i = 0; i < (int) nchildren; ++i) {                                                                        <* 
 *>       x_focus = xlib_focus_find(a_level + 1, i, children[i], x_focus);                                            <* 
 *>    }                                                                                                              <* 
 *>    if (a_level == 0 && x_focus == 0) return my.root;                                                              <* 
 *>    else                              return x_focus;                                                              <* 
 *> }                                                                                                                 <*/



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char   unit_answer [LEN_RECD];

char       /*----: set up program urgents/debugging --------------------------*/
yX11__unit_quiet   (void)
{
   yX11_reset ();
   return 0;
}

char       /*----: set up program urgents/debugging --------------------------*/
yX11__unit_loud    (void)
{
   yLOGS_begin ("yX11" , YLOG_SYS, YLOG_NOISE);
   yURG_by_name  ("kitchen"      , YURG_ON);
   yURG_by_name  ("desk"         , YURG_ON);
   yURG_by_name  ("ystr"         , YURG_ON);
   DEBUG_DESK    yLOG_info     ("yX11"    , yX11_version   ());
   yX11_reset ();
   return 0;
}

char       /*----: stop logging ----------------------------------------------*/
yX11__unit_end     (void)
{
   yLOGS_end     ();
   return 0;
}

