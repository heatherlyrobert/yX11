#include "yX11.h"
#include "yX11_priv.h"



#define    MAX_DESK    10
typedef struct cDESK tDESK;
static struct cDESK {
   char          id;
   char          curr;
   char          label     [LEN_LABEL];
   short         wide;
   short         tall;
   char          wins;
};
static tDESK s_desks [MAX_DESK];
int          s_ndesk      = 0;  /* public to assist unit testing    */



char yX11_desktop_prev      (void)  { return system("fluxbox-remote \"PrevWorkspace\""); }
char yX11_desktop_next      (void)  { return system("fluxbox-remote \"NextWorkspace\""); }

char yX11_desktop_prev_with (void)  { return system("fluxbox-remote \"TakeToPrevWorkspace\""); }
char yX11_desktop_next_with (void)  { return system("fluxbox-remote \"TakeToNextWorkspace\""); }

char yX11_window_prev       (void)  { return system("fluxbox-remote \"PrevWindow\""); }
char yX11_window_next       (void)  { return system("fluxbox-remote \"NextWindow\""); }



/*====================------------------------------------====================*/
/*===----                     counting assistance                      ----===*/
/*====================------------------------------------====================*/
static void      o___COUNTERS________________o (void) {;}

char
yx11_desk_addwin        (char a_desk)
{
   if (a_desk < 0 || a_desk >= s_ndesk)  return 0;
   ++s_desks [a_desk].wins;
   return 0;
}

char
yx11_desk_subwin        (char a_desk)
{
   if (a_desk < 0 || a_desk >= s_ndesk)  return 0;
   --s_desks [a_desk].wins;
   return 0;
}



/*====================------------------------------------====================*/
/*===----                     program level                            ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}


char
yx11_desk_purge         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_ndesk = 0;
   for (i = 0; i < MAX_DESK; ++i) {
      s_desks [i].id        = -1;
      s_desks [i].curr      = '-';
      s_desks [i].label [0] = '\0';
      s_desks [i].wide      = -1;
      s_desks [i].tall      = -1;
      s_desks [i].wins      =  0;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yx11_desk_init       (void)
{
   yx11_desk_purge  ();
   return 0;
}



/*====================------------------------------------====================*/
/*===----                    inventorying desk/win                     ----===*/
/*====================------------------------------------====================*/
static void      o___INVENTORY_______________o (void) {;}

char
yx11_desk_inventory     (char a_real)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   FILE       *f           = NULL;
   char        x_recd      [LEN_RECD]  = "";
   char        t           [LEN_LABEL] = "";
   int         i           =    0;
   int         x_len       =    0;
   char       *p           = NULL;
   char       *q           = " ";
   char       *r           = NULL;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(initialize)---------------------*/
   yx11_desk_purge ();
   /*---(generate data)------------------*/
   if (a_real == 'y')  rc = system ("wmctrl -d > /tmp/yX11_desktops.txt");
   /*---(open)---------------------------*/
   f = fopen ("/tmp/yX11_desktops.txt", "rt");
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
      DEBUG_DESK   yLOG_complex ("x_recd"    , "%d) %2d[%s]", s_ndesk, x_len, x_recd);
      if (x_len < 25)  continue;
      p = strtok_r (x_recd, q, &r);
      s_desks [s_ndesk].id   = atol (p);
      p = strtok_r (NULL  , q, &r);
      s_desks [s_ndesk].curr = p [0];
      p = strtok_r (NULL  , q, &r);
      p = strtok_r (NULL  , q, &r);
      p = strtok_r (NULL  , q, &r);
      p = strtok_r (NULL  , q, &r);
      p = strtok_r (NULL  , q, &r);
      p = strtok_r (NULL  , q, &r);
      p = strtok_r (NULL  , "x", &r);
      s_desks [s_ndesk].wide = atoi (p);
      p = strtok_r (NULL  , q, &r);
      s_desks [s_ndesk].tall = atoi (p);
      x_len = strlen (p);
      p += x_len + 1;
      strltrim (p, ySTR_BOTH, LEN_DESC);
      strlcpy (s_desks [s_ndesk].label, p, LEN_LABEL);
      DEBUG_DESK   yLOG_complex ("result"    , "%d %c %-10.10s %4d %4d", s_desks [s_ndesk].id, s_desks [s_ndesk].curr, s_desks [s_ndesk].label, s_desks [s_ndesk].wide, s_desks [s_ndesk].tall);
      ++s_ndesk;
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



