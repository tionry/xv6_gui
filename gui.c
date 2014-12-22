#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "gui.h"
#include "window.h"
#include "bitmap.h"

GUI_MODE_INFO GUI_INFO;
extern Window windowQueue;

void initGUI()
{
  GUI_INFO = *((GUI_MODE_INFO *)(GUI_BUF << 4));
}

void drawWindow()
{
  RGB *screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);

  int i, j;
  Window *p = &windowQueue;

  while (((int)p->next) != -1)
  {
    p = p->next;
    if (p->state == show)
      for (i = 0; i < p->width; i++)
        for (j = 0; j < p->height; j++)
        {
          screen_temp[(p->leftTopY + j) * SCREEN_WIDTH + p->leftTopX + i].R = 0x00;
          screen_temp[(p->leftTopY + j) * SCREEN_WIDTH + p->leftTopX + i].G = 0xFF;
          screen_temp[(p->leftTopY + j) * SCREEN_WIDTH + p->leftTopX + i].B = 0x00;
        }
  }
}

void updateGUI()
{
  RGB *screen = (RGB *)GUI_INFO.PhysBasePtr;
  RGB *screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);
  int totalPels = SCREEN_WIDTH * SCREEN_HEIGHT;
  int i;

  memset(screen_temp, 0, sizeof(RGB) * totalPels);
  drawWindow();

  for (i = 0; i < totalPels; i++)
    screen[i] = screen_temp[i];
}

void drawBitmap(char *filename, int leftTopX, int leftTopY)
{
  vfs_open(filename, 0);  
}
