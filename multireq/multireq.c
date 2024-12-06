/*
 * A simple form-fillout function that lets you pass an array of
 * Tag's that describe a form and it will pop up a window and let
 * a user fill out a form that describes the values you'd like.
 *
 * The main function is GetValues().  See the demo program in this
 * same directory (main.c, in the function edit()) for a more
 * detailed example.
 *
 * Written by Allen Martin, (amartin@cs.wpi.edu).
 * revised by Giampiero Raschetti (jimi@settimo.italtel.it)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/StringDefs.h>

#include "libsx.h"
#include "multireq.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct {
  Widget window;
  int cancelled;
  int numentry;

  Widget *w;
  TagList *tags;
  
} MReqData;

MReqData mdata;
int BgColor,FgColor;

/* protos */
static void mreq_ok(Widget w, MReqData *mdata);
static void mreq_cancel(Widget w, MReqData *mdata);
static void mreq_acceptfield(Widget w, char *str, MReqData *mdata);
static void FillForm();
static void SwitchFocusF();
static void SwitchFocusB();
static void SetFocus();
static void InitFocus(MReqData *mdata);
  
int GetValues(TagList *tags)
{
  int num_tags=0, num_widg=0, num_labels=0;
  int i, w, l;
  Widget *label_w, ok_w, cancel_w;
  char string[256], window_name[256]="Requestor";
  int maxwidth=0, widest_label=-1;

  /* first count the number of tag items and required widgets */
  for(; tags[num_tags].tag != TAG_DONE; num_tags++)
    {
      if(tags[num_tags].tag == TAG_LABEL)
	{
	  num_labels++;
	  continue;
	}
      else if(tags[num_tags].tag == TAG_WINDOW_LABEL)
	{
	  if(!tags[num_tags].label)
	    {
	      fprintf(stderr, "Invalid window name passed to GetValues()\n");
	      return(TRUE);
	    }

	  strcpy(window_name, tags[num_tags].label);
	  continue;
	}
      
      /* determine the widest label */
      if(strlen(tags[num_tags].label) > maxwidth)
	{
	  maxwidth = strlen(tags[num_tags].label);
	  widest_label = num_labels;
	}

      num_labels++;
      num_widg++;
    }
      
  /* allocate mem for the widgets */
  if(!(mdata.w = (Widget *)malloc(num_widg * sizeof(Widget))))
    return(TRUE);
  if(!(label_w = (Widget *)malloc(num_labels * sizeof(Widget))))
    {
      free(mdata.w);
      return(TRUE);
    }
  
  mdata.window = MakeWindow(window_name, SAME_DISPLAY, EXCLUSIVE_WINDOW);
  mdata.tags = tags;
  mdata.numentry = num_widg;

  /* create the label widgets first */
  for(i=0, l=0; i<num_tags; i++)
    {
      if(tags[i].tag == TAG_WINDOW_LABEL)
	continue;
      
      label_w[l] = MakeLabel(tags[i].label);
      if(l)
	SetWidgetPos(label_w[l], PLACE_UNDER, label_w[l-1], NO_CARE, NULL);
      l++;
    }
  
  for(i=0, w=0, l=0; i<num_tags; i++)
    {
      switch(tags[i].tag)
	{
	case TAG_STRING:
	  if(tags[i].init)
	    mdata.w[w]=MakeStringEntry((char *)tags[i].data, 250, 
				       (void *)mreq_acceptfield, &mdata);
	  else
	    mdata.w[w] = MakeStringEntry(NULL, 250, (void *)mreq_acceptfield, 
					 &mdata);
	  
	  if(l)
	    SetWidgetPos(mdata.w[w], PLACE_UNDER, label_w[l-1],
			 PLACE_RIGHT, label_w[widest_label]);
	  else
	    SetWidgetPos(mdata.w[w], PLACE_RIGHT, label_w[widest_label],
			 NO_CARE, NULL);

	  l++;
	  w++;
	  break;

	case TAG_INT:
	  if(tags[i].init)
	    {
	      sprintf(string, "%d", *((int *)tags[i].data));
	      mdata.w[w] = MakeStringEntry(string, 250, 
			 		   (void *)mreq_acceptfield, &mdata);
	    }
	  else
	    mdata.w[w] = MakeStringEntry(NULL, 250, (void *)mreq_acceptfield, 
					 &mdata);
	  
	  if(l)
	    SetWidgetPos(mdata.w[w], PLACE_UNDER, label_w[l-1],
			 PLACE_RIGHT, label_w[widest_label]);
	  else
	    SetWidgetPos(mdata.w[w], PLACE_RIGHT, label_w[widest_label],
			 NO_CARE, NULL);

	  l++;
	  w++;
	  break;

	case TAG_FLOAT:
	  if(tags[i].init)
	    {
	      sprintf(string, "%f", *((float *)tags[i].data));
	      mdata.w[w] = MakeStringEntry(string, 250, 
					   (void *)mreq_acceptfield, &mdata);
	    }
	  else
	    mdata.w[w] = MakeStringEntry(NULL, 250, (void *)mreq_acceptfield, 
					 &mdata);
	  
	  if(l)
	    SetWidgetPos(mdata.w[w], PLACE_UNDER, label_w[l-1],
			 PLACE_RIGHT, label_w[widest_label]);
	  else
	    SetWidgetPos(mdata.w[w], PLACE_RIGHT, label_w[widest_label],
			 NO_CARE, NULL);

	  w++;
	  l++;
	  break;

	case TAG_LABEL:
	  l++;
	  break;
	case TAG_WINDOW_LABEL:
	  break;
	  
	default:
	  fprintf(stderr, "GetValues() : Invalid tag item %d\n", tags[i].tag);
	}
    }
  
  ok_w = MakeButton("Ok", (void *)mreq_ok, &mdata);
  SetWidgetPos(ok_w, PLACE_UNDER, label_w[num_labels-1], NO_CARE, NULL);

  cancel_w = MakeButton("Cancel", (void *)mreq_cancel, &mdata);
  SetWidgetPos(cancel_w, PLACE_UNDER, label_w[num_labels-1],
	       PLACE_RIGHT, ok_w);

  ShowDisplay();

  BgColor = GetBgColor(ok_w);
  FgColor = GetFgColor(ok_w);

  InitFocus(&mdata);
  MainLoop();
  
  SetCurrentWindow(ORIGINAL_WINDOW);

  /* check for cancel */
  if(mdata.cancelled)
    return(TRUE);

  free(mdata.w);
  free(label_w);
  
  return(FALSE);
} /* GetValues */

