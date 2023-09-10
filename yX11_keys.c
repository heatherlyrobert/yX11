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
yx11_keys__normal         (uchar a_ch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   /*---(defense)------------------------*/
   yx11_keys__reset ();
   s_orig = s_char = a_ch;
   --rce;  if (a_ch < 32 || a_ch >= 127)   return rce;
   /*---(find)---------------------------*/
   for (i = 0; i < MAX_KEYS; ++i) {
      if (s_keys [i].norm == -1)  break;
      if (s_keys [i].norm == a_ch) {
         return s_long = s_keys [i].keysym;
      }
      if (s_keys [i].shft == a_ch) {
         s_mods [S_SHFT] = 'y';
         return s_long = s_keys [i].keysym;
      }
      if (s_keys [i].mode == a_ch) {
         s_mods [S_MODE] = 'y';
         return s_long = s_keys [i].keysym;
      }
   }
   return s_long = 0;
}

/*> ,x··23|··ve··d··9|··P··a              : x_ch = ¥··25|··dw··f:··r;··l··v··f;··d··n··,y   <*/

char
yx11_keys__extended     (uchar a_ch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        x_ch        =    0;
   /*---(defense)------------------------*/
   yx11_keys__reset ();
   switch (a_ch) {
   case G_CHAR_REXCLAM  : x_ch = '!'  ;  break;    /*    33 ! = 161 ¡   */
   case G_CHAR_RQUEST   : x_ch = '?'  ;  break;    /*    63 ? = 162 ¢   */
                          /*---(hidden)--------------------*/
   case G_CHAR_NULL     : x_ch = '0'  ;  break;    /*    48 0 = 163 £   */
   case G_CHAR_PLACE    : x_ch = 'p'  ;  break;    /*   112 p = 164 ¤   */
   case G_CHAR_ESCAPE   : x_ch = 'e'  ;  break;    /*   101 e = 165 ¥   */
   case G_CHAR_RETURN   : x_ch = 'n'  ;  break;    /*   110 n = 166 ¦   */
   case G_CHAR_FIELD    : x_ch = 'f'  ;  break;    /*   102 f = 167 §   */
   case G_CHAR_GROUP    : x_ch = 'g'  ;  break;    /*   103 g = 168 ¨   */
                          /*---(macro)---------------------*/
   case G_CHAR_DISPLAY  : x_ch = '+'  ;  break;    /*    43 + = 169 ©   */
   case G_CHAR_BREAK    : x_ch = '.'  ;  break;    /*    46 . = 170 ª   */
   case G_CHAR_WAIT     : x_ch = ','  ;  break;    /*    44 , = 171 «   */
   case G_CHAR_HALT     : x_ch = 'q'  ;  break;    /*   113 q = 179 ³   */
   case G_CHAR_BIGDOT   : x_ch = '\'' ;  break;    /*    39 ' = 207 ´   */
   case G_CHAR_HUGEDOT  : x_ch = '@'  ;  break;    /*    64 @ = 180 Ï   */
                          /*---(special)-------------------*/
   case G_CHAR_STORAGE  : x_ch = ' '  ;  break;    /*    32   = 178 ²   */
   case G_CHAR_MASK     : x_ch = '*'  ;  break;    /*    42 * = 172 ¬   */
   /*> case G_CHAR_TAB      : x_ch = 't'  ;  break;    /+   116 t = 173 ­   +/        <*/
   case G_CHAR_SUMMARY  : x_ch = ':'  ;  break;    /*    58 : = 174 ®   */
   case G_CHAR_SYSTEM   : x_ch = '%'  ;  break;    /*    37 % = 175 ¯   */
                          /*---(delayed)-------------------*/
   case G_CHAR_DBSLASH  : x_ch = '\\' ;  break;    /*    92 \ = 181 µ   */
   case G_CHAR_DDQUOTE  : x_ch = '"'  ;  break;    /*    34 " = 182 ¶   */
   case G_CHAR_SPACE    : x_ch = 's'  ;  break;    /*   115 s = 183 ·   */
                          /*---(grouping)------------------*/
   case G_CHAR_SLPAREN  : x_ch = '('  ;  break;    /*    40 ( = 184 ¸   */
   case G_CHAR_SRPAREN  : x_ch = ')'  ;  break;    /*    41 ) = 185 ¹   */
   case G_CHAR_SLBRACK  : x_ch = '['  ;  break;    /*    91 [ = 186 º   */
   case G_CHAR_SRBRACK  : x_ch = ']'  ;  break;    /*    93 ] = 187 »   */
   case G_CHAR_SLCHEV   : x_ch = '{'  ;  break;    /*   123 { = 188 ¼   */
   case G_CHAR_SRCHEV   : x_ch = '}'  ;  break;    /*   125 } = 189 ½   */
   case G_CHAR_DEL      : x_ch = 'd'  ;  break;    /*   100 d = 190 ¾   */
   case G_CHAR_BS       : x_ch = 'b'  ;  break;    /*    98 b = 191 ¿   */
                          /*---(super/sub)-----------------*/
   case G_CHAR_SUB0     : x_ch = '5'  ;  break;    /*    53 5 = 192 À   */
   case G_CHAR_SUB1     : x_ch = '6'  ;  break;    /*    54 6 = 193 Á   */
   case G_CHAR_SUB2     : x_ch = '7'  ;  break;    /*    55 7 = 194 Â   */
   case G_CHAR_SUB3     : x_ch = '8'  ;  break;    /*    56 8 = 195 Ã   */
   case G_CHAR_SUB4     : x_ch = '9'  ;  break;    /*    57 9 = 196 Ä   */
   case G_CHAR_SUBN     : x_ch = 'z'  ;  break;    /*   122 z = 197 Å   */
   case G_CHAR_POW2     : x_ch = '2'  ;  break;    /*    50 2 = 198 Æ   */
   case G_CHAR_POW3     : x_ch = '3'  ;  break;    /*    51 3 = 199 Ç   */
   case G_CHAR_POW4     : x_ch = '4'  ;  break;    /*    52 4 = 200 È   */
   case G_CHAR_POWX     : x_ch = 'x'  ;  break;    /*   120 x = 201 É   */
   case G_CHAR_POWY     : x_ch = 'y'  ;  break;    /*   121 y = 202 Ê   */
   case G_CHAR_SMHASH   : x_ch = '#'  ;  break;    /*    35 # = 203 Ë   */
   case G_CHAR_DEGREE   : x_ch = 'a'  ;  break;    /*    97 a = 204 Ì   */
   case G_CHAR_RADIAN   : x_ch = 'r'  ;  break;    /*   114 r = 205 Í   */
   case G_CHAR_SQRT     : x_ch = '1'  ;  break;    /*    49 1 = 206 Î   */
                          /*---(logic)---------------------*/
   case G_CHAR_AND      : x_ch = '&'  ;  break;    /*    38 & = 208 Ð   */
   case G_CHAR_OR       : x_ch = '|'  ;  break;    /*   124 | = 209 Ñ   */
   case G_CHAR_XOR      : x_ch = '^'  ;  break;    /*    94 ^ = 210 Ò   */
   case G_CHAR_UP       : x_ch = 'k'  ;  break;    /*   107 k = 211 Ó   */
   case G_CHAR_DOWN     : x_ch = 'j'  ;  break;    /*   106 j = 212 Ô   */
   case G_CHAR_RIGHT    : x_ch = 'l'  ;  break;    /*   108 l = 213 Õ   */
   case G_CHAR_LEFT     : x_ch = 'h'  ;  break;    /*   104 h = 214 Ö   */
   case G_CHAR_LE       : x_ch = '<'  ;  break;    /*    60 < = 221 Ý   */
   case G_CHAR_GE       : x_ch = '>'  ;  break;    /*    62 > = 222 Þ   */
   case G_CHAR_NE       : x_ch = '/'  ;  break;    /*    61 = = 223 ß   */
   case G_CHAR_EQUIV    : x_ch = '='  ;  break;    /*    47 = = 229 å   */
   case G_CHAR_APPROX   : x_ch = '~'  ;  break;    /*   126 ~ = 230 æ   */
   case G_CHAR_RANGE    : x_ch = '$'  ;  break;    /*    36 $ = 177 ±   */
                          /*---(greek)---------------------*/
   case G_CHAR_ALPHA    : x_ch = 'A'  ;  break;    /*    65 A = 232 è   */
   case G_CHAR_BETA     : x_ch = 'B'  ;  break;    /*    66 B = 233 é   */
   case G_CHAR_GAMMA    : x_ch = 'G'  ;  break;    /*    71 G = 234 ê   */
   case G_CHAR_DELTA    : x_ch = 'D'  ;  break;    /*    68 D = 235 ë   */
   case G_CHAR_EPSILON  : x_ch = 'E'  ;  break;    /*    69 E = 236 ì   */
   case G_CHAR_ZETA     : x_ch = 'Z'  ;  break;    /*    90 Z = 237 í   */
   case G_CHAR_ETA      : x_ch = 'H'  ;  break;    /*    72 H = 238 î   */
   case G_CHAR_THETA    : x_ch = 'Y'  ;  break;    /*    89 Y = 239 ï   */
   case G_CHAR_IOTA     : x_ch = 'I'  ;  break;    /*    73 I = 240 ð   */
   case G_CHAR_KAPPA    : x_ch = 'K'  ;  break;    /*    75 K = 241 ñ   */
   case G_CHAR_LAMBDA   : x_ch = 'L'  ;  break;    /*    76 L = 242 ò   */
   case G_CHAR_MU       : x_ch = 'M'  ;  break;    /*    77 M = 243 ó   */
   case G_CHAR_NU       : x_ch = 'N'  ;  break;    /*    78 N = 244 ô   */
   case G_CHAR_XI       : x_ch = 'X'  ;  break;    /*    88 X = 245 õ   */
   case G_CHAR_OMICRON  : x_ch = 'O'  ;  break;    /*    79 O = 246 ö   */
   case G_CHAR_PI       : x_ch = 'P'  ;  break;    /*    80 P = 247 ÷   */
   case G_CHAR_RHO      : x_ch = 'R'  ;  break;    /*    82 R = 248 ø   */
   case G_CHAR_SIGMA    : x_ch = 'S'  ;  break;    /*    83 S = 249 ù   */
   case G_CHAR_TAU      : x_ch = 'T'  ;  break;    /*    84 T = 250 ú   */
   case G_CHAR_UPSILON  : x_ch = 'U'  ;  break;    /*    85 U = 251 û   */
   case G_CHAR_PHI      : x_ch = 'F'  ;  break;    /*    70 F = 252 ü   */
   case G_CHAR_CHI      : x_ch = 'C'  ;  break;    /*    67 C = 253 ý   */
   case G_CHAR_PSI      : x_ch = 'Q'  ;  break;    /*    81 Q = 254 þ   */
   case G_CHAR_OMEGA    : x_ch = 'W'  ;  break;    /*    87 W = 255 ÿ   */
   default : return rce;  break;
   }
   yx11_keys__normal (x_ch);
   s_orig = a_ch;
   s_char = x_ch;
   s_slash = 1;
   return x_ch;
}

