#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "window.h"

Window windowLine[MAX_WINDOW_NUM];
Window *windowQueue = (Window *)-1;
int windowNum = 0;

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
  
}

int createWindow(int leftTopX, int leftTopY, int width, int height)
{
  if (windowNum == MAX_WINDOW_NUM)
    return (-1);

  windowLine[windowNum].leftTopX = leftTopX;
  windowLine[windowNum].leftTopY = leftTopY;
  windowLine[windowNum].width = width;
  windowLine[windowNum].height = height;
  mystrcpy(windowLine[windowNum].caption, "New window");
  windowLine[windowNum].show = 1;
  windowNum++;
  updateGUI();
  return (windowNum - 1);
}

