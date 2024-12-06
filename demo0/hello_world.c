#include "libsx.h"

int main(int argc, char **argv)
{
   OpenDisplay(argc, argv);
   MakeLabel("Hello World");
   MainLoop();
   return 0;
}
