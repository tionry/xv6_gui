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
extern struct Window windowLine[MAX_WINDOW_NUM];
extern int windowNum;

void initGUI()
{
  GUI_INFO = *((GUI_MODE_INFO *)(GUI_BUF << 4));
}

void drawWindow()
{
  RGB *screen = (RGB *)GUI_INFO.PhysBasePtr;
  RGB *screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);

  int i, j, k;

  for (k = 0; k < windowNum; k++)
    if (windowLine[k].show == 1)
      for (i = 0; i < windowLine[k].width; i++)
        for (j = 0; j < windowLine[k].height; j++)
        {
          screen_temp[(windowLine[k].leftTopY + j) * SCREEN_WIDTH + windowLine[k].leftTopX + i].R = 0x00;
          screen_temp[(windowLine[k].leftTopY + j) * SCREEN_WIDTH + windowLine[k].leftTopX + i].G = 0xFF;
          screen_temp[(windowLine[k].leftTopY + j) * SCREEN_WIDTH + windowLine[k].leftTopX + i].B = 0x00;
        }

  for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    screen[i] = screen_temp[i];
}

void updateGUI()
{
  drawWindow();
}

//void drawBitmap(char *filename, int leftTopX, int leftTopY);

