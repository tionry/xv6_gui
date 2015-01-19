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
#include "character.h"

GUI_MODE_INFO GUI_INFO;
extern WindowQueue windowQueue;
extern mouseinfo mouse_info;
RGB *screen, *screen_temp;

void initGUI()
{
  GUI_INFO = *((GUI_MODE_INFO *)(GUI_BUF << 4));
  screen = (RGB *)GUI_INFO.PhysBasePtr;
  screen_temp = (RGB *)(GUI_INFO.PhysBasePtr + 0x3c0000);
}

inline int isAlpha(RGB *color)
{
  if ((color->R == color->G) && (color->R == color->B) && (color->R == 0x00))
    return 1;
  else
    return 0;
}

void drawPoint(RGB* color, unsigned char R, unsigned char G, unsigned char B)
{
  color->R = R;
  color->G = G;
  color->B = B;
}

int drawCharacter(int x, int y, char ch, unsigned char R, unsigned char G, unsigned char B)
{
  int i, j;
  RGB *t;
  int ord = ch - 0x20;

  if(ord > 94 || ord < 0)
    return -1;
  for (j = 0; j < CHARACTER_HEIGHT; j++)
  {
    t = screen_temp + (y + j) * SCREEN_WIDTH + x;
    for (i = 0; i < CHARACTER_WIDTH; i++)
    {
      if (character[ord][j][i] == 1)
        drawPoint(t, R, G, B);
      t++;
    }
  }
  return (i);
}

void drawCharacters(int x, int y, char *str, unsigned char R, unsigned char G, unsigned char B)
{
  int pos_x = 0;

  while (*str != '\0')
  {
    pos_x += drawCharacter(x + pos_x, y, *str, R, G, B);
    str++;
  }
}

void drawLabel(Label *label)
{
  int i, j;
  RGB *t;

  for (j = 0; j < label->height; j++)
  {
    t = screen_temp + (label->leftTopY + j) * SCREEN_WIDTH + label->leftTopX;
    for (i = 0; i < label->width; i++)
    {
      drawPoint(t, 0xc8, 0xc8, 0xc8);
      t++;
    }
  }
}

void drawTextBox(TextBox *textBox)
{
  int i, j;
  RGB *t;

  for (j = 0; j < textBox->height; j++)
  {
    t = screen_temp + (textBox->leftTopY + j) * SCREEN_WIDTH + textBox->leftTopX;
    for (i = 0; i < textBox->width; i++)
    {
      drawPoint(t, 0xff, 0xff, 0xff);
      t++;
    }
  }
}

void drawButton(Button *button)
{
  int i, j;
  RGB *t;

  for (j = 0; j < button->height; j++)
  {
    t = screen_temp + (button->leftTopY + j) * SCREEN_WIDTH + button->leftTopX;
    for (i = 0; i < button->width; i++)
    {
      drawPoint(t, 0xc8, 0xc8, 0xc8);
      t++;
    }
  }
}

void drawImageView(ImageView *imageView)
{
  int i, j;
  RGB *t1, *t2;

  for (j = 0; j < imageView->height; j++)
  {
    t1 = screen_temp + (imageView->leftTopY + j) * SCREEN_WIDTH + imageView->leftTopX;
    t2 = imageView->image + (imageView->height - 1 - j) * imageView->width;
    for (i = 0; i < imageView->width; i++)
    {
      *t1 = *t2;
      t1++;
      t2++;
    }
  }
}

void drawIconView(IconView *iconView)
{
  int i, j;
  RGB *t1, *t2;

  for (j = 0; j < iconView->height; j++)
  {
    t1 = screen_temp + (iconView->leftTopY + j) * SCREEN_WIDTH + iconView->leftTopX;
    t2 = iconView->image + (iconView->height - 1 - j) * iconView->width;
    for (i = 0; i < iconView->width; i++)
    {
      if (isAlpha(t2) == 0)
        *t1 = *t2;
      t1++;
      t2++;
    }
  }
  drawCharacters(iconView->leftTopX + 10, iconView->leftTopY + iconView->height + 5, iconView->text, 0, 0, 0);
}

