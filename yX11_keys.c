#include "yX11.h"
#include "yX11_priv.h"



/*====================------------------------------------====================*/
/*===----                     sending keystrokes                       ----===*/
/*====================------------------------------------====================*/
static void      o___KEYSTROKES______________o (void) {;}

Time
yx11_keys__timestamp    (void)
{
   int              tint;
   struct timeval   tv;
   gettimeofday (&tv, NULL);
   tint = (int) tv.tv_sec * 1000;
   tint = tint / 1000 * 1000;
   tint = tint + tv.tv_usec / 1000;
   return (Time) tint;
}

char             /* [------] pass a key to the appropriate window ------------*/
yx11_keys__sendone      (long a_window, char *a_mods, u_long a_keysym)
{
   /*---(design notes)-------------------*/
   /*
    *   1) shift
    *   2) mode
    *   3) control
    *   4) alt
    *   5) hyper
    *   6) super
    */
   /*---(locals)-----------+-----------+-*/
   char        tmp         [10];
   char        rc          = 0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(set basic xsendevent fields)----*/
   DEBUG_DESK   yLOG_note    ("set header fields");
   YX_SKEY.display     = YX_DISP;
   if (a_window == NULL)  a_window = YX_FOCU;
   YX_SKEY.window      = a_window;
   YX_SKEY.send_event  = 1;
   YX_SKEY.root        = YX_ROOT;
   YX_SKEY.subwindow   = None;
   YX_SKEY.time        = yx11_keys__timestamp ();
   YX_SKEY.x           = 1;
   YX_SKEY.y           = 1;
   YX_SKEY.x_root      = 1;
   YX_SKEY.y_root      = 1;
   YX_SKEY.same_screen = 1;
   YX_SKEY.state       = 0;
   /*---(window manager keys)------------*/
   DEBUG_DESK   yLOG_note    ("check for super/hyper");
   if (a_mods [5] == 'y' || a_mods [6] == 'y')  YX_SKEY.window = YX_ROOT;
   /*---(set mods and key)------------*/
   DEBUG_DESK   yLOG_note    ("do state masking");
   YX_SKEY.state       = 0;
   if (a_mods [0] == 'y')  YX_SKEY.state      |= ShiftMask;
   if (a_mods [1] == 'y')  YX_SKEY.state      |= 0x2000;
   if (a_mods [2] == 'y')  YX_SKEY.state      |= ControlMask;
   if (a_mods [3] == 'y')  YX_SKEY.state      |= Mod1Mask;;
   if (a_mods [4] == 'y')  YX_SKEY.state      |= Mod5Mask;;
   if (a_mods [5] == 'y')  YX_SKEY.state      |= Mod4Mask;;
   DEBUG_DESK   yLOG_note    ("translate keysym");
   YX_SKEY.keycode     = XKeysymToKeycode (YX_DISP, a_keysym);
   /*---(keypress side)---------------*/
   DEBUG_DESK   yLOG_note    ("send key press");
   YX_SKEY.time        = yx11_keys__timestamp ();
   YX_SKEY.type        = KeyPress;
   XSendEvent(YX_DISP, YX_SKEY.window, 0, KeyPressMask, (XEvent *) &YX_SKEY);
   /*---(keyrelease side)-------------*/
   DEBUG_DESK   yLOG_note    ("send key release");
   YX_SKEY.time        = yx11_keys__timestamp ();
   YX_SKEY.type        = KeyRelease;
   XSendEvent(YX_DISP, YX_SKEY.window, 0, KeyReleaseMask, (XEvent *) &YX_SKEY);
   /*---(sync it up)------------------*/
   DEBUG_DESK   yLOG_note    ("sync and flush");
   XSync (YX_DISP, 1);
   XFlush(YX_DISP);
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return;
}

char
yX11_keys_send          (long a_window, uchar *a_keys)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   char        i           =    0;
   char        x_mods      [LEN_TERSE] = "------";
   u_long      x_ch        =  '-';
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defense)-------------------------------*/
   rc = yx11_base_defense ();
   DEBUG_DESK   yLOG_value   ("defense"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (a_keys == NULL) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_len = strlen (a_keys);
   DEBUG_DESK   yLOG_value   ("x_len"     , x_len);
   for (i = 0; i < x_len; ++i) {
      DEBUG_DESK   yLOG_complex ("key"       , "%2d %3d %c %s", i, a_keys [i], a_keys [i], x_mods);
      switch (a_keys [i]) {
      case (uchar) 'Ð' :  x_mods [0] = 'y';  continue;  break; /* shift       */
      case (uchar) 'Ñ' :  x_mods [1] = 'y';  continue;  break; /* mode        */
      case (uchar) 'Ô' :  x_mods [2] = 'y';  continue;  break; /* control     */
      case (uchar) 'Ó' :  x_mods [3] = 'y';  continue;  break; /* alt         */
      case (uchar) 'Ö' :  x_mods [4] = 'y';  continue;  break; /* hyper       */
      case (uchar) 'Õ' :  x_mods [5] = 'y';  continue;  break; /* super       */
      case (uchar) '·' :  x_ch    = ' ';                break;
      case (uchar) '\n':  x_ch    = 0xff0d;             break;
      case (uchar) '¦' :  x_ch    = 0xff0d;             break;
      case (uchar) '\e':  x_ch    = 0xff1b;             break;
      case (uchar) '¥' :  x_ch    = 0xff1b;             break;
      case (uchar) '\t':  x_ch    = 0xff09;             break;
      case (uchar) '­' :  x_ch    = 0xff09;             break;
      default          :  if (a_keys [i] <  32)   continue; 
                             if (a_keys [i] > 127)   continue; 
                          x_ch    = a_keys [i];
                          break;
      }
      DEBUG_DESK   yLOG_complex ("sending"   , "%2d %3d %s", i, x_ch, x_mods);
      yx11_keys__sendone (a_window, x_mods, x_ch);
      strlcpy (x_mods, "------", LEN_TERSE);
   }
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}



