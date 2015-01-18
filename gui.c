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
static struct RGB temp_color;

void initGUI()
{
  GUI_INFO = *((GUI_MODE_INFO *)(GUI_BUF << 4));
  screen = (RGB *)GUI_INFO.PhysBasePtr;
  screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);
}

int isAlpha(RGB color)
{
  if ((color.R == color.G) && (color.R == color.B) && (color.R == 0x00))
    return 1;
  else
    return 0;
}

void drawImageView(ImageView *imageView)
{
  int i, j;

  for (i = 0; i < imageView->width; i++)
    for (j = 0; j < imageView->height; j++)
      screen_temp[(imageView->leftTopY + j) * SCREEN_WIDTH + imageView->leftTopX + i] = imageView->image[(imageView->height - 1 - j) * imageView->width + i];
}

void drawIconView(IconView *iconView)
{
  int i, j;

  for (i = 0; i < iconView->width; i++)
    for (j = 0; j < iconView->height; j++)
      if (isAlpha(iconView->image[(iconView->height - 1 - j) * iconView->width + i]) == 0)
        screen_temp[(iconView->leftTopY + j) * SCREEN_WIDTH + iconView->leftTopX + i] = iconView->image[(iconView->height - 1 - j) * iconView->width + i];
}

void drawPoint(int x, int y)
{
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
    return;
  screen_temp[y* SCREEN_WIDTH + x].R = temp_color.R;
  screen_temp[y * SCREEN_WIDTH + x].G = temp_color.G;
  screen_temp[y * SCREEN_WIDTH + x].B = temp_color.B;
}

void setColor(int R, int G, int B)
{
  temp_color.R = R;
  temp_color.G = G;
  temp_color.B = B;
}

void drawWindow(Window *window)
{
  int i, j, k;

  for (i = 0; i < window->width; i++)
  {
    if (window->hasCaption == 1)
    {
      for (j = 0; j < CAPTION_HEIGHT; j++)
      {
        setColor(0x00, 0x00, 0x33);
        drawPoint(window->leftTopX+i, window->leftTopY + j);
      }
      for (j = CAPTION_HEIGHT; j < CAPTION_HEIGHT + MENU_HEIGHT; j++)
      {
        setColor(0xcc, 0xcc, 0xff);
        drawPoint(window->leftTopX+i, window->leftTopY + j);
      }
      for (j = CAPTION_HEIGHT + MENU_HEIGHT; j < CAPTION_HEIGHT + MENU_HEIGHT + 2; j++)
      {
        setColor(0x66, 0x66, 0x66);
        drawPoint(window->leftTopX+i, window->leftTopY + j);
      }
      for (j = CAPTION_HEIGHT + MENU_HEIGHT + 2; j < window->height; j++)
      {
        setColor(0xff, 0xff, 0xff);
        drawPoint(window->leftTopX+i, window->leftTopY + j);
      }
    }
    else
    {
      for (j = 0; j < window->height; j++)
      {
        setColor(0xbb, 0xbb, 0xbb);
        drawPoint(window->leftTopX+i, window->leftTopY + j);
      }
    }
  }
  //draw border
  for (i = 0; i < BORDER_WIDTH; i++)
    for (j = CAPTION_HEIGHT+MENU_HEIGHT; j <= window->height - BORDER_WIDTH; j++)
    {
      if (i == BORDER_WIDTH - 1)
        setColor(0x66, 0x66, 0x66);
      else
         setColor(0xcc, 0xcc, 0xcc);
      drawPoint(window->leftTopX+i, window->leftTopY + j);
    }
  for (i = window->width - BORDER_WIDTH; i < window->width; i++)
    for (j = CAPTION_HEIGHT + MENU_HEIGHT; j <= window->height - BORDER_WIDTH; j++)
    {
      if (i == window->width - BORDER_WIDTH)
        setColor(0x66, 0x66, 0x66);
      else
         setColor(0xcc, 0xcc, 0xcc);
      drawPoint(window->leftTopX+i, window->leftTopY + j);
    }  
  for (i = 0; i < window->width; i++)
    for (j = window->height - BORDER_WIDTH; j < window->height; j++)
    {
       if (j == window->height-BORDER_WIDTH)
       {
        if (i < BORDER_WIDTH || i > window->width - BORDER_WIDTH)
          continue;
        setColor(0x66, 0x66, 0x66);
       }
      else
        setColor(0xcc, 0xcc, 0xcc);
      drawPoint(window->leftTopX+i, window->leftTopY + j);
    }

  setColor(0xff, 0xff, 0xff);
  for (i = 0 ; i < window->width; i++)
  {
    j = 0;
    drawPoint(window->leftTopX+i, window->leftTopY + j);
    j = window->height - 1;
    drawPoint(window->leftTopX+i, window->leftTopY + j);
  }
  for (j = 0; j < window->height; j++)
  {
    i = 0;
    drawPoint(window->leftTopX+i, window->leftTopY + j);
    i = window->width - 1;
    drawPoint(window->leftTopX+i, window->leftTopY + j);
  }
  //draw close
  for (i = 0; i < 10; i++)
  {
    j = 10 + i;
    for (k = 0; k < 2; k++)
    {
      drawPoint(window->leftTopX + window->width - 30 + i + k, window->leftTopY + j);
      drawPoint(window->leftTopX + window->width - 21 - i + k, window->leftTopY + j);
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
          case iconView:
            drawIconView(p->window->widgets[k].context.iconView);
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

