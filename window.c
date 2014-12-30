#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "window.h"

WindowQueue windowLine[MAX_WINDOW_NUM];
WindowQueue windowQueue;

void initWindow()
{
  int i;

  for (i = 0; i < MAX_WINDOW_NUM; i++)
  {
    windowLine[i].proc = 0;
    windowLine[i].window = 0;
    windowLine[i].next = 0;
  }
  windowQueue.proc = 0;
  windowQueue.window = 0;
  windowQueue.next = 0;
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
  while (p->next != 0)
    p = p->next;
  p->next = &windowLine[x];
  p = p->next;
  p->next = 0;
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
  switchkvm();
  updateGUI();
  switchuvm(proc);
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
      p->next = p->next->next;
      switchkvm();
      updateGUI();
      switchuvm(proc);
      return 0;
    }
    p = p->next;
  }
  return -1;
}