static void mreq_ok(Widget w, MReqData *mdata)
{
  FillForm();
}

static void mreq_cancel(Widget w, MReqData *mdata)
{
  mdata->cancelled = TRUE;
  SetCurrentWindow(mdata->window);
  CloseWindow();
}

/**************************************************************
 * jimi
 **************************************************************/

static int CurrentFocus;

static void mreq_acceptfield(Widget w, char *str, MReqData *mdata)
{
  SwitchFocusF();
}


static void FillForm(Widget w, XEvent *event, String *args, Cardinal n_args)
{
  int widg_num;
  TagList *tagptr;
  char *cptr;
  
  /* extract the info from the widgets */
  for(widg_num=0, tagptr=mdata.tags; tagptr->tag != TAG_DONE; tagptr++)
    {
      switch(tagptr->tag)
	{
	case TAG_STRING:
	  cptr = GetStringEntry(mdata.w[widg_num++]);
	  strcpy(tagptr->data, cptr);
	  break;
	case TAG_INT:
	  cptr = GetStringEntry(mdata.w[widg_num++]);
	  *((int *)tagptr->data) = atoi(cptr);
	  break;
	case TAG_FLOAT:
	  cptr = GetStringEntry(mdata.w[widg_num++]);
	  *((float *)tagptr->data) = atof(cptr);
	  break;
	case TAG_WINDOW_LABEL:
	case TAG_LABEL:
	  break;
	default:
	  fprintf(stderr, "GetValues() : Invalid tag item %d\n", tagptr->tag);
	}
    }

  mdata.cancelled = FALSE;
  SetCurrentWindow(mdata.window);
  CloseWindow();
} /* FillForm */


