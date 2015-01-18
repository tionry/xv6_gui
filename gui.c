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

void drawWindow(Window *window)
{
  int i, j;

  for (i = 0; i < window->width; i++)
  {
    if (window->hasCaption == 1)
    {
      for (j = 0; j < CAPTION_HEIGHT; j++)
      {
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].R = 0x00;
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].G = 0x00;
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].B = 0x00;
      }
      for (j = CAPTION_HEIGHT; j < window->height; j++)
      {
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].R = 0xbb;
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].G = 0xbb;
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].B = 0xbb;
      }
    }
    else
    {
      for (j = 0; j < window->height; j++)
      {
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].R = 0xbb;
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].G = 0xbb;
        screen_temp[(window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + i].B = 0xbb;
      }
    }
  }
}

void drawWindows()
{
  int k;
  WindowQueue *p = &windowQueue;

  cli();
  while (p->next != 0)
  {
    p = p->next;
    switchuvm(p->proc);
    if (p->window != 0)
      if (p->window->show == 1)
      {
        drawWindow(p->window);
        for (k = 0; k < p->window->widgetsNum; k++)
          switch (p->window->widgets[k].type)
          {
          case imageView:
            drawImageView(p->window->widgets[k].context.imageView);
            break;
          default:
            break;
          }
      }
  }
  switchuvm(proc);
  sti();
}

void removeMouse()
{
  int i, j, x, y;
  
  x  = mouse_info.last_draw_x;
  y = mouse_info.last_draw_y;
  for (j = 0; j < MOUSE_HEIGHT; j++)
    for (i = 0; i < MOUSE_WIDTH; i++)
      if (x + i <= SCREEN_WIDTH && y + j <= SCREEN_HEIGHT)
      {
        screen[(y + j) * SCREEN_WIDTH + x + i].R = screen_temp[(y + j) * SCREEN_WIDTH + x + i].R;
        screen[(y + j) * SCREEN_WIDTH + x + i].G = screen_temp[(y + j) * SCREEN_WIDTH + x + i].G;
        screen[(y + j) * SCREEN_WIDTH + x + i].B = screen_temp[(y + j) * SCREEN_WIDTH + x + i].B;
      }
}

void drawMouse()
{
  int i, j,x,y;
  
  removeMouse();
  x = mouse_info.x_position;
  y = mouse_info.y_position;
  if (mouse_info.event == MOUSE_DRAGGING)
  {
    for (j = 0; j < MOUSE_HEIGHT / 2; j++)
      for (i = 0; i < MOUSE_WIDTH; i++ )
       if (x + i <= SCREEN_WIDTH && y + j <= SCREEN_HEIGHT)
        {
          screen[(y + j) * SCREEN_WIDTH + x + i].R = 0xff;
          screen[(y + j) * SCREEN_WIDTH + x + i].G = 0xff;
          screen[(y + j) * SCREEN_WIDTH + x + i].B = 0xff;
        } 
  }
  else
  {
    for (j = 0; j < MOUSE_HEIGHT / 2; j++)
      for (i = 0; i < j; i++)
        if (x + i <= SCREEN_WIDTH && y + j <= SCREEN_HEIGHT)
        {
          screen[(y + j) * SCREEN_WIDTH + x + i].R = 0x00;
          screen[(y + j) * SCREEN_WIDTH + x + i].G = 0x00;
          screen[(y + j) * SCREEN_WIDTH + x + i].B = 0x00;
        }
    for (j = MOUSE_HEIGHT / 2; j < MOUSE_HEIGHT / 2 + MOUSE_HEIGHT / 4; j++)
      for (i = 0; i < (MOUSE_HEIGHT / 2 + MOUSE_HEIGHT / 4 - 1 - j) ; i++)
        if (x + i <= SCREEN_WIDTH && y + j <= SCREEN_HEIGHT)
        {
          screen[(y + j) * SCREEN_WIDTH + x + i].R = 0x00;
          screen[(y + j) * SCREEN_WIDTH + x + i].G = 0x00;
          screen[(y + j) * SCREEN_WIDTH + x + i].B = 0x00;
        } 
  }

  mouse_info.last_draw_x = mouse_info.x_position;
  mouse_info.last_draw_y = mouse_info.y_position;
}

void updateWindow()
{
  int totalPels = SCREEN_WIDTH * SCREEN_HEIGHT;
  
  memset(screen_temp, 0xff, sizeof(RGB) * totalPels);
  drawWindows();
  memmove(screen, screen_temp, sizeof(RGB) * totalPels);
  drawMouse();
}

void updateMouse()
{
  //int totalPels = SCREEN_WIDTH * SCREEN_HEIGHT;

  //memmove(screen, screen_temp, sizeof(RGB) * totalPels);
  drawMouse();
}

