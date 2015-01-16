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
#include "mouse.h"

GUI_MODE_INFO GUI_INFO;
extern WindowQueue windowQueue;
extern struct mouseinfo mouse_info;
RGB *screen, *screen_temp;

void initGUI()
{
  GUI_INFO = *((GUI_MODE_INFO *)(GUI_BUF << 4));
  screen = (RGB *)GUI_INFO.PhysBasePtr;
  screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);
}

void drawImageView(ImageView *imageView)
{
  int i, j;

  for (i = 0; i < imageView->width; i++)
    for (j = 0; j < imageView->height; j++)
      screen_temp[(imageView->leftTopY + j) * SCREEN_WIDTH + imageView->leftTopX + i] = imageView->image[(imageView->height - 1 - j) * imageView->width + i];
}

void drawWindows()
{
  int i, j, k;
  WindowQueue *p = &windowQueue;

  while (p->next != 0)
  {
    p = p->next;
    switchuvm(p->proc);
    if (p->window != 0)
      if (p->window->show == 1)
      {
        for (i = 0; i < p->window->width; i++)
        {
          if (p->window->hasCaption == 1)
          {
            for (j = 0; j < CAPTION_HEIGHT; j++)
            {
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].R = 0x00;
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].G = 0x00;
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].B = 0x00;
            }
            for (j = CAPTION_HEIGHT; j < p->window->height; j++)
            {
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].R = 0xbb;
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].G = 0xbb;
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].B = 0xbb;
            }
          }
          else
          {
            for (j = 0; j < p->window->height; j++)
            {
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].R = 0xbb;
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].G = 0xbb;
              screen_temp[(p->window->leftTopY + j) * SCREEN_WIDTH + p->window->leftTopX + i].B = 0xbb;
            }
          }
        }
        for (k = 0; k < p->window->widgetsNum; k++)
          if (p->window->widgets[k].type == imageView)
            drawImageView(p->window->widgets[k].context.imageView);
      }
    switchkvm();
  }
}

void drawMouse()
{
  int i, j;

  for (i = 0; i < 10; i++)
    for (j = 0; j < 20; j++)
    {
      screen[(mouse_info.y_position + j) * SCREEN_WIDTH + mouse_info.x_position + i].R = 0x00;
      screen[(mouse_info.y_position + j) * SCREEN_WIDTH + mouse_info.x_position + i].G = 0x00;
      screen[(mouse_info.y_position + j) * SCREEN_WIDTH + mouse_info.x_position + i].B = 0x00;
    }
}

void updateWindow()
{
  int totalPels = SCREEN_WIDTH * SCREEN_HEIGHT;
  
  memset(screen_temp, 0xff, sizeof(RGB) * totalPels);
  cli();
  drawWindows();
  sti();
  memmove(screen, screen_temp, sizeof(RGB) * totalPels);
  drawMouse();
}

void updateMouse()
{
  int totalPels = SCREEN_WIDTH * SCREEN_HEIGHT;

  memmove(screen, screen_temp, sizeof(RGB) * totalPels);
  drawMouse();
}

