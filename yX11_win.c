#include "yX11.h"
#include "yX11_priv.h"



/*
 *   XGetInputFocus (YX_DISP, &x_win, &x_int);
 *
 *   XSetInputFocus (YX_DISP,  x_win, RevertToParent, timestamp);
 *
 *
 *
 *
 *
 */


#define    MAX_WINS   100
typedef struct cWINS tWINS;
static struct cWINS {
   /*---(reference)---------*/
   long        id;
   /*---(key data)----------*/
   char        desk;
   char        title       [LEN_FULL];
   char        curr;
   char        type;                    /* window use based on title          */
   /*---(position)----------*/
   short       left;
   short       topp;
   short       wide;
   short       tall;
   char        size;
   char        scrn;
   char        locn;
   /*---(done)--------------*/
};
static tWINS s_wins  [MAX_WINS];
int          s_nwin       = 0;  /* public to assist unit testing    */




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
      /*---(reference)------*/
      s_wins [i].id        = -1;
      /*---(key data)-------*/
      s_wins [i].desk      = -1;
      s_wins [i].title [0] = '\0';
      s_wins [i].curr      = '·';
      s_wins [i].type      = '·';
      /*---(position)-------*/
      s_wins [i].left      = -1;
      s_wins [i].topp      = -1;
      s_wins [i].wide      = -1;
      s_wins [i].tall      = -1;
      s_wins [i].size      = '·';
      s_wins [i].scrn      = '·';
      s_wins [i].locn      = '·';
      /*---(done)-----------*/
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
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
   yx11_yvikeys_purge ();
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    window inventory                          ----===*/
/*====================------------------------------------====================*/
static void      o___INVENTORY_______________o (void) {;}

char
yx11_win__type            (cchar *a_title)
{
   if (a_title == NULL)  return '·';
   if (strstr (a_title, "VIM") != NULL)  return 'v';
   return '·';
}

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
      strlcpy (s_wins [s_nwin].title, p, LEN_FULL);
      s_wins [s_nwin].type =  yx11_win__type (p);
      s_wins [s_nwin].size =  yx11_loc_by_size (s_wins [s_nwin].wide, s_wins [s_nwin].tall, NULL);
      s_wins [s_nwin].locn =  yx11_loc_by_locn (s_wins [s_nwin].size, s_wins [s_nwin].left, s_wins [s_nwin].topp, NULL, &(s_wins [s_nwin].scrn));
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
   for (i = 0; i < s_nwin; ++i)  s_wins [i].curr = '·';
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

long
yX11_win_current        (char *a_name, char *a_desk)
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
      return s_wins [i].id;
   }
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

