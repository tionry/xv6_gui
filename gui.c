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
extern WindowQueue windowQueue;

void initGUI()
{
  GUI_INFO = *((GUI_MODE_INFO *)(GUI_BUF << 4));
}

void drawWindow()
{
  RGB *screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);

  int i, j;
  WindowQueue *p = &windowQueue;

  while (p->next != 0)
  {
    p = p->next;
    switchuvm(p->proc);
    if (p->window != 0)
      if (p->window->show == 1)
        for (i = 0; i < p->window->width; i++)
        {
          for (j = 0; j < CAPTION_HEIGHT; j++)
          {
            screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].R = 0xFF;
            screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].G = 0xFF;
            screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].B = 0x00;
          }
          for (j = CAPTION_HEIGHT; j < p->window->height; j++)
          {
            screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].R = 0x00;
            screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].G = 0xFF;
            screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].B = 0x00;
          }
        }
    switchkvm();
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

