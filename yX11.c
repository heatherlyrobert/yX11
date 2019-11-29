#include "yX11.h"
#include "yX11_priv.h"

char       yXINIT__xconnect        (void);
char       yXINIT__xsetup          (void);
char       yXINIT__xdestroy        (void);
char       yXINIT__gconnect        (void);
char       yXINIT__gsetup          (void);
char       yXINIT__gdestroy        (void);
int        yX11__xerror            (Display *a_disp, XErrorEvent *a_event);

#define   DEBUG      if (gXINIT.verbose) 

tXINIT  gXINIT;

static char      yX11_ver [200] = "";

char*      /* ---- : return library versioning information -------------------*/
yX11_version       (void)
{
   char    t [20] = "";
#if    __TINYC__ > 0
   strncpy (t, "[tcc built]", 15);
#elif  __GNUC__  > 0
   strncpy (t, "[gnu gcc  ]", 15);
#else
   strncpy (t, "[unknown  ]", 15);
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
yX11_start (
      char     *a_name,                /* main window name                    */
      int       a_width,               /* window width                        */
      int       a_height,              /* window height                       */
      char      a_focusable,           /* will/should it accept focus         */
      char      a_resizeable,          /* will/should it resize               */
      char      a_verbose)             /* debugging noise desired             */
{  /*---(start)----------------------------*/
   VERBOSE = 'n';
   if (a_verbose == 'y') VERBOSE = 'y';
   DEBUG_YXINIT printf("yXINIT -- heatherly xlib/glx setup ------------------------------------------ (start)\n\n");
   /*---(establish verbosity)--------------*/
   DEBUG_YXINIT printf("starting up...\n");
   DEBUG_YXINIT printf("   - setting verbosity . . . . . . . . . . %d\n", VERBOSE);
   DEBUG_YXINIT printf("   - record window name  . . . . . . . . . ");
   strncpy(TITLE, a_name, 100);
   DEBUG_YXINIT printf("%-30.30s\n", TITLE);
   DEBUG_YXINIT printf("   - record suggested size . . . . . . . . ");
   WIDTH  = a_width;
   HEIGHT = a_height;
   DEBUG_YXINIT printf("%dw, %dh\n", WIDTH, HEIGHT);
   DEBUG_YXINIT printf("   - focusable attribute . . . . . . . . . ");
   CAN_FOCUS = a_focusable;
   CAN_SIZE  = a_resizeable;
   DEBUG_YXINIT printf("%c\n", CAN_FOCUS);
   /*---(reset summary counters)-------*/
   DEBUG_YXINIT printf("   - start executing setup functions\n\n");
   yXINIT__xconnect();
   yXINIT__gconnect();
   yXINIT__xsetup();
   yXINIT__gsetup();
   DEBUG_YXINIT printf("yXINIT -- heatherly xlib/glx setup -------------------------------------------- (end)\n\n");
   /*---(complete)-------------------------*/
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
   /*---(shut the old down)----------------*/
   XResizeWindow (YX_DISP, YX_BASE, a_wide, a_tall);
   /*---(complete)-------------------------*/
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
   DEBUG_YXINIT printf("\nyXINIT -- heatherly xlib/glx destroy ---------------------------------------- (start)\n\n");
   yXINIT__gdestroy();
   yXINIT__xdestroy();
   DEBUG_YXINIT printf("yXINIT -- heatherly xlib/glx setup -------------------------------------------- (end)\n\n");
   return 0;
}

int
yX11__xerror       (Display *a_disp, XErrorEvent *a_event)
{
   return 0;
}

char
yXINIT__xconnect()
{
   DEBUG_YXINIT  printf("x11/xlib connection (YX_DISP, YX_SCRN, ROOT, FOCU)...¦");
   /*---(connect)-------------------------------*/
   DEBUG_YXINIT printf("   - xserver connection  . . . . .  (YX_DISP) ");
   YX_DISP = XOpenDisplay(NULL);      /* pull the default                 */
   if (YX_DISP == NULL) {
      DEBUG_YXINIT printf("NULL, EXITING\naborted\n");
      return -1;
   }
   DEBUG_YXINIT printf("%p¦", (void *) YX_DISP);
   /*---(get the connection number)-------------*/
   DEBUG_YXINIT printf("   - connection number . . . . . . . . . . ");
   DEBUG_YXINIT printf("%d¦", ConnectionNumber(YX_DISP));
   /*---(get the vendor)------------------------*/
   DEBUG_YXINIT printf("   - server vendor . . . . . . . . . . . . ");
   DEBUG_YXINIT printf("%s¦", ServerVendor(YX_DISP));
   /*---(get the vendor release)----------------*/
   DEBUG_YXINIT printf("   - server vendor release . . . . . . . . ");
   DEBUG_YXINIT printf("%d¦", VendorRelease(YX_DISP));
   /*---(get the protocol version)--------------*/
   DEBUG_YXINIT printf("   - protocol version  . . . . . . . . . . ");
   DEBUG_YXINIT printf("v%dr%d¦", ProtocolVersion(YX_DISP), ProtocolRevision(YX_DISP));
   /*---(show the display name)-----------------*/
   DEBUG_YXINIT printf("   - display name  . . . . . . . . . . . . ");
   DEBUG_YXINIT printf("%s¦", DisplayString(YX_DISP));
   /*---(show the screen count)-----------------*/
   DEBUG_YXINIT printf("   - number of screens . . . . . . . . . . ");
   DEBUG_YXINIT printf("%d¦", ScreenCount(YX_DISP));
   /*---(show the current screen)---------------*/
   DEBUG_YXINIT printf("   - CURRENT SCREEN  . . . . . . .  (YX_SCRN) ");
   YX_SCRN = DefaultScreen(YX_DISP);
   DEBUG_YXINIT printf("%d¦", YX_SCRN);
   /*---(get the screen pointer)----------------*/
   DEBUG_YXINIT printf("   - screen pointer  . . . . . . . . . . . ");
   SCRN_PTR    = DefaultScreenOfDisplay(YX_DISP);
   DEBUG_YXINIT printf("%p\n", (void *) SCRN_PTR);
   /*---(show the size)-------------------------*/
   DEBUG_YXINIT printf("   - screen size . . . . . . . . . . . . . ");
   DEBUG_YXINIT printf("%dw, %dh¦", DisplayWidth(YX_DISP, YX_SCRN), DisplayHeight(YX_DISP, YX_SCRN));
   /*---(show the depth)------------------------*/
   DEBUG_YXINIT printf("   - screen depth  . . . . . . . . . . . . ");
   DEBUG_YXINIT printf("%d¦", DefaultDepth(YX_DISP, YX_SCRN));
   /*---(show the root window)------------------*/
   DEBUG_YXINIT printf("   - root window . . . . . . . . .  (ROOT) ");
   ROOT      = RootWindow(YX_DISP, YX_SCRN);
   if (ROOT == 0) {
      DEBUG_YXINIT printf("NULL, EXITING\naborted\n");
      return -3;
   }
   DEBUG_YXINIT printf("%ld\n", ROOT);
   /*---(show the colormap entries)-------------*/
   DEBUG_YXINIT printf("   - color map entries . . . . . . . . . . ");
   DEBUG_YXINIT printf("%d¦", DisplayCells(YX_DISP, YX_SCRN));
   /*---(show the backing store)----------------*/
   DEBUG_YXINIT printf("   - backing store . . . . . . . . . . . . ");
   DEBUG_YXINIT printf("%d\n", DoesBackingStore(SCRN_PTR));
   /*---(current focus window)------------------*/
   DEBUG_YXINIT printf("   - focus window  . . . . . . . .  (FOCU) ");
   int trash;
   XGetInputFocus(YX_DISP, &FOCU, &trash);
   if (FOCU == 0) {
      DEBUG_YXINIT printf("NULL, EXITING\naborted\n");
      return -4;
   }
   DEBUG_YXINIT printf("%ld\n", FOCU);
   /*-------------------------------------------*/
   DEBUG_YXINIT printf("   - setup error handler . . . . . . . . . ");
   XSetErrorHandler(yX11__xerror);
   DEBUG_YXINIT printf("success\n");
   /*---(complete)------------------------------*/
   DEBUG_YXINIT printf("   - done\n\n");
   return 0;
}

char
yXINIT__xsetup()
{
   /*---(assumptions)---------------------------*/
   /* only options are to set the width, height, title, and focusable
    * - top level windows should not fight WM about size or location (hint only)
    * - set standard window properties before mapping
    * these drops of wisdom come from Tronche
    */
   /*---(start message)-------------------------*/
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
   YX_BASE = XCreateWindow(YX_DISP, ROOT,
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
   return 0;
}

char
yXINIT__xdestroy()
{
   DEBUG_YXINIT  printf("wrapping up of xwindows connection...\n");
   /*---(wipe window)---------------------------*/
   XUnmapWindow   (YX_DISP, YX_BASE);
   XDestroyWindow (YX_DISP, YX_BASE);
   /*---(disconnect)----------------------------*/
   DEBUG_YXINIT  printf("   - closing connection  . . . . . . . . . ");
   XCloseDisplay(YX_DISP);
   DEBUG_YXINIT  printf("success\n");
   /*---(complete)------------------------------*/
   DEBUG_YXINIT  printf("   - done\n\n");
   return 0;
}



/* establish the glx connection ----------------------------------------------*
 *    0 = success
 *   -1 = glxchoosevisual failed
 */
char
yXINIT__gconnect()
{
   /*> GLint       x_att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};   <*/
   GLint       x_att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
   int         major, minor;
   DEBUG_YXINIT  printf("opengl/glx connection (VISU)...\n");
   /*---(get glx version)-----------------------*/
   DEBUG_YXINIT  printf("   - glx version . . . . . . . . . . . . . ");
   glXQueryVersion(YX_DISP, &major, &minor);
   DEBUG_YXINIT  printf("v%dr%d\n", major, minor);
   /*---(setup visual)--------------------------*/
   DEBUG_YXINIT  printf("   - CREATE VISUALIZAION . . . . . .(VISU) ");
   VISU = glXChooseVisual(YX_DISP, 0, x_att);
   if (VISU == NULL) {
      DEBUG_YXINIT  printf("NULL, EXITING\naborted\n");
      return -1;
   }
   DEBUG_YXINIT  printf("%p\n", (void *) VISU);
   /*---(complete)------------------------------*/
   DEBUG_YXINIT  printf("   - done\n\n");
   return 0;
}


/* update the base window with opengl settings -------------------------------*
 *    0 = success
 */
char
yXINIT__gsetup()
{
   DEBUG_YXINIT  printf("opengl/glx main window (GCON)...\n");
   /*---(context)-------------------------------*/
   DEBUG_YXINIT  printf("   - CREATE CONTEXT  . . . . . . . .(GCON) ");
   GCON = glXCreateContext(YX_DISP, VISU, NULL, GL_TRUE);
   DEBUG_YXINIT  printf("%p\n", (void *) GCON);
   /*---(make current)--------------------------*/
   DEBUG_YXINIT  printf("   - make current  . . . . . . . . . . . . ");
   glXMakeCurrent(YX_DISP, YX_BASE, GCON);
   DEBUG_YXINIT  printf("success\n");
   /*---(make current)--------------------------*/
   DEBUG_YXINIT  printf("   - enable depth calcs  . . . . . . . . . ");
   glEnable(GL_DEPTH_TEST);
   DEBUG_YXINIT  printf("success\n");
   /*---(make current)--------------------------*/
   DEBUG_YXINIT  printf("   - enable LE depth calc  . . . . . . . . ");
   glDepthFunc(GL_LEQUAL);
   DEBUG_YXINIT  printf("success\n");
   /*---(make current)--------------------------*/
   DEBUG_YXINIT  printf("   - enable flat shading . . . . . . . . . ");
   glEnable(GL_FLAT);
   DEBUG_YXINIT  printf("success\n");
   /*-------------------------------------------*/
   DEBUG_YXINIT printf("   - smoothing hints . . . . . . . . . . . ");
   glEnable(GL_POINT_SMOOTH);
   glEnable(GL_LINE_SMOOTH);
   /*> glEnable(GL_POLYGON_SMOOTH);                                                   <*/
   DEBUG_YXINIT printf("success\n");
   /*-------------------------------------------*/
   DEBUG_YXINIT printf("   - blending hints  . . . . . . . . . . . ");
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,  GL_ONE_MINUS_SRC_ALPHA);
   DEBUG_YXINIT printf("success\n");
   /*-------------------------------------------*/
   DEBUG_YXINIT printf("   - antialiasing hints  . . . . . . . . . ");
   glHint(GL_POINT_SMOOTH_HINT,   GL_NICEST);
   glHint(GL_LINE_SMOOTH_HINT,    GL_NICEST);
   glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
   DEBUG_YXINIT printf("success\n");
   /*---(make current)--------------------------*/
   DEBUG_YXINIT  printf("   - perspective hints . . . . . . . . . . ");
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
   DEBUG_YXINIT  printf("success\n");
   /*---(check double buffer)-------------------*/
   DEBUG_YXINIT  printf("   - check double buffer . . . . . . . . . ");
   if (glXIsDirect(YX_DISP, GCON)) {
      DEBUG_YXINIT  printf("double\n");
   } else {
      DEBUG_YXINIT  printf("SINGLE\n");
   }
   /*---(complete)------------------------------*/
   DEBUG_YXINIT  printf("   - done\n\n");
   return 0;
}

char
yXINIT__gdestroy()
{
   glXDestroyContext(YX_DISP, GCON);
   return 0;
}


ulong            /* [------] find the currrently focused window -----------------------------*/
yXINIT_focus       (void)
{
   Window x_focus;
   int    x_waste;
   char  *x_name;
   static    int x_runs  = 0;
   ++x_runs;
   DEBUG_YXINIT printf("F> START xlib_focus() : root=%ld, base=%ld, run=%d¦", ROOT, YX_BASE, x_runs);
   XGetInputFocus (YX_DISP, &x_focus, &x_waste);
   DEBUG_YXINIT printf("F> FIND  xlib_focus() : focus window = %ld\n", FOCU);
   FOCU =  x_focus;
   strncpy (FOCU_NAME, "(null)", 100);
   if (FOCU != ROOT) {
      XFetchName(YX_DISP, FOCU, &x_name);
      if (x_name == NULL) strncpy(FOCU_NAME, "(found)", 100);  /* gimp, etc.  */
      else                strncpy(FOCU_NAME, x_name,  50);     /* 99% of apps */
   }
   DEBUG_YXINIT printf("F> NAME <<%s>>\n", FOCU_NAME);
   return FOCU;
}

/*> ulong                                                                              <* 
 *> xlib_focus(void)                                                                   <* 
 *> {                                                                                  <* 
 *>    Window x_focus;                                                                 <* 
 *>    x_focus = xlib_focus_find(0, 0, ROOT, ROOT);                                    <* 
 *>    x_focus = xlib_focus_find(0, 0, ROOT, ROOT);                                    <* 
 *>    XSetInputFocus(YX_DISP, x_focus, RevertToParent, CurrentTime);                     <* 
 *>    if (debug_xlib_foc) printf("   > KEY REQUEST -- focus window = %ld\n", FOCU);   <* 
 *>    FOCU =  x_focus;                                                                <* 
 *>    return FOCU;                                                                    <* 
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
 *>       else if (a_top == my.root) printf(" ROOT  ");                                                               <* 
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

