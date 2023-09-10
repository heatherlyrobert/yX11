#include "yX11.h"
#include "yX11_priv.h"


/*> 123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456789-   <*/

#define   MAX_SIZES    10
const static struct {
   char        abbr;
   char        desc        [LEN_LABEL];
   short       wide;
   short       tall;
} s_sizes [MAX_SIZES] = {
   /*---(talls)-----------------------------*/
   { 'n', "editor"             ,  529,  733 },   /*  72 lines by 86 char wide          */
   { 'p', "programmer"         ,  655,  733 },   /*  tags plus  80 char editing space  */
   { 'm', "monster"            ,  895,  733 },   /*  tags plus 120 char editing space  */
   /*---(rectangles)------------------------*/
   { 'e', "eighth"             ,  529,  383 },
   { 'w', "worker"             ,  619,  443 },   /*  the gyges demo size               */
   { 'q', "quarter"            ,  805,  643 },
   /*---(wides)-----------------------------*/
   { 'h', "half"               , 1200,  444 },
   { 'f', "full"               , 1366,  768 },
   /*---(done)------------------------------*/
   {  0 , "---end---"          ,    0,    0 },
};


#define   MAX_LOCNS    45
const static struct {
   char        abbr;
   char        size;
   char        desc        [LEN_LABEL];
   short       left;
   short       topp;
} s_locns [MAX_LOCNS] = {
   /*---(tall editor)-----------------------*/
   { 'a', 'n', "far left"           ,   10,   10 },
   { 'b', 'n', "mid left"           ,  280,   17 },
   { 'd', 'n', "mid right"          ,  550,   24 },
   { 'e', 'n', "far right"          ,  825,   30 },
   /*---(tall programmer)-------------------*/
   { 'a', 'p', "far left"           ,   10,   10 },
   { 'b', 'p', "mid left"           ,  240,   17 },
   { 'd', 'p', "mid right"          ,  470,   24 },
   { 'e', 'p', "far right"          ,  700,   30 },
   /*---(tall monster)----------------------*/
   { 'a', 'm', "far left"           ,   10,   10 },
   { 'c', 'm', "middle"             ,  220,   20 },
   { 'e', 'm', "far right"          ,  440,   30 },
   /*---(inner eighths)---------------------*/
   { 'f', 'e', "inner north-west"   ,  150,  100 },
   { 'g', 'e', "inner south-west"   ,  150,  300 },
   { 'h', 'e', "inner north-east"   ,  675,  100 },
   { 'i', 'e', "inner south-east"   ,  675,  300 },
   /*---(outer eighths)---------------------*/
   { 'j', 'e', "outer north-west"   ,   10,   10 },
   { 'k', 'e', "outer south-west"   ,   10,  380 },
   { 'l', 'e', "outer north-east"   ,  830,   10 },
   { 'm', 'e', "outer south-east"   ,  830,  380 },
   /*---(worker)----------------------------*/
   { 'j', 'w', "north-west"         ,   70,  100 },
   { 'k', 'w', "south-west"         ,   70,  300 },
   { 'l', 'w', "north-east"         ,  675,  100 },
   { 'm', 'w', "south-east"         ,  675,  300 },
   /*---(quarters)--------------------------*/
   { 'j', 'q', "north-west"         ,   10,   10 },
   { 'k', 'q', "south-west"         ,   10,  115 },
   { 'l', 'q', "north-east"         ,  550,   10 },
   { 'm', 'q', "south-east"         ,  550,  115 },
   /*---(horizontals)-----------------------*/
   { 'w', 'h', "wide top"           ,   40,   10 },
   { 'x', 'h', "wide middle"        ,   40,  160 },
   { 'y', 'h', "wide bottom"        ,   40,  310 },
   /*---(full)------------------------------*/
   { 'z', 'f', "full screen"        ,    0,    0 },
   /*---(done)------------------------------*/
   {  0 , '-', "---end---"          ,    0,    0 },
};


