#include <stdio.h>
#include <stdlib.h>
#include "libsx.h"


void quit(Widget w, void *data)
{
  CloseWindow();
  ExitMainLoop();
}

void real_quit(Widget w, void *data)
{
  exit(0);
}


int main(int argc, char **argv)
{
  MakeButton("Click to Quit", quit, NULL);
  MainLoop();

  MakeWindow("Second Window", NULL, NONEXCLUSIVE_WINDOW);
  MakeButton("Click to Quit", quit, NULL);
  MainLoop();

  MakeWindow("Third Window", NULL, NONEXCLUSIVE_WINDOW);
  MakeButton("Click to Quit", quit, NULL);
  MainLoop();

  MakeWindow("Last Window", NULL, NONEXCLUSIVE_WINDOW);
  MakeButton("My last window.", real_quit, NULL);
  MainLoop();
  return 0;
}
