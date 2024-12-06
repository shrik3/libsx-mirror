void
SetWidgetSize(Widget w, int width, int height)
{
  int n = 0;
  Arg wargs[2];

  if (width > 0)
   {
     XtSetArg(wargs[0], XtNwidth, width);
     n++;
   }

  if (height > 0)
   {
     XtSetArg(wargs[0], XtNheight, height);
     n++;
   }
  
  if (n > 0 && w != NULL)
    XtSetValues(w, wargs, n);
}