char
yx11_keys__extra        (uchar a_ch)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        x_ch        =    0;
   /*---(defense)------------------------*/
   yx11_keys__reset ();
   switch (a_ch) {
   /*> case G_CHAR_BECAUSE    : x_ch = 't'  ;    break;                               <* 
    *> case G_CHAR_FOREACH    : x_ch = 'a'  ;    break;                               <* 
    *> case G_CHAR_EXISTS     : x_ch = 'e'  ;    break;                               <* 
    *> case G_CHAR_IMPLIES    : x_ch = 'i'  ;    break;                               <* 
    *> case G_CHAR_PROVES     : x_ch = 'p'  ;    break;                               <* 
    *> case G_CHAR_FORCES     : x_ch = 'f'  ;    break;                               <* 
    *> case G_CHAR_CONTRA     : x_ch = 'c'  ;    break;                               <* 
    *> case G_CHAR_UNION      : x_ch = 'j'  ;    break;                               <* 
    *> case G_CHAR_INTERSECT  : x_ch = 'k'  ;    break;                               <* 
    *> case G_CHAR_SUBSET     : x_ch = 'h'  ;    break;                               <* 
    *> case G_CHAR_SUPERSET   : x_ch = 'l'  ;    break;                               <* 
    *> case G_CHAR_MEMBER     : x_ch = 'm'  ;    break;                               <* 
    *> case G_CHAR_ONLYIF     : x_ch = 'o'  ;    break;                               <*/
   /*> default : return rce;  break;                                                  <*/
   }
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
      case (uchar) 'Õ' :
         s_long  = 0xff09;
         DEBUG_DESK   yLOG_value   ("blitz"    , s_long);
         break;
      case (uchar) 'Ö' :
         s_long  = 0xff08;
         DEBUG_DESK   yLOG_value   ("unblitz"  , s_long);
         break;
      case (uchar) 'Ó' :
         DEBUG_DESK   yLOG_note    ("control Ó or hyper ÓÓ");
         if (s_mods [S_CTRL] == 'y') {
            s_mods [S_HYPE] = 'y';
            s_mods [S_CTRL] = '-';
         } else s_mods [S_CTRL] = 'y';
         continue;
         break;
      case (uchar) 'Ô' :
         DEBUG_DESK   yLOG_note    ("alt Ô or super ÔÔ");
         if (s_mods [S_ALTT] == 'y') {
            s_mods [S_SUPR] = 'y';
            s_mods [S_ALTT] = '-';
         } else s_mods [S_ALTT] = 'y';
         continue;
         break;
      case (uchar) '²' :
         s_long  = XK_space;
         DEBUG_DESK   yLOG_value   ("spacer"   , s_long);
         break;
      case (uchar) '·' :
         DEBUG_DESK   yLOG_note    ("spacer");
         continue;
         break;
      case (uchar) '\n': case (uchar) '¦' :
         s_long  = 0xff0d;
         DEBUG_DESK   yLOG_value   ("return"   , s_long);
         break;
      case (uchar) '\e': case (uchar) '¥' :
         s_long  = 0xff1b;
         DEBUG_DESK   yLOG_value   ("escape"   , s_long);
         break;
      default          : 
         if        (a_keys [i] <  32) {
            continue; 
         } else if (a_keys [i] < 127) {
            yx11_keys__normal   (a_keys [i]);
         } else if (a_keys [i] < 161) {
            continue; 
         } else  {
            yx11_keys__reset ();
            rc = yx11_keys__extended (a_keys [i]);
            if (rc < 0)  rc = yx11_keys__extra (a_keys [i]);
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
      snprintf (unit_answer, LEN_RECD, "KEYS symkey      : orig %c/%-3d, char %c/%-3d, sym %-8d, slash %d, mods %-6.6s", s_orig, (uchar) s_orig, s_char, (uchar) s_char, s_long, s_slash, s_mods);
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





