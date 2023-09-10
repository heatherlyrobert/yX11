#include "yX11.h"
#include "yX11_priv.h"


#define   MAX_PROC   100
static struct cPROC {
   short       rpid;
   char        pubname     [LEN_LABEL];
   char        cmdline     [LEN_RECD];
   char        state;
   int         ppid;
   int         bash;
   int         uid;
   char        home        [LEN_PATH];
   char        pwd         [LEN_PATH];
   char        color       [LEN_TERSE];
   char        cols;
   char        rows;
} s_procs [MAX_PROC];
static char    s_nproc  = 0;


char
yx11_proc__clear        (void)
{
   int         i           =    0;
   for (i = 0; i < MAX_PROC; ++i) {
      s_procs [i].rpid    = -1;
      s_procs [i].pubname [0] = '\0';
      s_procs [i].cmdline [0] = '\0';
      s_procs [i].state   = '·';
      s_procs [i].ppid    = -1;
      s_procs [i].bash    = -1;
      s_procs [i].uid     = -1;
      s_procs [i].home    [0] = '\0';
      s_procs [i].pwd     [0] = '\0';
      s_procs [i].color   [0] = '\0';
      s_procs [i].cols    = -1;
      s_procs [i].rows    = -1;
   }
   s_nproc = 0;
   return 0;
}

static DIR        *s_dir       = NULL;
static tDIRENT    *s_file      = NULL;

char
yx11_proc__open         (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_spoint  (s_dir);
   --rce;  if (s_dir != NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(open)---------------------------*/
   DEBUG_INPT   yLOG_snote   ("/proc");
   s_dir = opendir ("/proc");
   DEBUG_INPT   yLOG_spoint  (s_dir);
   --rce;  if (s_dir == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yx11_proc__close        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_senter  (__FUNCTION__);
   /*---(defense)------------------------*/
   DEBUG_INPT   yLOG_spoint  (s_dir);
   --rce;  if (s_dir == NULL) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(close)--------------------------*/
   rc = closedir (s_dir);
   DEBUG_INPT   yLOG_sint    (rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_sexitr  (__FUNCTION__, rce);
      return rce;
   }
   /*---(ground)-------------------------*/
   s_dir = NULL;
   DEBUG_INPT   yLOG_spoint  (s_dir);
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_sexit   (__FUNCTION__);
   return 0;
}

char
yx11_proc__pass         (char a_type)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   tDIRENT    *x_file      = NULL;
   int         x_rpid      =    0;
   char        x_pubname   [LEN_LABEL] = "";
   int         x_ppid      =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(defense)------------------------*/
   rc = yx11_proc__open ();
   DEBUG_INPT   yLOG_value   ("open"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(walk files)---------------------*/
   DEBUG_INPT   yLOG_note    ("walk through processes");
   while ((x_file = readdir (s_dir)) != NULL) {
      rc = 0;
      /*---(simple filtering)-------------------*/
      if (strchr (YSTR_NUMBER, x_file->d_name [0]) == NULL) {
         DEBUG_INPT   yLOG_note    ("not leading number");
         continue;
      }
      x_rpid = atoi (x_file->d_name);
      DEBUG_INPT   yLOG_value   ("x_rpid"    , x_rpid);
      if (x_rpid <= 0) {
         DEBUG_INPT   yLOG_note    ("not a process entry");
         continue;
      }
      /*---(get data)--------------------*/
      switch (a_type) {
      case 'E' :
         s_procs [s_nproc].rpid = x_rpid;
         rc = yEXEC_proc_data  (x_rpid, s_procs [s_nproc].pubname, s_procs [s_nproc].cmdline, &(s_procs [s_nproc].state), &(s_procs [s_nproc].ppid));
         DEBUG_INPT   yLOG_value   ("proc_data" , rc);
         if (rc < 0)  continue;
         if (strcmp (s_procs [s_nproc].pubname, "Eterm") != 0)  continue;
         ++s_nproc;
         break;
      case 'b' :
         rc = yEXEC_proc_data  (x_rpid, x_pubname, NULL, NULL, &x_ppid);
         if (strcmp (x_pubname, "bash") != 0)  continue;
         printf ("%d, %s\n", x_rpid, x_pubname);
         for (i = 0; i < s_nproc; ++i) {
            if (x_ppid == s_procs [i].rpid) {
               s_procs [i].bash = x_rpid;
               rc = yEXEC_proc_eterm (x_rpid, &(s_procs [i].uid), s_procs [i].home, s_procs [i].pwd, NULL, s_procs [i].color, NULL, NULL, &(s_procs [i].rows), &(s_procs [i].cols));
               break;
            }
         }
         break;
      }
      /*---(done)------------------------*/
   }
   /*---(close)--------------------------*/
   rc = yx11_proc__close ();
   DEBUG_INPT   yLOG_value   ("close"     , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}

char
yx11_proc_gather        (void)
{
   /*---(locals)-----------+-----+-----+-*/
   char        rce         =  -10;
   int         rc          =    0;
   int         i           =    0;
   /*---(header)-------------------------*/
   DEBUG_INPT   yLOG_enter   (__FUNCTION__);
   /*---(prepare)------------------------*/
   yx11_proc__clear ();
   /*---(eterms)-------------------------*/
   rc = yx11_proc__pass ('E');
   DEBUG_INPT   yLOG_value   ("eterms"    , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(bash)---------------------------*/
   rc = yx11_proc__pass ('b');
   DEBUG_INPT   yLOG_value   ("bash"      , rc);
   --rce;  if (rc < 0) {
      DEBUG_INPT   yLOG_exitr   (__FUNCTION__, rce);
      return rce;
   }
   /*---(print list)---------------------*/
   for (i = 0; i < s_nproc; ++i) {
      printf ("%2d, %5d, %-15.15s, %-30.30s, %c, %5d, %5d, %s, %5d, %5d\n", i,
            s_procs [i].rpid, s_procs [i].pubname, s_procs [i].cmdline,
            s_procs [i].state, s_procs [i].ppid, s_procs [i].bash,
            s_procs [i].color, s_procs [i].cols, s_procs [i].rows);
   }
   /*---(complete)-----------------------*/
   DEBUG_INPT   yLOG_exit    (__FUNCTION__);
   return rc;
}


