/* This is the main body of the program.  It is just a simple skeleton
 * and should be fleshed out as desired.  Mainly what you'll do to flesh 
 * it out is to edit the data structure defined in main.h so that it contains 
 * the information your program needs.  Then modify init_display() to create
 * the interface you want, and then just write the associated callback 
 * routines that are driven by the display.  Easy, huh?
 *
 *               --  This code is under the GNU copyleft  --
 *
 *   Dominic Giampaolo
 *   dbg@sgi.com
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "libsx.h"             /* should come first, defines libsx stuff  */
#include "controlbox.h"

#include "main.h"              /* where program specific stuff is defined */
#include "callbacks.h"         /* prototypes for callback functions       */

/* define's */
#define X_SIZE 300             /* default draw area size, change as desired */
#define Y_SIZE 300



int main(int argc, char **argv)
{
  MyProgram mydata;

  mydata.whatever = 0;                /* do your initializations here */

  init_display(argc, argv, &mydata);  /* setup the display */

  MainLoop();                         /* go right into the main loop */

  return 0;
}


float f1=50.0, f2=0.75, f3=7.5;
SliderVar sliders[] =
{
 {  "Var #1",       &f1, 100.0,  1.0, NULL, NULL },
 {  "Float Var #2", &f2,   1.0,  0.1, NULL, NULL },
 {  "Thing 3",      &f3,  25.0, 10.0, NULL, NULL },
 {  NULL,          NULL,   0.0,  0.0, NULL, NULL }
};


/* This function sets up the display.  For any kind of a real program, 
 * you'll probably want to save the values returned by the MakeXXX calls
 * so that you have a way to refer to the display objects you have 
 * created (like if you have more than one drawing area, and want to
 * draw into both of them).
 */

void init_display(int argc, char **argv, MyProgram *me)
{
  Widget w[6];

  if (OpenDisplay(argc, argv) == FALSE)
    return;

  w[0] = MakeButton("Change Slider #1", button1, me);
  w[1] = MakeButton("Dump Slider Vals", button2, me);
  w[2] = MakeButton("Quit!",     quit,    me);

  w[4] = MakeDrawArea(X_SIZE, Y_SIZE, redisplay, me);

  SetWidgetPos(w[1], PLACE_RIGHT, w[0], NO_CARE, NULL);
  SetWidgetPos(w[2], PLACE_RIGHT, w[1], NO_CARE, NULL);
  SetWidgetPos(w[4], PLACE_UNDER, w[0], NO_CARE, NULL);

  w[6] = MakeControlBox("Some Cool Sliders", &sliders[0]);
  SetWidgetPos(w[6], PLACE_UNDER, w[4], NO_CARE, NULL); 

  /*
   * These callbacks are unnecessary for most applications, but
   * they're here for completeness.  Remove them if you don't
   * need them.
   */
  SetButtonDownCB(w[4],  button_down);
  SetButtonUpCB(w[4],    button_up);
  SetKeypressCB(w[4],    keypress);
  SetMouseMotionCB(w[4], motion);



  /* This call actually causes the whole thing to be displayed on the
   * screen.  You have to call this function before doing any drawing
   * into the window.
   */
  ShowDisplay();

  
  /* Get standard (red, blue, green, yellow, black, white) colors for
   * drawing stuff.  Check libsx.h for more info. 
   */
  GetStandardColors();

  
  SetBgColor(sliders[0].slider, RED);
  SetFgColor(sliders[0].slider, WHITE);
}




