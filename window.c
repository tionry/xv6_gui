#include "window.h"
#include "gui.h"
#define MAX_WINDOW_NUM 256

char*
mystrcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

Window windowQueue[MAX_WINDOW_NUM];
int windowNum = 0;

void updateWindow()
{
  RGB *screen = (RGB *)GUI_INFO.PhysBasePtr;
  RGB *screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);

  int i, j, k;

  for (k = 0; k < windowNum; k++)
    if (windowQueue[k].show == 1)
      for (i = 0; i < windowQueue[k].width; i++)
        for (j = 0; j < windowQueue[k].height; j++)
        {
//          screen_temp[windowQueue[k].leftTopX + i][windowQueue[k].leftTopY + j].R = 0xFF;
          screen_temp[(windowQueue[k].leftTopY + j) * SCREEN_WIDTH + windowQueue[k].leftTopX + i].R = 0x00;
          screen_temp[(windowQueue[k].leftTopY + j) * SCREEN_WIDTH + windowQueue[k].leftTopX + i].G = 0xFF;
          screen_temp[(windowQueue[k].leftTopY + j) * SCREEN_WIDTH + windowQueue[k].leftTopX + i].B = 0x00;
        }

  for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    screen[i] = screen_temp[i];
}

Window* createWindow(int leftTopX, int leftTopY, int width, int height)
{
  if (windowNum == MAX_WINDOW_NUM)
    return (Window*)-1;

  windowQueue[windowNum].leftTopX = leftTopX;
  windowQueue[windowNum].leftTopY = leftTopY;
  windowQueue[windowNum].width = width;
  windowQueue[windowNum].height = height;
  mystrcpy(windowQueue[windowNum].caption, "New window");
  windowQueue[windowNum].show = 1;
  windowNum++;
  return &windowQueue[windowNum - 1];
}

