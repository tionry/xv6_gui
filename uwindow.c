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

void handleEvent(Window *window)
{
  int i;

  for (i = 0; i < window->widgetsNum; i++)
    switch (window->widgets[i].type)
    {
    case iconView:
      if (window->widgets[i].context.iconView->onLeftDoubleClickHandler.triggered == 1)
      {
        window->widgets[i].context.iconView->onLeftDoubleClickHandler.handlerFunction(window->widgets + i);
        window->widgets[i].context.iconView->onLeftDoubleClickHandler.triggered = 0;
      }
      break;
    default:
      break;
    }
}