void drawWindow(Window *window)
{
  int i, j, k;
  RGB *t;
  
  if (window->hasCaption == 1)
  {
    for (j = 0; j < CAPTION_HEIGHT; j++)
    {
      t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
      for (i = 0; i < window->width; i++)
      {
        drawPoint(t, 145, 210, 228);
        t++;
      }
    }
    drawCharacters(window->leftTopX + 10, window->leftTopY + 5, window->caption, 0, 0, 0);
    for (j = CAPTION_HEIGHT; j < window->height - BORDER_WIDTH; j++)
    {
      t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
      for (i = 0; i < window->width; i++)
      {
        drawPoint(t, 0xff, 0xff, 0xff);
        t++;
      }
    }
  }
  else
  {
    for (j = 0; j < window->height; j++)
    {
      t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
      for (i = 0; i < window->width; i++)
      {
        drawPoint(t, 0xff, 0xff, 0xff);
        t++;
      }
    }
  }
  //draw border
  for (j = CAPTION_HEIGHT; j <= window->height - BORDER_WIDTH; j++)
  {
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
    for (i = 0; i < BORDER_WIDTH; i++)
    {
      drawPoint(t, 145, 210, 228);
      t++;
    }
  }
  for (j = CAPTION_HEIGHT; j <= window->height - BORDER_WIDTH; j++)
  {
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + window->width - BORDER_WIDTH;
    for (i = window->width - BORDER_WIDTH; i < window->width; i++)
    {
      drawPoint(t, 145, 210, 228);
      t++;
    }
  }
  for (j = window->height - BORDER_WIDTH - 1; j < window->height; j++)
  {
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
    for (i = 0; i < window->width; i++)
    {
      drawPoint(t, 145, 210, 228);
      t++;
    }
  }
  //draw shadow
  t = screen_temp + window->leftTopY * SCREEN_WIDTH + window->leftTopX;
  for (i = 0 ; i < window->width; i++)
  {
    if (i < 10 || window->width - i < 10)
    {
      t++;
      continue;
    }
    drawPoint(t, 0xcc, 0xcc, 0xcc);
    t++;
  }
  t = screen_temp + (window->leftTopY + window->height - 1) * SCREEN_WIDTH + window->leftTopX;
  for (i = 0 ; i < window->width; i++)
  {
    if (i < 10 || window->width - i < 10)
    {
      t++;
      continue;
    }
    drawPoint(t, 0xcc, 0xcc, 0xcc);
    t++;
  }
  for (j = 0; j < window->height; j++)
  {
    if (j < 10 || window->height - j < 10)
      continue;
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
    drawPoint(t, 0xcc, 0xcc, 0xcc);
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + window->width - 1;
    drawPoint(t, 0xcc, 0xcc, 0xcc);
  }
  t = screen_temp + (window->leftTopY - 1) * SCREEN_WIDTH + window->leftTopX - 1;
  for (i = -1 ; i < window->width+1; i++)
  {
    if (i < 10 || window->width - i < 10)
    {
      t++;
      continue;
    }
    drawPoint(t, 0xff, 0xff, 0xff);
    t++;
  }
  t = screen_temp + (window->leftTopY + window->height) * SCREEN_WIDTH + window->leftTopX - 1;
  for (i = -1 ; i < window->width+1; i++)
  {
    if (i < 10 || window->width - i < 10)
    {
      t++;
      continue;
    }
    drawPoint(t, 0xff, 0xff, 0xff);
    t++;
  }
  for (j = -1; j < window->height + 1; j++)
  {
    if (j < 10 || window->height - j < 10)
      continue;
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX - 1;
    drawPoint(t, 0xff, 0xff, 0xff);
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + window->width;
    drawPoint(t, 0xff, 0xff, 0xff);
  }
  //draw close
  for (i = 0; i < 10; i++)
  {
    t = screen_temp + (window->leftTopY + 10 + i) * SCREEN_WIDTH + window->leftTopX + window->width - BORDER_WIDTH - 10 + i;
    for (k = 0; k < 2; k++)
    {
      drawPoint(t, 0x00, 0x00, 0x00);
      t++;
    }
    t = screen_temp + (window->leftTopY + 10 + i) * SCREEN_WIDTH + window->leftTopX + window->width - BORDER_WIDTH - 1 - i;
    for (k = 0; k < 2; k++)
    {
      drawPoint(t, 0x00, 0x00, 0x00);
      t++;
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
          case label:
            drawLabel(p->window->widgets[k].context.label);
            break;
          case textBox:
            drawTextBox(p->window->widgets[k].context.textBox);
            break;
          case button:
            drawButton(p->window->widgets[k].context.button);
            break;
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
  RGB *t1, *t2;
    
  x = mouse_info.last_draw_x;
  y = mouse_info.last_draw_y;
  for (j = 0; j < MOUSE_HEIGHT; j++)
    if (y + j >= SCREEN_HEIGHT)
      break;
    else
    {
      t1 = screen + (y + j) * SCREEN_WIDTH + x;
      t2 = screen_temp + (y + j) * SCREEN_WIDTH + x;
      for (i = 0; i < MOUSE_WIDTH; i++)
        if (x + i >= SCREEN_WIDTH)
          break;
        else
        {
          *t1 = *t2;
          t1++;
          t2++;
        }
    }
}

void drawMouse()
{
  int i, j, x, y;
  RGB *t;
  
  x = mouse_info.x_position;
  y = mouse_info.y_position;
  if (mouse_info.event == MOUSE_DRAGGING)
  {
    for (j = 0; j < MOUSE_HEIGHT / 2; j++)
      if (y + j >= SCREEN_HEIGHT)
        break;
      else
      {
        t = screen + (y + j) * SCREEN_WIDTH + x;
        for (i = 0; i < MOUSE_WIDTH; i++)
          if (x + i >= SCREEN_WIDTH)
            break;
          else
          {
            drawPoint(t, 0xff, 0xff, 0xff);
            t++;
          }
      } 
  }
  else
  {
    for (j = 0; j < MOUSE_HEIGHT / 2; j++)
      if (y + j >= SCREEN_HEIGHT)
        break;
      else
      {
        t = screen + (y + j) * SCREEN_WIDTH + x;
        for (i = 0; i < j; i++)
          if (x + i >= SCREEN_WIDTH)
            break;
          else
          {
            if (i == 0 || i == j-1 || (j == MOUSE_HEIGHT / 2 - 1 && i >= MOUSE_WIDTH / 2))
              drawPoint(t, 0xff, 0xff, 0xff);
            else
              drawPoint(t, 0xff, 0xff, 0xff);
            t++;
          }   
      }
    for (j = MOUSE_HEIGHT / 2; j < MOUSE_HEIGHT / 2 + MOUSE_HEIGHT / 4; j++)
      if (y + j >= SCREEN_HEIGHT)
        break;
      else
      {
        t = screen + (y + j) * SCREEN_WIDTH + x;
        for (i = 0; i < (MOUSE_WIDTH + MOUSE_WIDTH / 2 - 1 - j) ; i++)
          if (x + i >= SCREEN_WIDTH)
            break;
          else
          {
            if (i == 0 || i == MOUSE_WIDTH + MOUSE_WIDTH / 2 - 2 - j)
              drawPoint(t, 0xff, 0xff, 0xff);
            else
              drawPoint(t, 0xff, 0xff, 0xff);
            t++;
          }
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
  removeMouse();
  drawMouse();
}

