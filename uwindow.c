#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "uwindow.h"

void initWindow(Window *window)
{
  memset(window, 0, sizeof(Window));
}

