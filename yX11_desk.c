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
static int   s_ndesk      = 0;

#define    MAX_WINS   100
typedef struct cWINS tWINS;
static struct cWINS {
   int           id;
   char          desk;
   char          title     [LEN_DESC];
};
static tWINS s_wins  [MAX_WINS];
static int   s_nwin       = 0;



char yX11_desktop_prev      (void)  { return system("fluxbox-remote \"PrevWorkspace\""); }
char yX11_desktop_next      (void)  { return system("fluxbox-remote \"NextWorkspace\""); }

char yX11_desktop_prev_with (void)  { return system("fluxbox-remote \"TakeToPrevWorkspace\""); }
char yX11_desktop_next_with (void)  { return system("fluxbox-remote \"TakeToNextWorkspace\""); }

char yX11_window_prev       (void)  { return system("fluxbox-remote \"PrevWindow\""); }
char yX11_window_next       (void)  { return system("fluxbox-remote \"NextWindow\""); }

#define    MAX_POS      60
const static struct {
   char        size;
   char        abbr;
   char        scrn;
   char        locn;
   char        desc      [50];
   short       wide;
   short       tall;
   short       topp;
   short       left;
} s_locs [MAX_POS] = {
   /*---(normal editing)-----------------*/
   { 'n', 'a', 't', 'a', "editor, top, far left"           ,  529,  733,   10,   10 },
   { 'n', 'r', 't', 'b', "editor, top, mid left"           ,  529,  733,   17,  280 },
   { 'n', 's', 't', 'c', "editor, top, mid right"          ,  529,  733,   24,  550 },
   { 'n', 't', 't', 'd', "editor, top, far right"          ,  529,  733,   30,  825 },
   { 'n', 'a', 'b', 'a', "editor, bottom, far left"        ,  529,  733,  778,   10 },
   { 'n', 'r', 'b', 'b', "editor, bottom, mid left"        ,  529,  733,  783,  280 },
   { 'n', 's', 'b', 'c', "editor, bottom, mid right"       ,  529,  733,  788,  550 },
   { 'n', 't', 'b', 'd', "editor, bottom, far right"       ,  529,  733,  798,  825 },
   /*---(programming)--------------------*/
   { 'p', 'A', 't', 'a', "programmer, top, far left"       ,  655,  733,   10,   10 },
   { 'p', 'R', 't', 'b', "programmer, top, mid left"       ,  655,  733,   17,  240 },
   { 'p', 'S', 't', 'c', "programmer, top, mid right"      ,  655,  733,   24,  470 },
   { 'p', 'T', 't', 'd', "programmer, top, far right"      ,  655,  733,   30,  700 },
   { 'p', 'A', 'b', 'a', "programmer, bottom, far left"    ,  655,  733,  778,   10 },
   { 'p', 'R', 'b', 'b', "programmer, bottom, mid left"    ,  655,  733,  783,  240 },
   { 'p', 'S', 'b', 'c', "programmer, bottom, mid right"   ,  655,  733,  788,  470 },
   { 'p', 'T', 'b', 'd', "programmer, bottom, far right"   ,  655,  733,  798,  700 },
   /*---(eighths)------------------------*/
   { 'e', 'q', 't', '1', "eighth, top, inner north-west"   ,  529,  383,  100,  150 },
   { 'e', 'w', 't', '2', "eighth, top, inner south-west"   ,  529,  383,  300,  150 },
   { 'e', 'f', 't', '3', "eighth, top, inner north-east"   ,  529,  383,  100,  675 },
   { 'e', 'p', 't', '4', "eighth, top, inner south-east"   ,  529,  383,  300,  675 },
   { 'e', 'Q', 't', '5', "eighth, top, outer north-west"   ,  529,  383,   10,   10 },
   { 'e', 'W', 't', '6', "eighth, top, outer south-west"   ,  529,  383,  380,   10 },
   { 'e', 'F', 't', '7', "eighth, top, outer north-east"   ,  529,  383,   10,  830 },
   { 'e', 'P', 't', '8', "eighth, top, outer south-east"   ,  529,  383,  380,  830 },
   { 'e', 'q', 'b', '1', "eighth, bottom, inner north-west",  529,  383,  868,  150 },
   { 'e', 'w', 'b', '2', "eighth, bottom, inner south-west",  529,  383, 1068,  150 },
   { 'e', 'f', 'b', '3', "eighth, bottom, inner north-east",  529,  383,  868,  675 },
   { 'e', 'p', 'b', '4', "eighth, bottom, inner south-east",  529,  383, 1068,  675 },
   { 'e', 'Q', 'b', '5', "eighth, bottom, outer north-west",  529,  383,  778,   10 },
   { 'e', 'W', 'b', '6', "eighth, bottom, outer south-west",  529,  383, 1148,   10 },
   { 'e', 'F', 'b', '7', "eighth, bottom, outer north-east",  529,  383,  778,  830 },
   { 'e', 'P', 'b', '8', "eighth, bottom, outer south-east",  529,  383, 1148,  830 },
   /*---(quarters)-----------------------*/
   { 'q', 'z', 't', '1', "quarter, top, north-west"        ,  850,  643,   10,   10 },
   { 'q', 'x', 't', '2', "quarter, top, south-west"        ,  850,  643,  115,   10 },
   { 'q', 'c', 't', '3', "quarter, top, north-east"        ,  850,  643,   10,  550 },
   { 'q', 'v', 't', '4', "quarter, top, south-east"        ,  850,  643,  115,  550 },
   { 'q', 'z', 'b', '1', "quarter, bottom, north-west"     ,  850,  643,  778,   10 },
   { 'q', 'x', 'b', '2', "quarter, bottom, south-west"     ,  850,  643,  883,   10 },
   { 'q', 'c', 'b', '3', "quarter, bottom, north-east"     ,  850,  643,  778,  550 },
   { 'q', 'v', 'b', '4', "quarter, bottom, south-east"     ,  850,  643,  883,  550 },
   /*---(horizontals)--------------------*/
   { 'h', 'Z', 't', 't', "horizontal, top, upper"          , 1200,  444,   10,   40 },
   { 'h', 'X', 't', 'm', "horizontal, top, middle"         , 1200,  444,  160,   40 },
   { 'h', 'C', 't', 'b', "horizontal, top, lower"          , 1200,  444,  310,   40 },
   { 'h', 'Z', 'b', 't', "horizontal, bottom, upper"       , 1200,  444,  778,   40 },
   { 'h', 'X', 'b', 'm', "horizontal, bottom, middle"      , 1200,  444,  928,   40 },
   { 'h', 'C', 'b', 'b', "horizontal, bottom, lower"       , 1200,  444, 1078,   40 },
   /*---(fulls)--------------------------*/
   { 'f', 'V', 't', '-', "full screen, top"                , 1366,  768,    0,    0 },
   { 'f', 'V', 'b', '-', "full screen, bottom"             , 1366,  768,  768,    0 },
   /*---(done)---------------------------*/
   {  0 ,  0 ,  0 ,  0 , "end-of-list"                     ,    0,    0,    0,    0 },
};