#define    MAX_LOC      60
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
} s_locs [MAX_LOC] = {
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
   { 'q', 'z', 't', '1', "quarter, top, north-west"        ,  805,  643,   10,   10 },
   { 'q', 'x', 't', '2', "quarter, top, south-west"        ,  805,  643,  115,   10 },
   { 'q', 'c', 't', '3', "quarter, top, north-east"        ,  805,  643,   10,  550 },
   { 'q', 'v', 't', '4', "quarter, top, south-east"        ,  805,  643,  115,  550 },
   { 'q', 'z', 'b', '1', "quarter, bottom, north-west"     ,  805,  643,  778,   10 },
   { 'q', 'x', 'b', '2', "quarter, bottom, south-west"     ,  805,  643,  883,   10 },
   { 'q', 'c', 'b', '3', "quarter, bottom, north-east"     ,  805,  643,  778,  550 },
   { 'q', 'v', 'b', '4', "quarter, bottom, south-east"     ,  805,  643,  883,  550 },
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
int            s_nloc  = 0;


char
yx11_loc_by_size        (short a_wide, short a_tall, char *r_desc)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(default)------------------------*/
   if (r_desc != NULL)  ystrlcpy (r_desc, "", LEN_LABEL);
   /*---(walk structure)-----------------*/
   for (i = 0; i < MAX_SIZES; ++i) {
      /*---(end of list)-----------------*/
      if (s_sizes [i].abbr  == 0)           break;
      /*---(outside limits)--------------*/
      if (s_sizes [i].wide + 30 <  a_wide)  continue;
      if (s_sizes [i].wide - 30 >  a_wide)  continue;
      if (s_sizes [i].tall + 30 <  a_tall)  continue;
      if (s_sizes [i].tall - 30 >  a_tall)  continue;
      /*---(appoximate match)------------*/
      if (r_desc != NULL)  ystrlcpy (r_desc, s_sizes [i].desc, LEN_LABEL);
      if (s_sizes [i].wide != a_wide)       return s_sizes [i].abbr;
      if (s_sizes [i].tall != a_tall)       return s_sizes [i].abbr;
      /*---(exact match)-----------------*/
      return toupper (s_sizes [i].abbr);
   }
   /*---(complete)-----------------------*/
   return '·';
}

char
yx11_loc_by_locn        (char a_size, short a_left, short a_topp, char *r_desc, char *r_scrn)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   char        x_size      =  '·';
   char        x_abbr      =  '·';
   char        x_scrn      =  '·';
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(default)------------------------*/
   if (r_desc != NULL)  ystrlcpy (r_desc, "", LEN_LABEL);
   if (r_scrn != NULL)  *r_scrn = '·';
   /*---(prepare)------------------------*/
   x_size = tolower (a_size);
   DEBUG_DESK   yLOG_char    ("x_size"    , x_size);
   if (a_topp <  768) {
      x_scrn = 't';
   } else {
      x_scrn = 'b';
      a_topp -= 768;
   }
   if (r_scrn != NULL)  *r_scrn = x_scrn;
   DEBUG_DESK   yLOG_char    ("x_scrn"    , x_scrn);
   DEBUG_DESK   yLOG_value   ("a_left"    , a_left);
   DEBUG_DESK   yLOG_value   ("a_topp"    , a_topp);
   /*---(walk structure)-----------------*/
   for (i = 0; i < MAX_LOCNS; ++i) {
      /*---(end of list)-----------------*/
      if (s_locns [i].abbr  == 0)      break;
      DEBUG_DESK   yLOG_complex ("check"     , "%2d, %c %c %4d %4d", i, s_locns [i].abbr, s_locns [i].size, s_locns [i].left, s_locns [i].topp);
      /*---(right size)------------------*/
      if (s_locns [i].size  != x_size)      continue;
      /*---(outside limits)--------------*/
      if (s_locns [i].left + 30 <  a_left)  continue;
      if (s_locns [i].left - 30 >  a_left)  continue;
      if (s_locns [i].topp + 30 <  a_topp)  continue;
      if (s_locns [i].topp - 30 >  a_topp)  continue;
      DEBUG_DESK   yLOG_note    ("found at least an approximate match");
      /*---(appoximate match)------------*/
      if      (s_locns [i].left != a_left)  x_abbr = s_locns [i].abbr;
      else if (s_locns [i].topp != a_topp)  x_abbr = s_locns [i].abbr;
      else                                  x_abbr = toupper (s_locns [i].abbr);
      /*---(save back)-------------------*/
      if (r_desc != NULL)  ystrlcpy (r_desc, s_locns [i].desc, LEN_LABEL);
      /*---(done)------------------------*/
      break;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return x_abbr;
}

