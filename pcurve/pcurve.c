/*
   pcurve -- A program to draw parametric curves and let you play with
             the parameters.

  This program lets a user play with simple parametric curves of the
form:

        (alpha * cos(u * t)) + (beta * sin(v * t))

  The interesting parameters are u & v.  Alpha and beta just affect
the size of the overall curve (in x and y respectively).  There are
also controls for adjusting the number of divisions of the curve drawn
and the number of periods of Pi to iterate over.

  All in all, pretty simple and basic but fun to play with.

  Dominic Giampaolo
  dbg@sgi.com
*/

  
#include <stdio.h>
#include <stdlib.h>
#include "GL/gl.h"
#include "GL/glx.h"
#include "GL/glut.h"
#include "libsx.h"
#include "controlbox.h"
#include "math.h"
  
int attributes[] = {
                     GLX_RGBA,
		     GLX_DEPTH_SIZE, 24,
		     GLX_DOUBLEBUFFER,
		  /*  GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, */
		     None
		   };

Widget  gui[25];
Display *dpy;
Window win;


#define QUIT           0
#define DRAW_AREA      1
#define PARAM_BOX      2

typedef struct CurveInfo
{
  float alpha, beta;
  float u, v;  
  float num_divisions;
  float periods;
}CurveInfo;


SliderVar params[] =
{
 { "Alpha", NULL, 5.0, 0.05, NULL },
 { "BETA",  NULL, 5.0, 0.05, NULL },
 { "U-val", NULL, 5.0, 0.05, NULL },
 { "V-val", NULL, 5.0, 0.05, NULL },
 { "# Divisions", NULL, 1000.0, 10.0, NULL },
 { "# Periods",   NULL, 20.0,  0.3, NULL },
 { NULL,    NULL, 0,   0,   NULL } 
};




#define TIME_OUT 50   /* amount of time between redraw (in milli-secs) */


void
glLine(float x1, float y1, float x2, float y2)
{
  glBegin(GL_LINES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
  glEnd();
}

void
gl_setup(void)
{

  /*
   * Clear the screen (both buffers).
   */
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glXSwapBuffers(dpy, win);
  glClear(GL_COLOR_BUFFER_BIT);
  glXSwapBuffers(dpy, win);

  /*
   * Setup some standard gl stuff
   */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-5.0,5.0, -5.0,5.0, -5.0,5.0);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}


void Quit(Widget w, void *data)
{
  exit(0);
}


void redraw(Widget w, int width, int height, void *data)
{
  SetDrawArea(w);
  glViewport(0,0,width,height);
}

void draw_stuff(void *data)
{
  CurveInfo *ci = (CurveInfo *)data;
  float r,g,b,x,y;
  int i;

  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINE_LOOP);

  for(i=0; i < ci->num_divisions; i++)
   {
     float radian;

     radian = (float)i * ((M_PI * ci->periods)/ci->num_divisions);
     x = cos(radian * ci->u) * ci->alpha;
     y = sin(radian * ci->v) * ci->beta;

     glVertex2f(x, y);
   }

  glEnd();
  glXSwapBuffers(dpy,win);

  AddTimeOut(TIME_OUT, draw_stuff, ci);
}

int main(int argc, char **argv)
{
  CurveInfo ci;
  XVisualInfo *vi;
  GLXContext glc;

  // argc = OpenGLDisplay(argc, argv, attributes);
  dpy = XOpenDisplay(NULL);
  if (argc == 0)                   /* woops, couldn't get started */
    exit(5);

  /*
   * Setup the initial curve info
   */
  ci.alpha = 4.0;  ci.beta = 4.0;
  ci.u     = 1.0;  ci.v    = 1.0;
  ci.num_divisions = 100;  ci.periods = 2.0;
    
  params[0].val = &ci.alpha;  params[1].val = &ci.beta;
  params[2].val = &ci.u;      params[3].val = &ci.v;
  params[4].val = &ci.num_divisions;
  params[5].val = &ci.periods;

  gui[QUIT]      = MakeButton("Quit", Quit, &ci);
  gui[DRAW_AREA] = MakeDrawArea(500,500, redraw, &ci);
  gui[PARAM_BOX] = MakeControlBox("Curve Parameters", &params[0]);

  SetWidgetPos(gui[PARAM_BOX], PLACE_UNDER, gui[QUIT],      NO_CARE, NULL);
  SetWidgetPos(gui[DRAW_AREA], PLACE_UNDER, gui[PARAM_BOX], NO_CARE, NULL);

  srand48((long)101);

  ShowDisplay();
  GetStandardColors();

  dpy = XtDisplay(gui[DRAW_AREA]);
  win = XtWindow(gui[DRAW_AREA]);
  vi = glXChooseVisual(dpy, 0, attributes);
  glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
  glXMakeCurrent(dpy, win, glc);
  gl_setup();

  AddTimeOut(TIME_OUT, draw_stuff, &ci);
  MainLoop();   /* off we go! */

  return 0;
}
