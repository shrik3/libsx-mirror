/* This file contains the various things related to the main body of the 
 * program.  It is pretty sparse, and really shouldn't be too cluttered
 * up.
 *
 */

/* This structure contains information relevant to your program.
 * You should fill it in with information that you need.
 *
 */
typedef struct MyProgram
{
  Rectangle  world_view, view_port;
  Point     *pts;
  Widget     label_widget, draw_widg;
  float      timeout_val;
  int        remove_timeout;
  int        cur_point;
  int        mode;
  int        width, height;  
}MyProgram;

#define MODE_NOTHING      0x00
#define MODE_MOVING       0x01


/* protos */
void init_display(int argc, char **argv, MyProgram *me);


#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif
