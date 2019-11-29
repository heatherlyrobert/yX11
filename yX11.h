/*===[[ START HDOC ]]=========================================================*/
/*---[[ HEADER ]]-------------------------------------------------*

 *   niche         : xlib/opengl    (xwindows and opengl)
 *   application   : yX11           (standardize init and wrap-up)
 *   purpose       : provide consistent and simple X11/OpenGL setup/teardown
 *   base_system   : gnu/linux
 *   lang_name     : ansi-c         (because its right, just, and proper)
 *   created       : 2009-05        (near the start of my opengl training)
 *   author        : the_heatherlys
 *   dependencies  : xlib/glx
 *
 */
/*---[[ PURPOSE ]]------------------------------------------------*

 *   yXINIT is a set of library functions to provide highly reliable, testable,
 *   and consistent connection, setup, and destruction support for both XLIB for
 *   x-windows and GLX for opengl.  because every application has to accomplish
 *   these functions in a nearly consistent fashion, it is simple to ignore
 *   basic checks and safety precautions because it hasn't happened in a while.
 *   also, it is very easy to forget all the great things learned in this area
 *   as it is not a consistent focus.  the goal is to take a painful, tedious,
 *   onerous, haphazard, and error prone process and turn it a easy,
 *   flexible, predictable, rapid, and automated set of functions.
 *
 *   yXINIT will focus on...
 *      - establishing a connection with the x-server
 *      - gathering the key statistics that can be learned about that server
 *      - establishing a connection and visual for opengl
 *      - creating and mapping a window
 *      - getting default graphics contexts in place
 *      - destroying both the x-windows and opengl connections
 *      - putting a simple error handler in place
 *
 *   yXINIT will NOT provide...
 *      - event handling
 *      - any drawing
 *      - window handling outside of initial mapping
 *
 *   there are many existing libraries and utilities have been built by better
 *   programmers and are likely superior in speed, size, capability, and
 *   reliability; BUT, i would not have learned nearly as much using them,
 *   so i follow the adage..
 *
 *   TO TRULY LEARN> do not seek to follow in the footsteps of the men of old;
 *   seek what they sought ~ Matsuo Basho
 *
 *   the basic idea is to take the great ideas from existing frameworks
 *   without taking all the unneeded complexity that comes from backwards
 *   compatiblilty, odd viewpoints, and traditional understandings.
 *
 */
/*---[[ END HDOC ]]-----------------------------------------------------------*/


/*===[[ HEADER GUARD ]]=======================================================*/
#ifndef YX11   
#define YX11   loaded





#define    YX_FOCUSABLE   'y'
#define    YX_HIDDEN      'n'

#define    YX_SIZEABLE    'y'
#define    YX_FIXED       'n'

#define    YX_VERBOSE     'y'
#define    YX_SILENT      'n'


/*===[[ PROTOTYPES ]]=========================================================*/
char      *yX11_version            (void);
char       yX11_start              (char*, int, int, char, char, char);
char       yX11_screensize         (int *a_wide, int *a_tall, int *a_deep);
char       yX11_resize             (int a_wide, int a_tall);
char       yX11_move               (int a_xpos, int a_ypos);
char       yX11_end                (void);
ulong      yXINIT_focus            (void);


#endif

