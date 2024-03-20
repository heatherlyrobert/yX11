#include "yX11.h"
#include "yX11_priv.h"



#define    MAX_SHORT   50
typedef struct cSHORT tSHORT;
static struct cSHORT {
   uchar       abbr;
   long        id;
};
static tSHORT  s_shorts    [MAX_SHORT];
int            s_nshort    = 0;  /* public to assist unit testing    */



char
yx11_yvikeys_purge      (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_nshort = 0;
   for (i = 0; i < MAX_SHORT; ++i) {
      s_shorts [i].abbr      = '-';
      s_shorts [i].id        = -1;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yx11_yvikeys_valid      (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   uchar      *x_valid     = ".*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789èéêëìíîïðñòóôõö÷øùúûüýþÿ";
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DESK   yLOG_schar   (a_abbr);
   --rce;  if (a_abbr == 0 || strchr (x_valid, a_abbr) == NULL) {
      DEBUG_DESK   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}

long
yx11_yvikeys_exists     (uchar a_abbr)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   long        x_win       =   -1;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   DEBUG_DESK   yLOG_schar   (a_abbr);
   /*---(quick)--------------------------*/
   if (a_abbr == '*') {
      DEBUG_DESK   yLOG_snote   ("host");
      x_win = YX_BASE;
      DEBUG_DESK   yLOG_sint    (x_win);
      DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
      return x_win;
   }
   if (a_abbr == '.') {
      DEBUG_DESK   yLOG_snote   ("current");
      x_win = yX11_win_current  (NULL, NULL);
      DEBUG_DESK   yLOG_sint    (x_win);
      DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
      return x_win;
   }
   /*---(walk entries)-------------------*/
   DEBUG_DESK   yLOG_sint    (s_nshort);
   --rce;  for (i = 0; i < s_nshort; ++i) {
      if (s_shorts [i].abbr != a_abbr)  continue;
      DEBUG_DESK   yLOG_sint    (s_shorts [i].id);
      DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
      return s_shorts [i].id;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexitr  (__FUNCTION__, rce);
   return rce;
}

char
yx11_yvikeys_addshort   (uchar a_abbr, long a_win)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DESK   yLOG_sint    (s_nshort);
   DEBUG_DESK   yLOG_sint    (s_nshort);
   --rce;  if (s_nshort >= MAX_SHORT) {
      DEBUG_DESK   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(add)----------------------------*/
   s_shorts [s_nshort].abbr = a_abbr;
   s_shorts [s_nshort].id   = a_win;
   ++s_nshort;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                      yvikeys helpers                         ----===*/
/*====================------------------------------------====================*/
static void      o___YVIKEYS_________________o (void) {;}

char
yX11_yvikeys_name       (uchar *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x_len       =    0;
   long        x_win       =   -1;
   uchar       x_abbr      =  '-';
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DESK   yLOG_point   ("a_string"  , a_string);
   --rce;  if (a_string == NULL || strlen (a_string) < 3) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DESK   yLOG_info    ("a_string"  , a_string);
   /*---(abbrev)-------------------------*/
   x_abbr = a_string [0];
   DEBUG_DESK   yLOG_char    ("x_abbr"    , x_abbr);
   rc = yx11_yvikeys_valid (x_abbr);
   DEBUG_DESK   yLOG_value   ("valid"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(exists)-------------------------*/
   x_win = yx11_win__regex (a_string + 2, NULL, &c);
   DEBUG_DESK   yLOG_value   ("x_win"     , x_win);
   DEBUG_DESK   yLOG_value   ("c"         , c);
   --rce;  if (x_win < 0 || c != 1) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add)----------------------------*/
   rc = yx11_yvikeys_addshort (x_abbr, x_win);
   DEBUG_DESK   yLOG_value   ("addshort"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_yvikeys_term       (uchar *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   long        x_win       =   -1;
   uchar       x_abbr      =  '-';
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   x_abbr = a_string [0];
   DEBUG_DESK   yLOG_char    ("x_abbr"    , x_abbr);
   rc = yx11_yvikeys_valid (x_abbr);
   DEBUG_DESK   yLOG_value   ("valid"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(exists)-------------------------*/
   x_win = yx11_yvikeys_exists (x_abbr);
   DEBUG_DESK   yLOG_value   ("exists"    , x_win);
   --rce;  if (x_win >= 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   DEBUG_DESK   yLOG_complex ("parts"     , "desk %d, size %c, scrn %c, color %s", a_string [2] - '0', a_string [4], a_string [6], a_string + 8);
   x_win = yX11_term (a_string [2], a_string [4], a_string [6], a_string + 8);
   DEBUG_DESK   yLOG_value   ("term"      , x_win);
   --rce;  if (x_win < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(add)----------------------------*/
   rc = yx11_yvikeys_addshort (x_abbr, x_win);
   DEBUG_DESK   yLOG_value   ("addshort"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yx11_yvikeys__sendkeys   (char a_mode, uchar *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   long        x_win       =   -1;
   uchar       x_abbr      =  '-';
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   x_abbr = a_string [0];
   DEBUG_DESK   yLOG_char    ("x_abbr"    , x_abbr);
   rc = yx11_yvikeys_valid (x_abbr);
   DEBUG_DESK   yLOG_value   ("valid"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(exists)-------------------------*/
   x_win = yx11_yvikeys_exists (x_abbr);
   DEBUG_DESK   yLOG_value   ("exists"    , x_win);
   --rce;  if (x_win < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(create)-------------------------*/
   DEBUG_DESK   yLOG_char    ("a_mode"    , a_mode);
   DEBUG_DESK   yLOG_info    ("keys"      , a_string + 2);
   if (a_mode == 'b')  rc = yX11_keys_blitz (x_win, a_string + 2);
   else                rc = yX11_keys_send  (x_win, a_string + 2);
   DEBUG_DESK   yLOG_value   ("sendkeys"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_yvikeys_sendkeys   (uchar *a_string)
{
   return yx11_yvikeys__sendkeys ('-', a_string);
}

char
yX11_yvikeys_blitzkeys  (uchar *a_string)
{
   return yx11_yvikeys__sendkeys ('b', a_string);
}

char
yx11_yvikeys__desktop   (char a_desk)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        o           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_value   ("a_desk"    , a_desk);
   /*---(direct id)----------------------*/
   DEBUG_DESK   yLOG_value   ("s_ndesk"   , s_ndesk);
   --rce;  if  (a_desk >=  0  && a_desk < s_ndesk      ) {
      DEBUG_DESK   yLOG_exit   (__FUNCTION__);
      return a_desk;
   }
   else if     (a_desk >= '0' && a_desk < s_ndesk + '0') {
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return a_desk - '0';
   }
   /*---(cursoring)----------------------*/
   else {
      DEBUG_DESK   yLOG_char    ("a_desk"    , a_desk);
      o  = yX11_desk_current (NULL);
      switch (a_desk) {
      case '[' : a_desk = 0;                    break;
      case '<' : a_desk = o - 1;                break;
      case '.' : a_desk = o;                    break;
      case '>' : a_desk = o + 1;                break;
      case ']' : a_desk = s_ndesk - 1;          break;
      case -1  :
                 DEBUG_DESK   yLOG_exit    (__FUNCTION__);
                 return a_desk;
                 break;
      default  :
                 DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
                 return rce;

      }
   }
   /*---(limits)-------------------------*/
   if      (a_desk < 0)         a_desk = 0;
   else if (a_desk >= s_ndesk)  a_desk = s_ndesk - 1;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return a_desk;
}

char
yx11_yvikeys__winplay   (char a_type, uchar a_abbr, char d, int x, int y, int w, int t)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   long        x_win       =   -1;
   char        nd          =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_DESK   yLOG_char    ("a_abbr"    , a_abbr);
   rc = yx11_yvikeys_valid (a_abbr);
   DEBUG_DESK   yLOG_value   ("valid"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(exists)-------------------------*/
   x_win = yx11_yvikeys_exists (a_abbr);
   DEBUG_DESK   yLOG_value   ("exists"    , x_win);
   --rce;  if (x_win < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(desktop)------------------------*/
   switch (a_type) {
   case 'b' :  nd = yX11_desk_current (NULL);                        break;
   case 'g' :  yX11_win_where (x_win, &nd, NULL, NULL, NULL, NULL);  break;
   default  :  nd = yx11_yvikeys__desktop (d);                       break;
   }
   DEBUG_DESK   yLOG_value   ("nd"        , nd);
   --rce;  if (nd < -1) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(modify)-------------------------*/
   rc = yX11_win_exact (x_win, nd, x, y, w, t);
   DEBUG_DESK   yLOG_value   ("winexact"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(desktop)------------------------*/
   switch (a_type) {
   case 't' : case 'g' :
      if (nd != -1)  yX11_desk_goto (nd + '0');
      break; 
   }
   /*---(activate)-----------------------*/
   switch (a_type) {
   case 'b' : case 't' : case 'g' :
      yx11_win__activate (x_win, nd);
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_yvikeys_winexact   (uchar a_abbr, char d, int x, int y, int w, int t)
{
   return yx11_yvikeys__winplay ('e', a_abbr, d, x, y, w, t);
}

char
yX11_yvikeys_winplace   (char *a_string)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         x, y, w, t;
   /*---(get size)-----------------------*/
   rc = yx11_loc_sizing (a_string [4], a_string [6], &x, &y, &w, &t);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   return yx11_yvikeys__winplay ('e', a_string [0], a_string [2], x, y, w, t);
}

char
yX11_yvikeys_winbring   (uchar a_abbr)
{
   return yx11_yvikeys__winplay ('b', a_abbr, '.', -1, -1, -1, -1);
}

char
yX11_yvikeys_wingoto    (uchar a_abbr)
{
   return yx11_yvikeys__winplay ('g', a_abbr, -1, -1, -1, -1, -1);
}

char
yX11_yvikeys_winsend    (uchar a_abbr, char d)
{
   return yx11_yvikeys__winplay ('s', a_abbr, d, -1, -1, -1, -1);
}

char
yX11_yvikeys_wintake    (uchar a_abbr, char d)
{
   return yx11_yvikeys__winplay ('t', a_abbr, d, -1, -1, -1, -1);
}

char
yX11_yvikeys_mydesk     (char d)
{
   char        rc          =    0;
   long        x_win       =   -1;
   char        x_desk      =    0;
   x_win  = yX11_win_current  (NULL, NULL);
   yX11_desk_goto     (d);
   yx11_loc_exact     (x_win, d, -1, -1, -1, -1);
   yx11_win__activate (x_win, d);
   return rc;
}

char
yX11_yvikeys_myhome     (int x, int y)
{
   char        rc          =    0;
   long        x_win       =   -1;
   int         nx          =    0;
   int         ny          =    0;
   char        x_tries     =    0;
   x_win  = yX11_win_current  (NULL, NULL);
   while (x != nx || y != ny)  {
      rc = yX11_win_exact (x_win, -1, x, y, -1, -1);
      usleep (100000);  /* appears to have a update/caching delay */
      yx11_desk_inventory ('y');
      rc = yX11_win_where (x_win, NULL, &nx, &ny, NULL, NULL);
      if (++x_tries > 3)  break;
   }
   return rc;
}

char
yX11_yvikeys_mysize     (int w, int t)
{
   char        rc          =    0;
   long        x_win       =   -1;
   int         nw          =    0;
   int         nt          =    0;
   char        x_tries     =    0;
   x_win  = yX11_win_current  (NULL, NULL);
   while (w != nw || t != nt)  {
      rc = yX11_win_exact (x_win, -1, -1, -1, w, t);
      usleep (100000);  /* appears to have a update/caching delay */
      yx11_desk_inventory ('y');
      rc = yX11_win_where (x_win, NULL, NULL, NULL, &nw, &nt);
      if (++x_tries > 3)  break;
   }
   return rc;
}

char
yX11_yvikeys_mysizer    (int w, int t, int m, int s)
{
   char        rc          =    0;
   long        x_win       =   -1;
   int         ow          =    0;
   int         ot          =    0;
   int         i           =    0;
   int         nw          =    0;
   int         nt          =    0;
   x_win  = yX11_win_current  (NULL, NULL);
   rc = yX11_win_where (x_win, NULL, NULL, NULL, &ow, &ot);
   for (i = 1; i <= s; ++i) {
      nw = w + (w - ow) / s;
      nt = t + (t - ot) / s;
      rc = yX11_yvikeys_mysize (nw, nt);
      usleep (1000 * m);
   }
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
yX11__unit_yvikeys      (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL];
   char        t           [LEN_RECD ];
   int         i           =    0;
   /*---(initialize)---------------------*/
   ystrlcpy (unit_answer, "YVIKEYS unit     : unknown request", LEN_RECD);
   yx11_full_refresh ('y');
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "count"   , 20)  == 0) {
      snprintf (unit_answer, LEN_RECD, "YVIKEYS count    : %d", s_nshort);
   }
   else if (strncmp (a_question, "entry"   , 20)  == 0) {
      if (a_num >= 0 && a_num < s_nshort)
         snprintf (unit_answer, LEN_RECD, "YVIKEYS ent (%2d) : %c  %d", a_num, s_shorts [a_num].abbr, s_shorts [a_num].id);
      else
         snprintf (unit_answer, LEN_RECD, "YVIKEYS ent (%2d) : -  -", a_num);
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}
