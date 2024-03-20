#include "yX11.h"
#include "yX11_priv.h"



#define  S_SHFT         0
#define  S_MODE         1
#define  S_CTRL         2
#define  S_ALTT         3
#define  S_HYPE         4
#define  S_SUPR         5



char g_letters[40] = {
   't' , 'm' , '[' , 'c' , '\"',   /* N    */
   'n' , 'f' , '_' , 'r' , 'x' ,   /* NE   */
   '>' , 'u' , '.' , 'p' , ',' ,   /* E    */
   'i' , 'l' , 'q' , 'd' , 'b' ,   /* SE   */
   'e' , 'h' , '\'', 's' , ']' ,   /* S    */
   'o' , 'g' , 'z' , 'w' , ')' ,   /* SW   */
   '<' , 'j' , ';' , 'k' , ':' ,   /* W    */
   'a' , 'y' , '(' , 'v' , '-' ,   /* NW   */
};


#define   MAX_KEYS    100
struct cKEYS  {
   char        norm;
   char        shft;
   char        mode;
   long        keysym;
} s_keys [MAX_KEYS] = {
   /*---(upper left)---------------------*/
   { NULL, NULL, NULL, NULL               },
   { 'q' , 'Q' , '|' , XK_q               },
   { 'w' , 'W' , '{' , XK_w               },
   { 'f' , 'F' , '}' , XK_f               },
   { 'p' , 'P' , '[' , XK_p               },
   { 'g' , 'G' , ']' , XK_g               },
   { NULL, NULL, NULL, NULL               },
   /*---(upper right)--------------------*/
   { NULL, NULL, NULL, NULL               },
   { '#' , '#' , '&' , XK_numbersign      },
   { 'l' , 'L' , '_' , XK_l               },
   { 'u' , 'U' , '<' , XK_u               },
   { 'j' , 'J' , '>' , XK_j               },
   { 'y' , 'Y' , '$' , XK_y               },
   { NULL, NULL, NULL, NULL               },
   /*---(home left)----------------------*/
   { '\'', '`' , '%' , XK_apostrophe      },
   { 'a' , 'A' , ';' , XK_a               },
   { 'r' , 'R' , '/' , XK_r               },
   { 's' , 'S' , '-' , XK_s               },
   { 't' , 'T' , '0' , XK_t               },
   { 'd' , 'D' , ':' , XK_d               },
   { NULL, NULL, NULL, NULL               },
   /*---(home right)---------------------*/
   { NULL, NULL, NULL, NULL               },
   { 'h' , 'H' , '\\', XK_h               },
   { 'n' , 'N' , '1' , XK_n               },
   { 'e' , 'E' , '(' , XK_e               },
   { 'i' , 'I' , ')' , XK_i               },
   { 'o' , 'O' , '=' , XK_o               },
   { '"' , '~' , '^' , XK_quotedbl        },
   /*---(lower left)---------------------*/
   { NULL, NULL, '@' , XK_at              },
   { 'z' , 'Z' , '8' , XK_z               },
   { 'x' , 'X' , '6' , XK_x               },
   { 'c' , 'C' , '4' , XK_c               },
   { 'v' , 'V' , '2' , XK_v               },
   { 'b' , 'B' , '+' , XK_b               },
   { NULL, NULL, NULL, NULL               },
   /*---(lower right)--------------------*/
   { NULL, NULL, NULL, NULL               },
   { 'k' , 'K' , '*' , XK_k               },
   { 'm' , 'M' , '3' , XK_m               },
   { ',' , '?' , '5' , XK_comma           },
   { '.' , '!' , '7' , XK_period          },
   { NULL, NULL, '9' , XK_9               },
   { NULL, NULL, NULL, NULL               },
   /*---(thumb left)---------------------*/
   { '¦' , '¦' , '¦' , XK_Return          },
   { NULL, NULL, NULL, XK_Shift_L         },
   { '¥' , '¥' , '¥' , XK_Escape          },
   /*---(thumb right)--------------------*/
   { NULL, NULL, NULL, XK_BackSpace       },
   { ' ' , NULL, NULL, XK_space           },
   { NULL, NULL, NULL, XK_Control_L       },
   /*---(sub-thumb)----------------------*/
   { NULL, NULL, NULL, XK_Alt_L           },
   { NULL, NULL, NULL, XK_Mode_switch     },
   { NULL, NULL, NULL, XK_Tab             },
   /*---(done)---------------------------*/
   { -1  , -1  , -1  , -1                 },
};
static uchar  s_mods      [LEN_TERSE] = "······";
static char   s_orig      =   0;
static char   s_char      =   0;
static long   s_long      =   0;
static char   s_slash     =   0;
static char   s_protect   =   0;


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
   if (a_mods [S_HYPE] == 'y' || a_mods [S_SUPR] == 'y')  YX_SKEY.window = YX_ROOT;
   /*---(set mods and key)------------*/
   DEBUG_DESK   yLOG_note    ("do state masking");
   YX_SKEY.state       = 0;
   if (a_mods [S_SHFT] == 'y')  YX_SKEY.state      |= ShiftMask;
   if (a_mods [S_MODE] == 'y')  YX_SKEY.state      |= 0x2000;
   if (a_mods [S_CTRL] == 'y')  YX_SKEY.state      |= ControlMask;
   if (a_mods [S_ALTT] == 'y')  YX_SKEY.state      |= Mod1Mask;;
   if (a_mods [S_HYPE] == 'y')  YX_SKEY.state      |= Mod5Mask;;
   if (a_mods [S_SUPR] == 'y')  YX_SKEY.state      |= Mod4Mask;;
   DEBUG_DESK   yLOG_value   ("state"     , YX_SKEY.state);
   DEBUG_DESK   yLOG_note    ("translate keysym");
   YX_SKEY.keycode     = XKeysymToKeycode (YX_DISP, a_keysym);
   DEBUG_DESK   yLOG_value   ("keycode"   , YX_SKEY.keycode);
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
yx11_keys__reset        (void)
{
   s_orig  = 0;
   s_char  = 0;
   s_long  = 0;
   s_mods [S_SHFT] = '·';
   s_mods [S_MODE] = '·';
   s_slash = 0;
   return 0;
}