static void SwitchFocusF(Widget w, XEvent *event, String *args,
			 Cardinal n_args)
{
  Arg   wargs[3];               /* Used to set widget resources */
  int n;
  
  n = 0;
  XtSetArg(wargs[n], XtNbackground, BgColor );       n++;
  XtSetArg(wargs[n], XtNforeground, FgColor );       n++;
  XtSetValues(mdata.w[CurrentFocus], wargs, n);
  
  if ( ++CurrentFocus >= mdata.numentry )
    CurrentFocus=0;

  n = 0;
  XtSetArg(wargs[n], XtNbackground, FgColor );       n++;
  XtSetArg(wargs[n], XtNforeground, BgColor );       n++;
  XtSetValues(mdata.w[CurrentFocus], wargs, n);
  
  XtSetKeyboardFocus(mdata.window, mdata.w[CurrentFocus]);
} /* SwitchFocusF */



static void SetFocus(Widget w, XEvent *event, String *args, Cardinal n_args)
{
  int i,n;
  Arg   wargs[3];
  
  /* find clicked widget */
  for(i=0; i < mdata.numentry; i++) {
    if(w == mdata.w[i])
      break;
  }

  if (i >= mdata.numentry)
    return;

  n = 0;
  XtSetArg(wargs[n], XtNbackground, BgColor );       n++;
  XtSetArg(wargs[n], XtNforeground, FgColor );       n++;
  XtSetValues(mdata.w[CurrentFocus], wargs, n);

  n = 0;
  XtSetArg(wargs[n], XtNbackground, FgColor );       n++;
  XtSetArg(wargs[n], XtNforeground, BgColor );       n++;
  XtSetValues(mdata.w[i], wargs, n);

  CurrentFocus=i;
  XtSetKeyboardFocus(mdata.window, w);
} /* SetFocus */


static void SwitchFocusB(Widget w, XEvent *event, String *args,
			 Cardinal n_args)
{
  Arg   wargs[3];               /* Used to set widget resources */
  int n;
  
  n = 0;
  XtSetArg(wargs[n], XtNbackground, BgColor );       n++;
  XtSetArg(wargs[n], XtNforeground, FgColor );       n++;
  XtSetValues(mdata.w[CurrentFocus], wargs, n);
  
  if ( CurrentFocus-- == 0 )
    CurrentFocus = mdata.numentry - 1;

  n = 0;
  XtSetArg(wargs[n], XtNbackground, FgColor );       n++;
  XtSetArg(wargs[n], XtNforeground, BgColor );       n++;
  XtSetValues(mdata.w[CurrentFocus], wargs, n);
  
  XtSetKeyboardFocus(mdata.window, mdata.w[CurrentFocus]);
} /* SwitchFocusB */

/*-----------------*/
extern XtAppContext  lsx_app_con; /* variabile globale da non vedere ! */
static XtActionsRec actions_table[] =
{
  { "SwitchF",  (XtActionProc)SwitchFocusF },
  { "SwitchB",  (XtActionProc)SwitchFocusB },
  { "SetFocus", (XtActionProc)SetFocus },
  { "Close",    (XtActionProc)FillForm },
};


void InitFocus(MReqData *mdata)
{
  Arg   wargs[3];               /* Used to set widget resources */
  int n;
  XtTranslations trans;
  
  /* Change background color in the first widget */
  n = 0;
  XtSetArg(wargs[n], XtNbackground, FgColor );       n++;
  XtSetArg(wargs[n], XtNforeground, BgColor );       n++;
  XtSetValues(mdata->w[CurrentFocus], wargs, n);
  
  /* Set keyboard focus on the first widget */
  XtSetKeyboardFocus(mdata->window, mdata->w[CurrentFocus]);
  
  /* Set Keyboard callback function to all StringEntry widgets */
  XtAppAddActions(lsx_app_con, actions_table, XtNumber(actions_table));
  trans = XtParseTranslationTable("#override \n\
                                   Shift<Key>Tab: SwitchB()\n\
                                   <Key>Tab: SwitchF()\n\
                                   Shift<Key>Return: Close()\n\
                                   <Btn1Up>: SetFocus()\n\
                                   <Key>F10: SwitchB()\n");
  for(n = 0; n < mdata->numentry; n++)
    XtOverrideTranslations(mdata->w[n], trans);  
}
