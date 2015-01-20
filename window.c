#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "window.h"
#include "gui.h"

WindowQueue windowLine[MAX_WINDOW_NUM];
WindowQueue windowQueue;
WindowQueue *lastWindow;
ImageView closeTag;

void initWindow()
{
  int i;

  for (i = 0; i < MAX_WINDOW_NUM; i++)
  {
    windowLine[i].proc = 0;
    windowLine[i].window = 0;
    windowLine[i].next = 0;
    windowLine[i].prev = 0;
  }
  windowQueue.proc = 0;
  windowQueue.window = 0;
  windowQueue.next = 0;
  windowQueue.prev = 0;
  lastWindow = &windowQueue;
}

int acquireWindow()
{
  int i;

  for (i = 0; i < MAX_WINDOW_NUM; i++)
    if (windowLine[i].proc == 0)
      return i;
  return -1;
}

void addWindow(int x)
{
  WindowQueue *p = &windowQueue;
  WindowQueue *q;
  while (p->next != 0)
    p = p->next;
  q = p;
  p->next = &windowLine[x];
  p = p->next;
  p->prev = q;
  p->next = 0;
  lastWindow = &windowLine[x];
}

int sys_createWindow(void)
{
  int window;

  if (argint(0, &window) < 0)
    return -1;

  int hWind = acquireWindow();
  if (hWind < 0)
    return hWind;
  windowLine[hWind].proc = proc;
  windowLine[hWind].window = (Window *)window;
  addWindow(hWind);
  updateBackWindows();
  return hWind;
}

int sys_deleteWindow(void)
{
  int window;
  if (argint(0, &window) < 0)
    return -1;

  WindowQueue *p = &windowQueue;
  while (p->next != 0)
  {
    if (p->next->window == (Window*)window)
    {
      p->next->proc = 0;
      p->next->window = 0;
      if (p->next == lastWindow)
        lastWindow = p;
      p->next = p->next->next;
      if (p->next)
        p->next->prev = p;
      updateBackWindows();
      return 0;
    }
    p = p->next;
  }
  return -1;
}

void moveWindow(Window *window, int fx, int fy, int tx, int ty)
{
  int dx = tx - fx;
  int dy = ty - fy;
  if (window->leftTopX + dx < 0)
    dx = -window->leftTopX;
  if (window->leftTopY + dy < 0)
    dy = -window->leftTopY;
  if (window->leftTopX + window->width + dx >= SCREEN_WIDTH)
    dx = SCREEN_WIDTH - (window->leftTopX + window->width);
  if (window->leftTopY + window->height + dy >= SCREEN_HEIGHT)
    dy = SCREEN_HEIGHT - (window->leftTopY + window->height);
  window->leftTopX += dx;
  window->leftTopY += dy;
  cprintf("dx = %d, dy = %d\n",  window->leftTopX,  window->leftTopY);
  if (dx || dy) 
    updateLastWindow();
}