char
yx11_keys__control      (uchar a_ch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   long        c           =    0;
   /*---(reset)--------------------------*/
   yx11_keys__reset ();
   /*---(quick-out)----------------------*/
   if (a_ch >= 32 && a_ch != G_CHAR_RETURN && a_ch != G_CHAR_ESCAPE)   return 0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(handle controls)----------------*/
   switch (a_ch) {
   case G_CHAR_RETURN :
      DEBUG_DESK   yLOG_note    ("char return (¦)");
      c  = 0xff0d;
      break;
   case (uchar) '\n' :
      DEBUG_DESK   yLOG_note    ("true return");
      c  = 0xff0d;
      break;
   case G_CHAR_ESCAPE :
      DEBUG_DESK   yLOG_note    ("char escape (¥)");
      c  = 0xff1b;
      break;
   case (uchar) '\e' :
      DEBUG_DESK   yLOG_note    ("true escape");
      c  = 0xff1b;
      break;
   case (uchar) '\b' :
      DEBUG_DESK   yLOG_note    ("true backspace");
      c  = 0xff08;
      break;
   case (uchar) '' :
      DEBUG_DESK   yLOG_note    ("ascii field");
      c  = 0xff08;
      break;
   default :
      DEBUG_DESK   yLOG_note    ("no match found");
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(saveback)-----------------------*/
   DEBUG_DESK   yLOG_value   ("c"         , c);
   s_orig = s_char = a_ch;
   s_long = c;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
yx11_keys__normal         (uchar a_ch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   long        c           =    0;
   /*---(reset)--------------------------*/
   yx11_keys__reset ();
   /*---(quick-out)----------------------*/
   if (a_ch < 32 || a_ch >= 127)   return 0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(find)---------------------------*/
   for (i = 0; i < MAX_KEYS; ++i) {
      if (s_keys [i].norm == -1)  break;
      if (s_keys [i].norm == a_ch) {
         DEBUG_DESK   yLOG_note    ("normal key");
         c = s_keys [i].keysym;
      }
      if (s_keys [i].shft == a_ch) {
         DEBUG_DESK   yLOG_note    ("shifted key");
         s_mods [S_SHFT] = 'y';
         c = s_keys [i].keysym;
      }
      if (s_keys [i].mode == a_ch) {
         DEBUG_DESK   yLOG_note    ("moded key");
         s_mods [S_MODE] = 'y';
         c = s_keys [i].keysym;
      }
   }
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_value   ("c"         , c);
   if (c == 0) {
      DEBUG_DESK   yLOG_note    ("match not found");
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   /*---(saveback)-----------------------*/
   s_orig = s_char = a_ch;
   s_long = c;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 1;
}

/*> ,x··23|··ve··d··9|··P··a              : x_ch = ¥··25|··dw··f:··r;··l··v··f;··d··n··,y   <*/

char
yx11_keys__slashed      (uchar a_ch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        c           =  '·';
   char        x_slash     =    0;
   char        x_base      =    0;
   /*---(reset)--------------------------*/
   yx11_keys__reset ();
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(find)---------------------------*/
   rc = ychrbackward (a_ch, &x_slash, &x_base);
   if (rc == 0) {
      DEBUG_DESK   yLOG_note    ("match not found");
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return 0;
   }
   DEBUG_DESK   yLOG_complex ("trans"     , "%d  %c", x_slash, x_base);
   yx11_keys__normal (x_base);
   /*> s_orig = a_ch;                                                                 <* 
    *> s_char = x_ch;                                                                 <* 
    *> s_slash = 1;                                                                   <*/
   /*---(saveback)-----------------------*/
   s_orig  = s_char = a_ch;
   s_slash = x_slash;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 1;
}

char
yx11_keys__more         (uchar a_ch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        x_ch        =    0;
   /*---(defense)------------------------*/
   yx11_keys__reset ();
   yx11_keys__normal (x_ch);
   s_orig = a_ch;
   s_char = x_ch;
   s_slash = 2;
   return x_ch;
}

char
yX11_keys_send          (long a_window, uchar *a_keys)
{
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_complex ("a_args"    , "%x, %p", a_window, a_keys);
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
   DEBUG_DESK   yLOG_info    ("keys"      , a_keys);
   /*
    * verify window
    *
    */
   x_len = strlen (a_keys);
   DEBUG_DESK   yLOG_value   ("x_len"     , x_len);
   yx11_keys__reset ();
   for (i = 0; i < x_len; ++i) {
      DEBUG_DESK   yLOG_complex ("key"       , "%2d %3d %c %s", i, a_keys [i], a_keys [i], s_mods);
      switch (a_keys [i]) {
         /*> case (uchar) 'Õ' :                                                          <* 
          *>    s_long  = 0xff09;                                                        <* 
          *>    DEBUG_DESK   yLOG_value   ("blitz"    , s_long);                         <* 
          *>    break;                                                                   <*/
         /*> case (uchar) 'Ö' :                                                          <* 
          *>    s_long  = 0xff08;                                                        <* 
          *>    DEBUG_DESK   yLOG_value   ("unblitz"  , s_long);                         <* 
          *>    break;                                                                   <*/
         /*> case (uchar) 'Ó' :                                                          <* 
          *>    DEBUG_DESK   yLOG_note    ("control Ó or hyper ÓÓ");                     <* 
          *>    if (s_mods [S_CTRL] == 'y') {                                            <* 
          *>       s_mods [S_HYPE] = 'y';                                                <* 
          *>       s_mods [S_CTRL] = '-';                                                <* 
          *>    } else s_mods [S_CTRL] = 'y';                                            <* 
          *>    continue;                                                                <* 
          *>    break;                                                                   <*/
         /*> case (uchar) 'Ô' :                                                          <* 
          *>    DEBUG_DESK   yLOG_note    ("alt Ô or super ÔÔ");                         <* 
          *>    if (s_mods [S_ALTT] == 'y') {                                            <* 
          *>       s_mods [S_SUPR] = 'y';                                                <* 
          *>       s_mods [S_ALTT] = '-';                                                <* 
          *>    } else s_mods [S_ALTT] = 'y';                                            <* 
          *>    continue;                                                                <* 
          *>    break;                                                                   <*/
      case (uchar) '·' :  /* IGORE, normal spacer */
         DEBUG_DESK   yLOG_note    ("spacer");
         continue;
         break;
      case (uchar) '²' :  /* SPACE */
         s_long  = XK_space;
         DEBUG_DESK   yLOG_value   ("space"    , s_long);
         break;
      default          : 
         if        (a_keys [i] <  32) {
            rc = yx11_keys__control  (a_keys [i]);
            if (rc == 0)  continue; 
         } else if (a_keys [i] < 127) {
            rc = yx11_keys__normal   (a_keys [i]);
            if (rc == 0)  continue; 
         } else  {
            switch (a_keys [i]) {
            case G_CHAR_RETURN : case G_CHAR_ESCAPE :
               rc = yx11_keys__control  (a_keys [i]);
               break;
            default :
               rc = yx11_keys__slashed (a_keys [i]);
               break;
            }
            if (rc == 0)  continue; 
         }
         break;
      }
      DEBUG_DESK   yLOG_complex ("sending"   , "%2d %3d %d %s", i, s_char, s_long, s_mods);
      if (s_slash >= 1)  yx11_keys__sendone (a_window, "·y····", XK_h);
      if (s_slash == 2)  yx11_keys__sendone (a_window, "·y····", XK_l);
      yx11_keys__sendone (a_window, s_mods, s_long);
      yx11_keys__reset ();
      ystrlcpy (s_mods, "······", LEN_TERSE);
      s_protect = 0;
   }
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_keys_send_current  (uchar *a_keys)
{
   yX11_focus ();
   yX11_keys_send (YX_FOCU, a_keys);
   return 0;
}

char
yX11_keys_blitz         (long a_window, uchar *a_keys)
{
   uchar       t           [LEN_RECD ] = "";
   snprintf (t, LEN_RECD, "Õ%sÖ", a_keys);
   t [LEN_RECD - 2] = 'Ö';
   t [LEN_RECD - 1] = '\0';
   yX11_keys_send (a_window, t);
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
yX11__unit_keys         (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL];
   char        t           [LEN_RECD ];
   int         i           =    0;
   /*---(initialize)---------------------*/
   ystrlcpy (unit_answer, "KEYS unit        : unknown request", LEN_RECD);
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "symkey"  , 20)  == 0) {
      if (s_long == 0)  snprintf (unit_answer, LEN_RECD, "KEYS symkey      : orig ·/···, char ·/···, sym õ····, slash ·, mods ······");
      else              snprintf (unit_answer, LEN_RECD, "KEYS symkey      : orig %c/%-3d, char %c/%-3d, sym õ%04x, slash %d, mods %-6.6s", ychrvisible (s_orig), (uchar) s_orig, ychrvisible (s_char), (uchar) s_char, s_long, s_slash, s_mods);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}

char
yx11__unit_keys_read    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(open)---------------------------*/
   f = fopen ("/tmp/yX11_keys.txt", "rt");
   DEBUG_DESK   yLOG_point   ("fopen"     , f);
   --rce;  if (f == NULL) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read and inventory)-------------*/
   while (1) {
      fgets (x_recd, LEN_RECD, f);
      if (feof (f))     break;
      switch (c) {
      case 0  : rc = strcmp ("abcdefghijklmnopqrstuvwxyz\n", x_recd);  break;
      case 1  : rc = strcmp ("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", x_recd);  break;
      case 2  : rc = strcmp ("0123456789\n"                , x_recd);  break;
      case 3  : rc = strcmp (",.:;?!'\" \n"                , x_recd);  break;
      case 4  : rc = strcmp ("()[]<>{}\n"                  , x_recd);  break;
      case 5  : rc = strcmp ("-+*/=%$\n"                   , x_recd);  break;
      case 6  : rc = strcmp ("_#&|^@~'\n"                  , x_recd);  break;
      case 7  : rc = strcmp ("¡¢\n"                        , x_recd);  break;
      case 8  : rc = strcmp ("£¤¾¿\n"                      , x_recd);  break;
      case 9  : rc = strcmp ("©ª«³´Ï\n"                    , x_recd);  break;
      case 10 : rc = strcmp ("²¬­®¯\n"                     , x_recd);  break;
      case 11 : rc = strcmp ("·\n"                         , x_recd);  break;
      case 12 : rc = strcmp ("¸¹º»¼½\n"                    , x_recd);  break;
      case 13 : rc = strcmp ("ÀÁÂÃÄÅ\n"                    , x_recd);  break;
      case 14 : rc = strcmp ("ÆÇÈÉÊËÌÍÎ\n"                 , x_recd);  break;
      case 15 : rc = strcmp ("ÐÑÒÓÔÕÖÞÝßåæ±\n"             , x_recd);  break;
      case 16 : rc = strcmp ("èéêëìíîïðñòó\n"              , x_recd);  break;
      case 17 : rc = strcmp ("ôõö÷øùúûüýþÿ\n"              , x_recd);  break;
      case 18 : rc = strcmp ("°×ØÙÚÛÜàáâãäç\n"             , x_recd);  break;
      case 19 : rc = 99;  break;
      default : rc = -1;
      }
      DEBUG_DESK   yLOG_complex ("x_recd"    , "%d) %d %s", c, rc, x_recd);
      if (rc == 99) {
         rc = 0;
         break;
      }
      if (rc < 0)  break;
      ++c;
   }
   /*---(close)--------------------------*/
   fclose (f);
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return rc;
}





