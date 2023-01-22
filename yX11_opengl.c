#include "yX11.h"
#include "yX11_priv.h"



/* establish the glx connection ----------------------------------------------*
 *    0 = success
 *   -1 = glxchoosevisual failed
 */
char
yX11_opengl__connect    (void)
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
yX11_opengl__create      (void)
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
yX11_opengl__destroy    (void)
{
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   glXDestroyContext(YX_DISP, GCON);
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