/*====================------------------------------------====================*/
/*===----                       program level                          ----===*/
/*====================------------------------------------====================*/
static void      o___PROGRAM_________________o (void) {;}

char
yx11_loc_init           (void)
{
   /*---(locals)-----------+-----+-----+-*/
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_senter  (__FUNCTION__);
   /*---(initialize)---------------------*/
   s_nloc = 0;
   for (i = 0; i < MAX_LOC; ++i) {
      if (s_locs [i].abbr == 0)  break;
      ++s_nloc;
   }
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_sexit   (__FUNCTION__);
   return 0;
}



/*====================------------------------------------====================*/
/*===----                       handling windows                       ----===*/
/*====================------------------------------------====================*/
static void      o___WINDOWS_________________o (void) {;}

char
yX11_position_by_abbr   (char a_abbr, char a_scrn)
{
   char        rce         =  -10;
   char        rc          =    0;
   char        i           =    0;
   char        n           =   -1;
   char        t           [100] = "";
   for (i = 0; i < MAX_LOC; ++i) {
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
   for (i = 0; i < MAX_LOC; ++i) {
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
/*===----                      basic launching                         ----===*/
/*====================------------------------------------====================*/
static void      o___LAUNCH__________________o (void) {;}

char
yx11_loc_sizing         (char a_abbr, char a_scrn, int *a_left, int *a_topp, int *a_wide, int *a_tall)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         i           =    0;
   char        n           =    0;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(defaults)-----------------------*/
   if (a_left != NULL)  *a_left = 0;
   if (a_topp != NULL)  *a_topp = 0;
   if (a_wide != NULL)  *a_wide = 0;
   if (a_tall != NULL)  *a_tall = 0;
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(search)-------------------------*/
   for (i = 0; i < s_nloc; ++i) {
      /*---(filter)----------------------*/
      DEBUG_DESK   yLOG_complex ("checking"  , "%2d %c %c %c %c %s", i, s_locs [i].size, s_locs [i].abbr, s_locs [i].locn, s_locs [i].scrn, s_locs [i].desc);
      if (s_locs [i].abbr == 0)       break;
      if (s_locs [i].abbr != a_abbr)  continue;
      if (s_locs [i].scrn != a_scrn)  continue;
      /*---(found)-----------------------*/
      DEBUG_DESK   yLOG_note    ("FOUND");
      if (a_left != NULL)  *a_left = s_locs [i].left;
      if (a_topp != NULL)  *a_topp = s_locs [i].topp;
      if (a_wide != NULL)  *a_wide = s_locs [i].wide;
      if (a_tall != NULL)  *a_tall = s_locs [i].tall;
      DEBUG_DESK   yLOG_exit    (__FUNCTION__);
      return i;
   }
   /*---(trouble)------------------------*/
   DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
   return rce;
}

char
yx11_loc_exact          (long a_win, char d, int x, int y, int w, int t)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   char        x_pre       [LEN_HUND]  = "";
   char        x_loc       [LEN_HUND]  = "";
   char        x_suf       [LEN_HUND]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   DEBUG_DESK   yLOG_complex ("place"     , "%1d, %4dx, %4dy, %4dw, %4dt", d, x, y, w, t);
   /*---(prepare)------------------------*/
   sprintf (x_pre, "wmctrl -i -r %ld", a_win);
   sprintf (x_suf, "> /dev/null 2> /dev/null");
   /*---(set desktop)--------------------*/
   if (d != -1)  sprintf (x_loc, "-t %d", d);
   else          sprintf (x_loc, "-b add,sticky");
   sprintf (x_cmd, "%s %s %s", x_pre, x_loc, x_suf);
   DEBUG_DESK   yLOG_info    ("command"   , x_cmd);
   rc = system  (x_cmd);
   DEBUG_DESK   yLOG_value   ("system"    , rc);
   /*---(set size)-----------------------*/
   sprintf (x_loc, "-e 0,%d,%d,%d,%d", x, y, w, t);
   sprintf (x_cmd, "%s %s %s", x_pre, x_loc, x_suf);
   DEBUG_DESK   yLOG_info    ("command"   , x_cmd);
   rc = system  (x_cmd);
   DEBUG_DESK   yLOG_value   ("system"    , rc);
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_win_place          (long a_win, char a_desk, char a_abbr, char a_scrn)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   char        rc          =  -10;
   int         x, y, w, t;
   char        x_pre       [LEN_HUND]  = "";
   char        x_loc       [LEN_HUND]  = "";
   char        x_suf       [LEN_HUND]  = "";
   char        x_cmd       [LEN_RECD]  = "";
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   /*---(get size)-----------------------*/
   rc = yx11_loc_sizing (a_abbr, a_scrn, &x, &y, &w, &t);
   --rce;  if (rc < 0) {
      DEBUG_DESK   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(set size)-----------------------*/
   yx11_loc_exact (a_win, a_desk, x, y, w, t);
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return 0;
}

char
yX11_win_exact          (long a_win, char d, int x, int y, int w, int t)
{
   char        rc          =    0;
   int         cd, cx, cy, cw, ct;
   /*---(header)-------------------------*/
   DEBUG_DESK   yLOG_enter   (__FUNCTION__);
   yX11_win_where  (a_win, &cd, &cx, &cy, &cw, &ct);
   if (d == -1)  d = cd;
   if (x == -1)  x = cx;
   if (y == -1)  y = cy;
   if (w == -1)  w = cw;
   if (t == -1)  t = ct;
   rc = yx11_loc_exact (a_win, d, x, y, w, t);
   /*---(complete)-----------------------*/
   DEBUG_DESK   yLOG_exit    (__FUNCTION__);
   return rc;
}



/*====================------------------------------------====================*/
/*===----                    unit testing accessor                     ----===*/
/*====================------------------------------------====================*/
static void      o___UNITTEST________________o (void) {;}

char*            /* [------] unit test accessor ------------------------------*/
yX11__unit_loc          (char *a_question, int a_num, char a_scrn)
{
   /*---(locals)-----------+-----+-----+-*/
   int         n           =    0;
   /*---(initialize)---------------------*/
   ystrlcpy (unit_answer, "LOC unit         : unknown request", LEN_RECD);
   /*---(string testing)-----------------*/
   yx11_full_refresh ('y');
   if      (strncmp (a_question, "entry"     , 20)  == 0) {
      n = yx11_loc_sizing (a_num, a_scrn, NULL, NULL, NULL, NULL);
      if (n >= 0 && n < s_nloc) {
         snprintf (unit_answer, LEN_RECD, "LOC  entry  (%c%c) : %c %c %-20.20s %4dx %4dy %4dw %4dt", a_num, s_locs [n].scrn, s_locs [n].size, s_locs [n].locn, s_locs [n].desc, s_locs [n].left, s_locs [n].topp, s_locs [n].wide, s_locs [n].tall);
      } else {
         snprintf (unit_answer, LEN_RECD, "LOC  entry  (--) : - - -                       -x    -y    -w    -t");
      }
   }
   /*---(complete)-----------------------*/
   return unit_answer;
}
