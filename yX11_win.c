#include "yX11.h"
#include "yX11_priv.h"



#define    MAX_WINS   100
typedef struct cWINS tWINS;
static struct cWINS {
   long        id;
   char        curr;
   char        desk;
   char        title       [LEN_HUND];
   short       left;
   short       topp;
   short       wide;
   short       tall;
};
static tWINS s_wins  [MAX_WINS];
int          s_nwin       = 0;  /* public to assist unit testing    */



typedef struct cSHORT tSHORT;
static struct cSHORT {
   long        id;
   char        label       [LEN_LABEL];
};
static tSHORT  s_shorts    [MAX_WINS];
int            s_nshort    = 0;  /* public to assist unit testing    */



/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
yx11_win_purge          (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_nwin = 0;
   for (i = 0; i < MAX_WINS; ++i) {
      s_wins [i].id        = -1;
      s_wins [i].curr      = '-';
      s_wins [i].desk      = -1;
      s_wins [i].title [0] = '\0';
      s_wins [i].left      =  0;
      s_wins [i].topp      =  0;
      s_wins [i].wide      =  0;
      s_wins [i].tall      =  0;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yx11_win_short_purge    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_nshort = 0;
   for (i = 0; i < MAX_WINS; ++i) {
      s_shorts [i].id        = -1;
      s_shorts [i].label [0] = '\0';
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   0;
}

char
yx11_win_init           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   yx11_win_purge ();
   yx11_win_short_purge ();
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    window inventory                          ----===*/
/*====================------------------------------------====================*/
static void      o___INVENTORY_______________o (void) {;}

char
yx11_win_inventory      (char a_real)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   char        t           [LEN_LABEL] = "";
   int         i           =    0;
   int         x_len       =    0;
   double      x_val       =  0.0;
   char       *p           = NULL;
   char       *q           = " ";
   char       *r           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   yx11_win_purge  ();
   /*---(generate data)------------------*/
   if (a_real == 'y')  rc = system ("wmctrl -G -l > /tmp/yX11_windows.txt");
   /*---(open)---------------------------*/
   f = fopen ("/tmp/yX11_windows.txt", "rt");
   DEBUG_DESK   yLOG_point   ("fopen"     , f);
   --rce;  if (f == NULL) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read and inventory)-------------*/
   i = 0;
   while (1) {
      fgets (x_recd, LEN_RECD, f);
      if (feof (f))     break;
      x_len = strlen (x_recd);
      if (x_len < 10)  continue;
      if (x_recd [x_len - 1] == '\n')   x_recd [--x_len] = '\0';
      DEBUG_DESK   yLOG_complex ("x_recd"    , "%d) %2d[%s]", s_nwin, x_len, x_recd);
      if (x_len < 20)  continue;
      p = strtok_r (x_recd, q, &r);
      strl2hex (p, &x_val, LEN_LABEL);
      s_wins [s_nwin].id = x_val;
      DEBUG_DESK   yLOG_value   ("id"        , s_wins [s_nwin].id);
      p = strtok_r (NULL  , q, &r);
      s_wins [s_nwin].desk = atoi (p);
      DEBUG_DESK   yLOG_value   ("desk"      , s_wins [s_nwin].desk);
      p = strtok_r (NULL  , q, &r);
      s_wins [s_nwin].left = atoi (p);
      DEBUG_DESK   yLOG_value   ("left"      , s_wins [s_nwin].left);
      p = strtok_r (NULL  , q, &r);
      s_wins [s_nwin].topp = atoi (p);
      p = strtok_r (NULL  , q, &r);
      s_wins [s_nwin].wide = atoi (p);
      p = strtok_r (NULL  , q, &r);
      s_wins [s_nwin].tall = atoi (p);
      p = strtok_r (NULL  , q, &r);
      x_len = strlen (p);
      p += x_len + 1;
      strltrim (p, ySTR_BOTH, LEN_RECD);
      strlcpy (s_wins [s_nwin].title, p, LEN_HUND);
      DEBUG_DESK   yLOG_complex ("result"    , "%d %10.10x %2d %s", s_nwin, s_wins [s_nwin].id, s_wins [s_nwin].desk, s_wins  [s_nwin].title);
      yx11_desk_addwin (s_wins [s_nwin].desk);
      ++s_nwin;
   }
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_DESK   yLOG_value   ("fclose"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yx11_win_current        (char a_real)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   int         i           =    0;
   int         x_len       =    0;
   double      x_val       =  0.0;
   long        x_id        =    0;
   char       *p           = NULL;
   char       *q           = "\t ";
   char       *r           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(generate data)------------------*/
   if (a_real == 'y') {
      rc = system ("xprop -root 32x '\t$0' _NET_ACTIVE_WINDOW > /tmp/yX11_active.txt");
      system ("echo \"\n\" >> /tmp/yX11_active.txt");
   }
   /*---(open)---------------------------*/
   f = fopen ("/tmp/yX11_active.txt", "rt");
   DEBUG_DESK   yLOG_point   ("fopen"     , f);
   --rce;  if (f == NULL) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(read and inventory)-------------*/
   fgets (x_recd, LEN_RECD, f);
   x_len = strlen (x_recd);
   if (x_recd [x_len - 1] == '\n')   x_recd [--x_len] = '\0';
   DEBUG_DESK   yLOG_complex ("x_recd"    , "%2d[%s]", x_len, x_recd);
   p = strtok_r (x_recd, q, &r);
   DEBUG_DESK   yLOG_info    ("p"         , p);
   p = strtok_r (NULL  , q, &r);
   DEBUG_DESK   yLOG_info    ("p"         , p);
   strl2hex (p, &x_val, LEN_LABEL);
   x_id = x_val;
   DEBUG_DESK   yLOG_value   ("x_id"      , x_id);
   /*---(close)--------------------------*/
   rc = fclose (f);
   DEBUG_DESK   yLOG_value   ("fclose"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(mark window)--------------------*/
   for (i = 0; i < s_nwin; ++i) {
      DEBUG_DESK   yLOG_complex ("checking"  , "%d %10d %2d %s", i, s_wins [i].id, s_wins [i].desk, s_wins  [i].title);
      if (s_wins [i].id != x_id)    continue;
      s_wins [i].curr = '*';
      break;
   }
   --rce;  if (i == s_nwin) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_window_current     (char *a_name, char *a_desk)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        n           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_name != NULL)  strlcpy (a_name, "", LEN_LABEL);
   if (a_desk != NULL)  *a_desk = -1;
   yx11_full_refresh  ('y');
   DEBUG_DESK   yLOG_value   ("s_nwin"    , s_nwin);
   /*---(search)-------------------------*/
   for (i = 0; i < s_nwin; ++i) {
      /*---(filter)----------------------*/
      DEBUG_DESK   yLOG_complex ("checking"  , "%d %10d %2d %s", i, s_wins [i].id, s_wins [i].desk, s_wins  [i].title);
      if (s_wins [i].curr != '*')  continue;
      /*---(found)-----------------------*/
      DEBUG_DESK   yLOG_note    ("FOUND");
      if (a_name != NULL)  strlcpy (a_name, s_wins [i].title, LEN_LABEL);
      if (a_desk != NULL)  *a_desk = s_wins [i].desk;
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return i;
   }
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

char
yX11_where              (long a_win, char *d, int *x, int *y, int *w, int *t)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        n           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_complex ("args"      , "%ld, %p, %p, %p, %p, %p", a_win, d, x, y, w, t);
   /*---(defaults)-----------------------*/
   if (d != NULL)  *d = 0;
   if (x != NULL)  *x = 0;
   if (y != NULL)  *y = 0;
   if (w != NULL)  *w = 0;
   if (t != NULL)  *t = 0;
   /*---(update data)--------------------*/
   yx11_full_refresh  ('y');
   /*---(search)-------------------------*/
   for (i = 0; i < s_nwin; ++i) {
      /*---(filter)----------------------*/
      DEBUG_DESK   yLOG_complex ("checking"  , "%d %10d %2d %s", i, s_wins [i].id, s_wins [i].desk, s_wins  [i].title);
      if (s_wins [i].id != a_win)  continue;
      /*---(found)-----------------------*/
      DEBUG_DESK   yLOG_note    ("FOUND");
      if (d != NULL)  *d = s_wins [i].desk;
      if (x != NULL)  *x = s_wins [i].left;
      if (y != NULL)  *y = s_wins [i].topp;
      if (w != NULL)  *w = s_wins [i].wide;
      if (t != NULL)  *t = s_wins [i].tall;
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return i;
   }
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}



/*====================------------------------------------====================*/
/*===----                     terminal control                         ----===*/
/*====================------------------------------------====================*/
static void      o___TERMS___________________o (void) {;}

long
yX11_term               (char a_desk, char a_abbr, char a_scrn, char *a_color)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        x_pre       [LEN_HUND]  = "";
   char        x_loc       [LEN_HUND]  = "";
   char        x_suf       [LEN_HUND]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   int         x, y, w, t;
   int         c           =    0;
   int         x_tries     =    0;
   long        x_id        =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = yx11_base_defense ();
   DEBUG_DESK   yLOG_value   ("defense"   , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   DEBUG_DESK   yLOG_value   ("a_desk"    , a_desk);
   DEBUG_DESK   yLOG_value   ("s_ndesk"   , s_ndesk);
   --rce;  if (a_desk < 0 || a_desk >= s_ndesk) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(location)-----------------------*/
   rc = yx11_loc_sizing (a_abbr, a_scrn, &x, &y, &w, &t);
   DEBUG_DESK   yLOG_value   ("location"  , rc);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DESK   yLOG_complex ("place"     , "%4dx, %4dy, %4dw, %4dt", x, y, w, t);
   /*---(inventory)----------------------*/
   yX11_window_current (NULL, NULL);
   c = s_nwin;
   /*---(command)------------------------*/
   sprintf (x_pre, "Eterm --tint %-10.10s --trans --buttonbar 0 --scrollbar 0", a_color);
   sprintf (x_loc, "--desktop %d --geometry %dx%d+%d+%d", a_desk, w / 6 - 2, t / 10 - 2, x, y);
   sprintf (x_suf, "> /dev/null 2> /dev/null &");
   sprintf (x_cmd, "%s %s %s", x_pre, x_loc, x_suf);
   DEBUG_DESK   yLOG_info    ("command"   , x_cmd);
   rc = system  (x_cmd);
   DEBUG_DESK   yLOG_value   ("system"    , rc);
   /*---(sleep)--------------------------*/
   while (c == s_nwin && x_tries < 5) {
      sleep (1);
      yX11_window_current (NULL, NULL);
      ++x_tries;
   }
   --rce;  if (c == s_nwin) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_id = s_wins [s_nwin - 1].id;
   DEBUG_DESK   yLOG_value   ("x_id"      , x_id);
   /*---(place)--------------------------*/
   yX11_exact (x_id, a_desk, x, y, w, t);
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return x_id;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
yX11__unit_win          (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL];
   char        t           [LEN_RECD ];
   int         i           =    0;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "WIN unit         : unknown request", LEN_RECD);
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "count"     , 20)  == 0) {
      snprintf (unit_answer, LEN_RECD, "WIN  count       : %d", s_nwin);
   }
   else if (strncmp (a_question, "entry"     , 20)  == 0) {
      if (a_num >= 0 && a_num < s_nwin) {
         sprintf (t, "%2d[%-.20s]", strlen (s_wins [a_num].title), s_wins [a_num].title);
         snprintf (unit_answer, LEN_RECD, "WIN  entry   (%2d): %-10d %c %2d %-24.24s %4dx %4dy %4dw %4dt", a_num, s_wins [a_num].id, s_wins [a_num].curr, s_wins [a_num].desk, t, s_wins [a_num].left, s_wins [a_num].topp, s_wins [a_num].wide, s_wins [a_num].tall);
      } else {
         snprintf (unit_answer, LEN_RECD, "WIN  entry   (--): -          -  -  -[]                        -x    -y    -w    -t");
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



