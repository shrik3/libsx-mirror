/*
 * this file is an attempt at making a "control box" routine that will
 * take an array of variables and make a bunch of sliders to control
 * each of the variables.  It will put them in their own form widget
 * and return the form widget which you can then place wherever you like.
 *
 * The idea here is that a lot of times you just have a bunch of
 * parameters you want to control with sliders.  It should be easy
 * to do that, and this function is an attempt at making it easy.
 * All you should have to do with this is declare an array of SliderVal
 * structures describing the parameters you want to have under slider
 * control (i.e. min, max, size_shown and their name) and blammo! you've
 * got a nice box with a title that has a bunch of sliders that control
 * your variables.  You also have access to the underlying slider widgets
 * in case you need to change the parameters.
 *
 *  Dominic Giampaolo
 *  dbg@sgi.com
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "libsx.h"
#include "controlbox.h"



static void
change_val(Widget w, float new_val, void *arg)
{
  SliderVar *var = (SliderVar *)arg;
  char buff[256];

  sprintf(buff, "%f", new_val);
  SetStringEntry(var->text, buff);

  *var->val = new_val;
}


static void
str_change_val(Widget w, char *txt, void *arg)
{
  SliderVar *var = (SliderVar *)arg;
  float new_val;
  char buff[256];

  if (txt == NULL)
    return;

  if (*txt != '.' && *txt != '-' && isdigit(*txt) == 0)
    return;

  new_val = atof(txt);
  if (new_val < 0)
   {
     new_val = 0.0;

     sprintf(buff, "%f", new_val);
     SetStringEntry(var->text, buff);
   }
  if (new_val > var->max)
   {
     new_val = var->max;

     sprintf(buff, "%f", new_val);
     SetStringEntry(var->text, buff);
   }

  SetScrollbar(var->slider, new_val, var->max, var->shown);

  *var->val = new_val;
}


Widget
MakeControlBox(char *title_str, SliderVar *vars)
{
  int i, longest_len, longest_index, num_sliders;
  Widget form, title = None, *label, *slider, *text, previous_form;
  XFont font;

  previous_form = GetForm();

  form = MakeForm(TOP_LEVEL_FORM);  /* the parent could be passed in */
  if (form == NULL)
    return NULL;

  if (title_str)
    title = MakeLabel(title_str);
  
  longest_len = -1;
  font = GetWidgetFont(form);

  for(i=0; vars[i].name != NULL; i++)
   {
     int len;

     if (font != NULL)
       len = TextWidth(font, vars[i].name);
     else
       len = strlen(vars[i].name);
     
     if (len > longest_len)
      {
	longest_len   = len;
	longest_index = i;
      }
   }

  /*
   * At this point "i" contains how many sliders we need to create.
   * We'll use this information to allocate some arrays to hold
   * the widget pointers we get back from the MakeXXX() functions.
   * We need to save the values so that we can set their position later.
   */
  num_sliders = i;

  label  = calloc(1, num_sliders*sizeof(Widget));
  slider = calloc(1, num_sliders*sizeof(Widget));
  text   = calloc(1, num_sliders*sizeof(Widget));
  
  if (label == NULL || slider == NULL || text == NULL)
   {
     if (label)  free(label);
     if (slider) free(slider);
     if (text)   free(text);

     return NULL;
   }

  /*
   * First we just create the widgets, then we'll lay them out.
   */
  for(i=0; i < num_sliders; i++)
   {
     char buff[256];

     sprintf(buff, "%f", *vars[i].val);
     
     label[i]  = MakeLabel(vars[i].name);
     slider[i] = MakeHorizScrollbar(250, change_val, &vars[i]);
     text[i]   = MakeStringEntry(buff, 100, str_change_val, &vars[i]);

     vars[i].slider = slider[i];  /* save it for the user */
     vars[i].text   = text[i];

     if (label[i] == NULL || slider[i] == NULL || text == NULL)
      {
	/* XXX -- destroy the widgets? how? */
	free(label);
	free(slider);
	free(text);

	return NULL;
      }

     SetScrollbar(slider[i], *vars[i].val, vars[i].max, vars[i].shown);
   }

 
  /*
   * Here we set the positions of all the widgets we just created.
   * We set the first row of things just under the title string
   * if there was one.  All the sliders always get put to the right
   * of the widest name (based on the width of text).
   */
  if (title)
   {
     AttachEdge(title, TOP_EDGE, ATTACH_TOP);
     AttachEdge(title, BOTTOM_EDGE, ATTACH_TOP);
     AttachEdge(title, LEFT_EDGE, ATTACH_LEFT);
     AttachEdge(title, RIGHT_EDGE, ATTACH_LEFT);
   }

  for(i=0; i < num_sliders; i++)
   {
     if (i == 0)
      {
	SetWidgetPos(label[i],  PLACE_UNDER, title, NO_CARE, NULL);
	SetWidgetPos(slider[i], PLACE_RIGHT, label[longest_index],
		                PLACE_UNDER, title);
	SetWidgetPos(text[i],   PLACE_RIGHT, slider[i], PLACE_UNDER, title);
      }
     else
      {
	SetWidgetPos(label[i],  PLACE_UNDER, label[i-1], NO_CARE, NULL);

	SetWidgetPos(slider[i], PLACE_RIGHT, label[longest_index],
		                PLACE_UNDER, label[i-1]);

	SetWidgetPos(text[i],   PLACE_RIGHT, slider[i],PLACE_UNDER,label[i-1]);
      }
     
     AttachEdge(label[i],  TOP_EDGE,    ATTACH_TOP);
     AttachEdge(label[i],  BOTTOM_EDGE, ATTACH_TOP);
     AttachEdge(label[i],  LEFT_EDGE,   ATTACH_LEFT);
     AttachEdge(label[i],  RIGHT_EDGE,  ATTACH_LEFT);

     AttachEdge(slider[i], TOP_EDGE,    ATTACH_TOP);
     AttachEdge(slider[i], BOTTOM_EDGE, ATTACH_TOP);
     AttachEdge(slider[i], LEFT_EDGE,   ATTACH_LEFT);
     AttachEdge(slider[i], RIGHT_EDGE,  ATTACH_RIGHT);

     AttachEdge(text[i], TOP_EDGE,      ATTACH_TOP);
     AttachEdge(text[i], BOTTOM_EDGE,   ATTACH_TOP);
     AttachEdge(text[i], LEFT_EDGE,     ATTACH_RIGHT);
     AttachEdge(text[i], RIGHT_EDGE,    ATTACH_RIGHT);
   }
      
  SetForm(previous_form);

  return form;
}