/*====================------------------------------------====================*/
/*===----                       handling desktops                      ----===*/
/*====================------------------------------------====================*/
static void      o___DESKTOPS________________o (void) {;}

char
yx11_full_refresh       (char a_real)
{
   yx11_desk_inventory ('y');
   yx11_win_inventory  ('y');
   yx11_win_current    ('y');
   return 0;
}

char
yX11_desktop_current    (char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        n           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   if (a_name != NULL)  strlcpy (a_name, "", LEN_LABEL);
   yx11_full_refresh  ('y');
   DEBUG_DESK   yLOG_value   ("s_ndesk"   , s_ndesk);
   /*---(search)-------------------------*/
   for (i = 0; i < s_ndesk; ++i) {
      /*---(filter)----------------------*/
      DEBUG_DESK   yLOG_complex ("checking"  , "%d %c %-10.10s %4d %4d", s_desks [i].id, s_desks [i].curr, s_desks [i].label, s_desks [i].wide, s_desks [i].tall);
      if (s_desks [i].curr != '*')  continue;
      /*---(found)-----------------------*/
      DEBUG_DESK   yLOG_note    ("FOUND");
      if (a_name != NULL)  strlcpy (a_name, s_desks [i].label, LEN_LABEL);
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return i;
   }
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

char
yx11_desktop__cursor    (char a_action, char a_move, char *a_name)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   char        t           [LEN_HUND]  = "";
   char        x_orig      [LEN_LABEL] = "";
   char        o           =    0;
   static char n           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_complex ("args"      , "%c, %d, %p", a_action, a_move, a_name);
   /*---(prepare)------------------------*/
   if (a_name != NULL)  strlcpy (a_name, "", LEN_LABEL);
   o  = yX11_desktop_current (x_orig); /* never assume, update */
   if (a_action == 'y')  n = o;
   DEBUG_DESK   yLOG_value   ("current"   , n);
   /*---(defense)------------------------*/
   DEBUG_DESK   yLOG_value   ("s_ndesk"   , s_ndesk);
   --rce;  if (n < 0 || s_ndesk == 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(direct move)--------------------*/
   --rce;  if (a_move >= 0 && a_move <= 20) {
      DEBUG_DESK   yLOG_note    ("move by reference");
      n = a_move;
   }
   /*---(cursoring)----------------------*/
   else {
      switch (a_move) {
      case '[' :
         DEBUG_DESK   yLOG_note    ("move to first");
         n = 0;
         break;
      case '>' :  case 'k' :
         DEBUG_DESK   yLOG_note    ("move to next");
         ++n;
         break;
      case '.' :
         DEBUG_DESK   yLOG_note    ("move to current");
         break;
      case '<' :  case 'b' :
         DEBUG_DESK   yLOG_note    ("move to previous");
         --n;
         break;
      case ']' :
         DEBUG_DESK   yLOG_note    ("move to last");
         n = s_ndesk - 1;
         break;
      default  :
         DEBUG_DESK   yLOG_note    ("move not understood");
         DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
         return rce;
      }
   }
   /*---(range checking)-----------------*/
   DEBUG_DESK   yLOG_value   ("requested" , n);
   --rce;  if (n < 0) {
      DEBUG_DESK   yLOG_note    ("before start, updated to head");
      n = 0;
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   --rce;  if (n >= s_ndesk) {
      DEBUG_DESK   yLOG_note    ("before after end, updated to tail");
      n = s_ndesk - 1;
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(change desktop)-----------------*/
   if (a_action == 'y') {
      /*---(shortcut for no move)--------*/
      if (n == o) {
         if (a_name != NULL)  strlcpy (a_name, x_orig, LEN_LABEL);
         DEBUG_DESK   yLOG_exit    (__FUNCTION__);
         return o;
      }
      /*---(handle changes)--------------*/
      sprintf (t, "wmctrl -i -s %d", n);
      DEBUG_DESK   yLOG_info    ("t"         , t);
      rc = system (t);
      DEBUG_DESK   yLOG_value   ("system"    , rc);
      sleep (1);  /* appears to have a update/caching delay */
      /*---(update current)-----------------*/
      n  = yX11_desktop_current (a_name);
      DEBUG_DESK   yLOG_value   ("current"   , n);
   } else {
      if (a_name != NULL)  strlcpy (a_name, s_desks [n].label, LEN_LABEL);
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return n;
}

char
yX11_desktop_cursor     (char a_move, char *a_name)
{
   return yx11_desktop__cursor ('-', a_move, a_name);
}

char
yX11_desktop_move       (char a_move, char *a_name)
{
   return yx11_desktop__cursor ('y', a_move, a_name);
}

char
yx11_desktop__regex     (char a_action, char *a_regex, char *a_name, int *a_count)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =    0;
   int         i           =    0;
   char        x_orig      [LEN_LABEL] = "";
   char        o           =    0;
   char        n           =    0;
   int         c           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_complex ("args"      , "%c, %p, %p, %p", a_action, a_regex, a_name, a_count);
   /*---(prepare)------------------------*/
   if (a_name  != NULL)  strlcpy (a_name, "", LEN_LABEL);
   if (a_count != NULL)  *a_count = 0;
   /*---(defense)------------------------*/
   --rce;  if (a_regex == NULL || strlen (a_regex) == 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(update)-------------------------*/
   n  = o  = yX11_desktop_current (x_orig); /* never assume, update */
   DEBUG_DESK   yLOG_value   ("current"   , n);
   /*---(compile)------------------------*/
   rc = yREGEX_comp (a_regex);
   DEBUG_RPTG   yLOG_value   ("comp"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_RPTG   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(check all)----------------------*/
   --rce;  for (i = 0; i < s_ndesk; ++i) {
      /*---(filter)----------------------*/
      DEBUG_DESK   yLOG_complex ("checking"  , "%d %c %-10.10s %4d %4d", s_desks [i].id, s_desks [i].curr, s_desks [i].label, s_desks [i].wide, s_desks [i].tall);
      rc = yREGEX_fast (s_desks [i].label);
      DEBUG_INPT   yLOG_value   ("exec"      , rc);
      if (rc <= 0)    continue;
      /*---(found)-----------------------*/
      DEBUG_DESK   yLOG_note    ("FOUND");
      ++c;
      if (c == 1) {
         if (a_action == 'y') {
            n = yX11_desktop_move (i, a_name);
         } else {
            if (a_name != NULL)  strlcpy (a_name, s_desks [n].label, LEN_LABEL);
            n = i;
         }
      }
   }
   /*---(trouble)------------------------*/
   if (c <= 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   if (a_count != NULL)  *a_count = c;
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return n;
}

char
yX11_desktop_activate   (char *a_regex, char *a_name, int *a_count)
{
   return yx11_desktop__regex ('y', a_regex, a_name, a_count);
}

char
yX11_desktop_exists         (char *a_regex)
{
   int         n           =    0;
   int         c           =    0;
   n = yx11_desktop__regex ('-', a_regex, NULL, &c);
   return c;
}


/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
yX11__unit_desk         (char *a_question, int a_num)
{
   /*---(locals)-----------+-----+-----+-*/
   char        s           [LEN_LABEL];
   char        t           [LEN_RECD ];
   int         i           =    0;
   /*---(initialize)---------------------*/
   strlcpy (unit_answer, "DESK unit        : unknown request", LEN_RECD);
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "count"   , 20)  == 0) {
      strlcpy (t, "", LEN_RECD);
      for (i = 0; i < MAX_DESK; ++i) {
         if (i < s_ndesk)  sprintf (s, "%2d", s_desks [i].id);
         else              sprintf (s, " -");
         if (i > 0)  strlcat (t, " ", LEN_RECD);
         strlcat (t, s  , LEN_RECD);
      }
      snprintf (unit_answer, LEN_RECD, "DESK count       : %2d %s", s_ndesk, t);
   }
   else if (strncmp (a_question, "entry"   , 20)  == 0) {
      if (a_num >= 0 && a_num < s_ndesk)
         snprintf (unit_answer, LEN_RECD, "DESK entry   (%1d) : %2d  %c  %-10.10s  %4dw  %4dt  %4d", a_num, s_desks [a_num].id, s_desks [a_num].curr, s_desks [a_num].label, s_desks [a_num].wide, s_desks [a_num].tall, s_desks [a_num].wins);
      else
         snprintf (unit_answer, LEN_RECD, "DESK entry   (-) :  -  -  -              -w     -t     -");
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


