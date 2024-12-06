/* This program lets the user interactively play with bezier curves.  It 
 * will also animate them in a visually pleasing manner.  It is a pretty
 * good example of user interface interaction and stuff like that (because
 * it handles letting the user click on points to move them interactively).  
 *
 *   This code (for what it's worth) is under the GNU copyleft.
 *
 *   Dominic Giampaolo
 *   dbg@sgi.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "libsx.h"              /* pick up the libsx stuff we need */
#include "gfx.h"                /* some graphics stuff */
#include "main.h"              
#include "callbacks.h"
#include "bezier.h"


/* define's */
#define X_SIZE 750
#define Y_SIZE 750


/* internal protos */
int  init_myprogram(MyProgram *me);


int main(int argc, char **argv)
{
  MyProgram mydata;

  if (init_myprogram(&mydata) == FALSE)   /* init our data */
    exit(10);

  init_display(argc, argv, &mydata);      /* set up the display */

  MainLoop();                             /* and interact with the user */

  return 0;
}



/* This just initializes our program data structure to some sane
 * default values.  Nothing more, nothing less.
 */
int init_myprogram(MyProgram *me)
{
  memset(me, sizeof(MyProgram), '\0');              /* zero everything out */

  me->pts = (Point *)malloc(sizeof(Point) * 4);
  if (me->pts == NULL)
    return FALSE;

  me->pts[0].x = 0;      me->pts[0].y = 5000;
  me->pts[1].x = 2500;   me->pts[1].y = 7500;
  me->pts[2].x = 5000;   me->pts[2].y = 2500;
  me->pts[3].x = 7500;   me->pts[3].y = 5000;

  me->cur_point = -1;
  me->remove_timeout = TRUE;
  me->timeout_val = 0.03;

  return TRUE;
}



void init_display(int argc, char **argv, MyProgram *me)
{
  char buff[80];
  Widget w[8];

  if (OpenDisplay(argc, argv) == FALSE)
    return;

  w[0] = MakeButton("Redraw Curve", redraw_curve, me);
  w[1] = MakeButton("Reset Curve",  reset_curve,  me);
  w[2] = MakeButton("Animate!",     bez_anim,     me);
  w[3] = MakeButton("Stop Anim",    stop_anim,    me);
  w[4] = MakeButton("Quit!",        quit,         me);

  w[5] = MakeHorizScrollbar(225, time_out_adjust, me);

  sprintf(buff, "Timeout Val: %-6f", me->timeout_val);
  w[6] = MakeLabel(buff);

  w[7] = MakeDrawArea(X_SIZE, Y_SIZE, redisplay, me);

  SetWidgetPos(w[1], PLACE_RIGHT, w[0], NO_CARE, NULL);
  SetWidgetPos(w[2], PLACE_RIGHT, w[1], NO_CARE, NULL);
  SetWidgetPos(w[3], PLACE_RIGHT, w[2], NO_CARE, NULL);
  SetWidgetPos(w[4], PLACE_RIGHT, w[3], NO_CARE, NULL);
  SetWidgetPos(w[5], PLACE_RIGHT, w[4], NO_CARE, NULL);
  SetWidgetPos(w[6], PLACE_RIGHT, w[5], NO_CARE, NULL);
  SetWidgetPos(w[7], PLACE_UNDER, w[0], NO_CARE, NULL);


  me->label_widget = w[6];
  me->draw_widg    = w[7];
  
  SetScrollbar(w[5], me->timeout_val, 0.12, 0.0005);

  SetButtonDownCB(me->draw_widg, button_down);
  SetButtonUpCB(me->draw_widg, button_up);
  SetMouseMotionCB(me->draw_widg, process_motion);

  SetView(0,0, 10000,  10000,  &me->world_view);  /* set world view */
  SetView(0,0, X_SIZE, Y_SIZE, &me->view_port);   /* set view port view */
  MapView(&me->world_view, &me->view_port);

  ShowDisplay();
  
  GetStandardColors();

  SetBgColor(me->draw_widg, BLACK);
}