char
yX11_desk__purge        (void)
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
yX11_desk__win_purge    (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_nwin = 0;
   for (i = 0; i < MAX_DESK; ++i) {
      s_wins [i].id        = -1;
      s_wins [i].desk      = -1;
      s_wins [i].title [0] = '\0';
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yX11_desk__desktops     (char a_real)
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
   yX11_desk__purge ();
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
      s_desks [s_ndesk].id   = atoi (p);
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

char
yX11_desk__windows      (char a_real)
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
   yX11_desk__win_purge ();
   /*---(generate data)------------------*/
   if (a_real == 'y')  rc = system ("wmctrl -l > /tmp/yX11_windows.txt");
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
      p = strtok_r (NULL  , q, &r);
      s_wins [s_nwin].desk = atoi (p);
      p = strtok_r (NULL  , q, &r);
      x_len = strlen (p);
      p += x_len + 1;
      strlcpy (s_wins [s_nwin].title, p, LEN_DESC);
      DEBUG_DESK   yLOG_complex ("result"    , "%d %10.10x %2d %s", s_nwin, s_wins [s_nwin].id, s_wins [s_nwin].desk, s_wins  [s_nwin].title);
      if (s_desks [s_wins [s_nwin].desk].id >= 0)  ++s_desks [s_wins [s_nwin].desk].wins;
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
yX11_desktop_moves_NEW  (char a_move)
{
   char        rce         =  -10;
   switch (a_move) {
   case '[' :
      return system ("fluxbox-remote \"NextWorkspace\"");
      break;
   case '>' :  case 'k' :
      return system ("fluxbox-remote \"NextWorkspace\"");
      break;
   case 'K' :  return system("fluxbox-remote \"TakeToNextWorkspace\"");   break;
   case 'b' :  return system("fluxbox-remote \"PrevWorkspace\"");         break;
   case 'B' :  return system("fluxbox-remote \"TakeToPrevWorkspace\"");   break;
   }
}

char
yX11_desktop_moves      (char a_move)
{
   char        rce         =  -10;
   switch (a_move) {
   case 'k' :  return system("fluxbox-remote \"NextWorkspace\"");         break;
   case 'K' :  return system("fluxbox-remote \"TakeToNextWorkspace\"");   break;
   case 'b' :  return system("fluxbox-remote \"PrevWorkspace\"");         break;
   case 'B' :  return system("fluxbox-remote \"TakeToPrevWorkspace\"");   break;
   case 'h' :  return system("fluxbox-remote \"NextWindow\"");            break;
   case 'd' :  return system("fluxbox-remote \"PrevWindow\"");            break;
   default  :  return rce;
   }
   return 0;
}

char
yX11_position_by_abbr   (char a_abbr, char a_scrn)
{
   char        rce         =  -10;
   char        rc          =    0;
   char        i           =    0;
   char        n           =   -1;
   char        t           [100] = "";
   for (i = 0; i < MAX_POS; ++i) {
      if (s_locs [i].size   == 0)          break;
      if (s_locs [i].abbr   == a_abbr)     continue;
      if (s_locs [i].scrn   == a_scrn)     continue;
   }
   /*---(catch trouble)------------------*/
   if (n < 0)  return rce;
   /*---(resize)-------------------------*/
   sprintf (t, "fluxbox-remote \"ResizeTo %4d %4d\"", s_locs [n].wide, s_locs [n].tall);
   rc = system  (t);
   /*---(move)---------------------------*/
   sprintf (t, "fluxbox-remote \"MoveTo   %4d %4d\"", s_locs [n].left, s_locs [n].topp);
   rc = system  (t);
   /*---(complete)-----------------------*/
   return 0;
}

char
yX11_position_by_locn   (char a_size, char a_scrn, char a_locn)
{
   char        rce         =  -10;
   char        rc          =    0;
   char        i           =    0;
   char        n           =   -1;
   char        t           [100] = "";
   for (i = 0; i < MAX_POS; ++i) {
      if (s_locs [i].size   == 0)          break;
      if (s_locs [i].size   == a_size)     continue;
      if (s_locs [i].scrn   == a_scrn)     continue;
      if (s_locs [i].locn   == a_locn)     continue;
   }
   /*---(catch trouble)------------------*/
   if (n < 0)  return rce;
   /*---(resize)-------------------------*/
   sprintf (t, "fluxbox-remote \"ResizeTo %4d %4d\"", s_locs [n].wide, s_locs [n].tall);
   rc = system  (t);
   /*---(move)---------------------------*/
   sprintf (t, "fluxbox-remote \"MoveTo   %4d %4d\"", s_locs [n].left, s_locs [n].topp);
   rc = system  (t);
   /*---(complete)-----------------------*/
   return 0;
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
   strlcpy (unit_answer, "yX11_unit, unknown request", 100);
   /*---(string testing)-----------------*/
   if      (strncmp (a_question, "d_count"   , 20)  == 0) {
      strlcpy (t, "", LEN_RECD);
      for (i = 0; i < MAX_DESK; ++i) {
         if (i < s_ndesk)  sprintf (s, "%2d", s_desks [i].id);
         else              sprintf (s, " -");
         if (i > 0)  strlcat (t, " ", LEN_RECD);
         strlcat (t, s  , LEN_RECD);
      }
      snprintf (unit_answer, LEN_RECD, "yX11 d_count     : %2d %s", s_ndesk, t);
   }
   else if (strncmp (a_question, "d_entry"   , 20)  == 0) {
      if (a_num >= 0 && a_num < s_ndesk)
         snprintf (unit_answer, LEN_RECD, "yX11 d_entry (%1d) : %2d  %c  %-10.10s  %4dw  %4dt  %4d", a_num, s_desks [a_num].id, s_desks [a_num].curr, s_desks [a_num].label, s_desks [a_num].wide, s_desks [a_num].tall, s_desks [a_num].wins);
      else
         snprintf (unit_answer, LEN_RECD, "yX11 d_entry (-) :  -  -  -              -w     -t     -");
   }
   else if (strncmp (a_question, "w_count"   , 20)  == 0) {
      snprintf (unit_answer, LEN_RECD, "yX11 w_count     : %d", s_nwin);
   }
   else if (strncmp (a_question, "w_entry"   , 20)  == 0) {
      if (a_num >= 0 && a_num < s_nwin)
         snprintf (unit_answer, LEN_RECD, "yX11 w_entry (%2d): %-10d  %2d  %-.30s", a_num, s_wins [a_num].id, s_wins [a_num].desk, s_wins [a_num].title);
      else
         snprintf (unit_answer, LEN_RECD, "yX11 w_entry (--):  - - - - -   -  -");
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}


