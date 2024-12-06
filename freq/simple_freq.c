#include "libsx.h"
#include "freq.h"

char *SimpleGetFile(char *path)
{
   return GetFile("Simple file requestor", path, NULL, NULL);
}