char
yX11_win_by_name        (char a_move, long *r_id, char *r_desk, char *r_title, char *r_type, char *r_curr, short *r_left, short *r_topp, char *r_scrn, char *r_locn, short *r_wide, short *r_tall, char *r_size)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_FULL]  = "";
   char        x_orig      [LEN_LABEL] = "";
   long        x           =    0;
   static char n           =    0;
   char        m           =   -1;
   char        x_tries     =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_complex ("args"      , "%c, %p, %p", a_move, r_title, r_id);
   /*---(prepare)------------------------*/
   if (r_id    != NULL)  *r_id   = -1;
   if (r_desk  != NULL)  *r_desk = -1;
   if (r_title != NULL)  strlcpy (r_title, "", LEN_FULL);
   if (r_type  != NULL)  *r_type = '·';
   if (r_curr  != NULL)  *r_curr = '·';
   if (r_left  != NULL)  *r_left = -1;
   if (r_topp  != NULL)  *r_topp = -1;
   if (r_scrn  != NULL)  *r_scrn = '·';
   if (r_locn  != NULL)  *r_locn = '·';
   if (r_wide  != NULL)  *r_wide = -1;
   if (r_tall  != NULL)  *r_tall = -1;
   if (r_size  != NULL)  *r_size = '·';
   /*---(prepare)------------------------*/
   DEBUG_DESK   yLOG_value   ("s_nwin"    , s_nwin);
   /*---(cursoring)----------------------*/
   switch (a_move) {
   case YDLST_HEAD :
      DEBUG_DESK   yLOG_note    ("move to first");
      n = 0;
      break;
   case YDLST_NEXT :
      DEBUG_DESK   yLOG_note    ("move to next");
      ++n;
      break;
   case YDLST_CURR :
      DEBUG_DESK   yLOG_note    ("move to current");
      break;
   case YDLST_PREV :
      DEBUG_DESK   yLOG_note    ("move to previous");
      --n;
      break;
   case YDLST_TAIL :
      DEBUG_DESK   yLOG_note    ("move to last");
      n = s_nwin - 1;
      break;
   default  :
      DEBUG_DESK   yLOG_note    ("move not understood");
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(range checking)-----------------*/
   DEBUG_DESK   yLOG_value   ("requested" , n);
   --rce;  if (n < 0) {
      DEBUG_DESK   yLOG_note    ("before start, updated to head");
      n = 0;
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (n >= s_nwin) {
      DEBUG_DESK   yLOG_note    ("before after end, updated to tail");
      n = s_nwin - 1;
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save-back)----------------------*/
   if (r_id    != NULL)  *r_id   = s_wins [n].id;
   if (r_desk  != NULL)  *r_desk = s_wins [n].desk;
   if (r_title != NULL)  strlcpy (r_title, s_wins [n].title, LEN_FULL);
   if (r_type  != NULL)  *r_type = s_wins [n].type;
   if (r_curr  != NULL)  *r_curr = s_wins [n].curr;
   if (r_left  != NULL)  *r_left = s_wins [n].left;
   if (r_topp  != NULL)  *r_topp = s_wins [n].topp;
   if (r_scrn  != NULL)  *r_scrn = s_wins [n].scrn;
   if (r_locn  != NULL)  *r_locn = s_wins [n].locn;
   if (r_wide  != NULL)  *r_wide = s_wins [n].wide;
   if (r_tall  != NULL)  *r_tall = s_wins [n].tall;
   if (r_size  != NULL)  *r_size = s_wins [n].size;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return n;
}

char
yx11_win__verify        (long a_win)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        n           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   yx11_full_refresh  ('y');
   DEBUG_DESK   yLOG_value   ("s_nwin"    , s_nwin);
   /*---(search)-------------------------*/
   for (i = 0; i < s_nwin; ++i) {
      /*---(filter)----------------------*/
      DEBUG_DESK   yLOG_complex ("checking"  , "%d %10d %2d %s", i, s_wins [i].id, s_wins [i].desk, s_wins  [i].title);
      if (s_wins [i].id != a_win)  continue;
      /*---(found)-----------------------*/
      DEBUG_DESK   yLOG_note    ("FOUND");
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return i;
   }
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

char
yX11_win_verify         (long a_win)
{
   char rc = yx11_win__verify (a_win);
   if (rc < 0)  return 0;
   return 1;
}

char
yX11_win_where          (long a_win, char *d, int *x, int *y, int *w, int *t)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
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
   /*---(find window)--------------------*/
   n = yx11_win__verify (a_win);
   --rce;  if (n < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(save back)-------------------*/
   if (d != NULL)  *d = s_wins [n].desk;
   if (x != NULL)  *x = s_wins [n].left;
   if (y != NULL)  *y = s_wins [n].topp;
   if (w != NULL)  *w = s_wins [n].wide;
   if (t != NULL)  *t = s_wins [n].tall;
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return n;
}

long
yx11_win__regex         (char *a_regex, char *r_name, int *r_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        n           =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_complex ("args"      , "%p, %p, %p", a_regex, r_name, r_count);
   /*---(prepare)------------------------*/
   if (r_name  != NULL)  strlcpy (r_name, "", LEN_FULL);
   if (r_count != NULL)  *r_count = 0;
   /*---(defense)------------------------*/
   --rce;  if (a_regex == NULL || strlen (a_regex) == 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   yx11_full_refresh ('y');
   /*---(compile)------------------------*/
   rc = yREGEX_comp (a_regex);
   DEBUG_RPTG   yLOG_value   ("comp"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check all)----------------------*/
   --rce;  for (i = 0; i < s_nwin; ++i) {
      /*---(filter)----------------------*/
      DEBUG_DESK   yLOG_complex ("checking"  , "%d %10d %2d %s", i, s_wins [i].id, s_wins [i].desk, s_wins  [i].title);
      rc = yREGEX_filter (s_wins [i].title);
      DEBUG_INPT   yLOG_value   ("exec"      , rc);
      if (rc <= 0)    continue;
      /*---(found)-----------------------*/
      DEBUG_DESK   yLOG_note    ("FOUND");
      ++c;
      if (c == 1) {
         if (r_name != NULL)  strlcpy (r_name, s_wins [n].title, LEN_FULL);
         n = i;
      }
   }
   /*---(trouble)------------------------*/
   if (c <= 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (r_count != NULL)  *r_count = c;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return s_wins [n].id;
}

char
yx11_win__activate      (long a_win, char a_desk)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   char        x_pre       [LEN_HUND]  = "";
   char        x_suf       [LEN_HUND]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*> XRaiseWindow (YX_DISP, a_win);                                                 <*/
   /*---(prepare)------------------------*/
   sprintf (x_pre, "wmctrl -i -R %ld", a_win);
   sprintf (x_suf, "> /dev/null 2> /dev/null");
   /*---(set desktop)--------------------*/
   sprintf (x_cmd, "%s %s", x_pre, x_suf);
   DEBUG_DESK   yLOG_info    ("command"   , x_cmd);
   rc = system  (x_cmd);
   DEBUG_DESK   yLOG_value   ("system"    , rc);
   /*---(sticky)-------------------------*/
   if (a_desk == -1) {
      sprintf (x_suf, "-b add,sticky > /dev/null 2> /dev/null");
      sprintf (x_cmd, "%s %s", x_pre, x_suf);
      DEBUG_DESK   yLOG_info    ("command"   , x_cmd);
      rc = system  (x_cmd);
      DEBUG_DESK   yLOG_value   ("system"    , rc);
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_win_bring          (long a_win)
{
   char d = yX11_desk_current (NULL);
   yx11_loc_exact     (a_win, d, -1, -1, -1, -1);
   yx11_win__activate (a_win, d);
   return 0;
}

char
yX11_win_send           (long a_win, char a_desk)
{
   yx11_loc_exact (a_win, a_desk, -1, -1, -1, -1);
   return 0;
}

char
yX11_win_take           (long a_win, char a_desk)
{
   yX11_desk_goto     (a_desk);
   yx11_loc_exact     (a_win, a_desk, -1, -1, -1, -1);
   yx11_win__activate (a_win, a_desk);
   return 0;
}

char
yX11_win_goto           (long a_win)
{
   char        d           =   -1;
   yX11_win_where     (a_win, &d, NULL, NULL, NULL, NULL);
   if (d != -1)  yX11_desk_goto     (d);
   yx11_win__activate (a_win, d);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     terminal control                         ----===*/
/*====================------------------------------------====================*/
static void      o___TERMS___________________o (void) {;}

#define   MAX_TINTS       50
static struct {
   char        abbr;
   char        grp;
   char       *terse;
   char       *tint;
} s_tints [MAX_TINTS] = {
   /*---(clears)-------------------------*/
   { 'c', 'c', "clear"          , "clear"          },
   /*---(browns)-------------------------*/
   { 'e', 'w', "peru"           , "peru"           },
   { 'n', 'w', "sienna"         , "sienna"         },
   { 's', 'w', "salmon"         , "salmon"         },
   { 'x', 'w', "chocolate"      , "chocolate"      },
   { 'w', 'w', "brown"          , "brown"          },
   /*---(greens)-------------------------*/
   { 'g', 'g', "green"          , "green"          },
   { 'f', 'g', "forest"         , "forestgreen"    },
   { 'l', 'g', "lawn"           , "lawngreen"      },
   /*---(yellows)------------------------*/
   { 'y', 'y', "yellow"         , "yellow"         },
   { 'd', 'y', "golden"         , "goldenrod"      },
   { 'z', 'y', "orange"         , "darkorange"     },
   /*---(blues)--------------------------*/
   { 'y', 'b', "blue"           , "blue"           },
   { 'a', 'b', "cyan"           , "cyan"           },
   { 'q', 'b', "turquoise"      , "mediumturquoise"},
   { 'u', 'b', "steel"          , "steelblue"      },
   /*---(reds)---------------------------*/
   { 'r', 'r', "red"            , "red"            },
   { 't', 'r', "tomato"         , "tomato"         },
   /*---(reds)---------------------------*/
   { 'm', 'p', "magenta"        , "magenta"        },
   { 'v', 'p', "voilet"         , "voilet"         },
   { 'p', 'p', "purple"         , "purple4"        },
   /*---(non-trans)----------------------*/
   { 'k', 'n', "blk/grn"        , "green"          },
   { 'h', 'n', "blk/yel"        , "goldenrod"      },
   { 'i', 'n', "blk/red"        , "red"            },
   { 'j', 'n', "blk/pur"        , "purple"         },
   /*---(done)---------------------------*/
   {  0 ,  0 , NULL             , NULL             },
};

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
   int         x_len       =    0;
   int         i           =    0;
   int         n           =   -1;
   char        nd          =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   /*> rc = yx11_base_defense ();                                                     <* 
    *> DEBUG_DESK   yLOG_value   ("defense"   , rc);                                  <* 
    *> --rce;  if (rc < 0) {                                                          <* 
    *>    DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);                              <* 
    *>    return rce;                                                                 <* 
    *> }                                                                              <*/
   /*---(defense)------------------------*/
   nd = yx11_yvikeys__desktop   (a_desk);
   DEBUG_DESK   yLOG_value   ("nd"        , nd);
   --rce;  if (nd < -1) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(defense)------------------------*/
   x_len = strlen (a_color);
   --rce;  if (x_len == 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   DEBUG_DESK   yLOG_info    ("a_color"   , a_color);
   for (i = 0; i < MAX_TINTS; ++i) {
      if (s_tints [i].abbr == 0)                                      break;
      if (x_len == 1)  if (s_tints [i].abbr != a_color [0])           continue;
      if (x_len >  1)  if (strcmp (s_tints [i].terse, a_color) != 0)  continue;
      n = i;
      break;
   }
   DEBUG_DESK   yLOG_value   ("n"         , n);
   --rce;  if (n < 0) {
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
   yx11_full_refresh ('y');
   c = s_nwin;
   /*---(command)------------------------*/
   if (s_tints [n].grp == 'n')  sprintf (x_pre, "Eterm --tint darkslategrey -f %s --trans --shade 75", s_tints [n].tint);
   else                         sprintf (x_pre, "Eterm --tint %-10.10s --trans", s_tints [n].tint);
   sprintf (x_loc, "--desktop %d --geometry %dx%d+%d+%d", nd, w / 6 - 2, t / 10 - 2, x, y);
   sprintf (x_suf, "--buttonbar 0 --scrollbar 0 > /dev/null 2> /dev/null &");
   sprintf (x_cmd, "%s %s %s", x_pre, x_loc, x_suf);
   DEBUG_DESK   yLOG_info    ("command"   , x_cmd);
   rc = system  (x_cmd);
   DEBUG_DESK   yLOG_value   ("system"    , rc);
   /*---(sleep)--------------------------*/
   while (c == s_nwin && x_tries < 10) {
      usleep (100000);  /* appears to have a update/caching delay */
      yx11_full_refresh ('y');
      ++x_tries;
   }
   --rce;  if (c == s_nwin) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   x_id = s_wins [s_nwin - 1].id;
   DEBUG_DESK   yLOG_value   ("x_id"      , x_id);
   /*---(place)--------------------------*/
   yX11_win_exact (x_id, nd, x, y, w, t);
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return x_id;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
yX11__unit_win          (char *a_question, long a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rc          =    0; 
   char        s           [LEN_RECD]  = "";
   int         n           =    0;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "WIN unit         : unknown request", LEN_RECD);
   yx11_full_refresh ('y');
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "count"     , 20)  == 0) {
      snprintf (unit_answer, LEN_RECD, "WIN  count       : %d", s_nwin);
   }
   else if (strncmp (a_question, "entry"     , 20)  == 0) {
      if (a_num >= 0 && a_num < s_nwin) {
         sprintf (s, "%2d[%-.20s]", strlen (s_wins [a_num].title), s_wins [a_num].title);
         snprintf (unit_answer, LEN_RECD, "WIN  entry   (%2d): %-10d %c %2d %-24.24s %4dx %4dy %4dw %4dt", a_num, s_wins [a_num].id, s_wins [a_num].curr, s_wins [a_num].desk, s, s_wins [a_num].left, s_wins [a_num].topp, s_wins [a_num].wide, s_wins [a_num].tall);
      } else {
         snprintf (unit_answer, LEN_RECD, "WIN  entry   (--): -          -  -  -[]                        -x    -y    -w    -t");
      }
   }
   else if (strncmp (a_question, "where"     , 20)  == 0) {
      n = yx11_win__verify (a_num);
      if (n >= 0) {
         sprintf (s, "%2d[%-.20s]", strlen (s_wins [n].title), s_wins [n].title);
         snprintf (unit_answer, LEN_RECD, "WIN  where       : %-10d %c %2d %-24.24s %4dx %4dy %4dw %4dt", s_wins [n].id, s_wins [n].curr, s_wins [n].desk, s, s_wins [n].left, s_wins [n].topp, s_wins [n].wide, s_wins [n].tall);
      } else {
         snprintf (unit_answer, LEN_RECD, "WIN  where       : -          -  -  -[]                        -x    -y    -w    -t");
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}



