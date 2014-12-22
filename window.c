#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "window.h"

Window windowLine[MAX_WINDOW_NUM];
Window windowQueue;

char* mystrcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

void initWindow()
{
  int i;

  for (i = 0; i < MAX_WINDOW_NUM; i++)
  {
    windowLine[i].state = none;
    windowLine[i].next = (Window *)-1;
  }
  windowQueue.state = none;
  windowQueue.next = (Window *)-1;
}

int acquireWindow()
{
  int i;

  for (i = 0; i < MAX_WINDOW_NUM; i++)
    if (windowLine[i].state == none)
      return i;
  return -1;
}

void releaseWindow(int x)
{
  if ((x > 0) && (x < MAX_WINDOW_NUM))
    windowLine[x].state = none;
}

void addWindow(int x)
{
  Window *p = &windowQueue;
  while (((int)p->next) != -1)
    p = p->next;
  p->next = &windowLine[x];
  p = p->next;
}

void deleteWindow(int x)
{
  Window *p = &windowQueue;
  while (p->next != &windowLine[x])
    p = p->next;
  p->next->state = none;
  p->next = p->next->next;
  windowLine[x].next = (Window *)-1;
  updateGUI();
}

int createWindow(int leftTopX, int leftTopY, int width, int height)
{
  int hWind = acquireWindow();

  if (hWind < 0)
    return hWind;

  windowLine[hWind].leftTopX = leftTopX;
  windowLine[hWind].leftTopY = leftTopY;
  windowLine[hWind].width = width;
  windowLine[hWind].height = height;
  mystrcpy(windowLine[hWind].caption, "New window");
  windowLine[hWind].state = show;
  addWindow(hWind);  
  updateGUI();
  return hWind;
}

