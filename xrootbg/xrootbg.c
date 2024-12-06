#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libsx.h"
#include <X11/Xaw3dxft/Xaw3dXft.h>

Xaw3dXftData *xaw3dxft_data = NULL;
Xaw3dXftProc *xaw3dxft_proc = NULL;

static void color_exec(Widget w, CSelData *cdata)
{
char cmd[256];
  sprintf(cmd, "xsetroot -solid \\#%02X%02X%02X ", 
          (int)(cdata->r+0.5), (int)(cdata->g+0.5), (int)(cdata->b+0.5));
  system(cmd);
}

#define DEFAULT_BG "#dcdad5"


int main(int argc, char **argv)
{
  char *args[] = { "-bg", DEFAULT_BG, "-fn", "10x20", NULL };
  char label[80];
  char *ptr;

  XftInitFtLibrary();
  GET_XAW3DXFT_DATA(xaw3dxft_data);

  if (xaw3dxft_data) {
    xaw3dxft_data->encoding = -1;
    xaw3dxft_data->string_hilight = 0;
    xaw3dxft_data->string_use_pixmap = 0;
    xaw3dxft_data->no_hilit_reverse = 1;
    xaw3dxft_data->button_inverse = 0;
    xaw3dxft_data->button_dashed = 1;
    xaw3dxft_data->show_tips = 1;
    xaw3dxft_data->text_bg_hilight = 1;
    xaw3dxft_proc = xaw3dxft_data->proc;
    xaw3dxft_proc->set_default_fontname("Droid Sans Mono-12");
    xaw3dxft_proc->set_insensitive_twist("#a00000");
    xaw3dxft_proc->set_hilit_color("#332211");
  }
  
  PredefArgs = args;
  argc = OpenDisplay(argc, argv);
  if (argc == FALSE)
    return argc;

  GetStandardColors();
  INPUTBG = WHITE;
  BUTTONBG = GetNamedColor("Gray84");

#ifndef Xaw
  SetScrollbarDirection(-1);
#endif

  ptr = getenv("LANG");
  if (!strncmp(ptr, "de", 2)) 
     strcpy(label, "Grund Farbe");
  else
  if (!strncmp(ptr, "fr", 2)) 
     strcpy(label, "Couleur fond d'écran");
  else
     strcpy(label, "Background color");
  SelectColor("40,80,120", 0, label, color_exec, NULL);
 
  return(0);
}
