/*
 * This file contains the structures and #define's for use with
 * the control box routines.
 *
 *  Dominic Giampaolo
 *  dbg@sgi.com
 */

typedef struct SliderVar
{
  char *name;
  float *val;
  float max, shown;
  Widget slider;      /* gets filled in by MakeControlBox() */
  Widget text;        /* gets filled in by MakeControlBox() */
}SliderVar;


/*
 * MakeControlBox() returns a form widget who's position you can
 * set with SetWidgetPos().  The form will contain an vertical array
 * of sliders and any time the user plays with one, your variables
 * will change.  You don't have to do anything really.
 */
Widget MakeControlBox(char *title, SliderVar *variables);
