
/*
 * Gag, this is ugly.  Can't find a reliable way to pick up the
 * typedef for ushort, so we #define it instead.
 *
 * Will unix ever be clean?
 */
#ifndef ushort
#define ushort unsigned short
#endif

void bezier4(Point *control_pts, int num_steps);
void SetView(int le, int te, int re, int be, Rectangle *r);
void MapView(Rectangle *ww, Rectangle *vp);
void MoveTo(ushort x, ushort y);
void LineTo(ushort x, ushort y);
void FilledBox(ushort x1, ushort y1, ushort x2, ushort y2);
void Vp2World(int *x, int *y);
