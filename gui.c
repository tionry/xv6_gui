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

void drawLabel(Label *label)
{
  int i, j;
  RGB *t;

  for (j = 0; j < label->height; j++)
  {
    t = screen_temp + (label->leftTopY + j) * SCREEN_WIDTH + label->leftTopX;
    for (i = 0; i < label->width; i++)
    {
      t->R = 0xc8;
      t->G = 0xc8;
      t->B = 0xc8;
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
      t->R = 0xff;
      t->G = 0xff;
      t->B = 0xff;
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
      t->R = 0xc8;
      t->G = 0xc8;
      t->B = 0xc8;
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
}

int draw_character(unsigned int x, unsigned int y, char ch,struct RGB *color)
{
  //cprintf("hehe");
  int ord = ch - 0x20;
  if(ord > 94 || ord < 0)
    return -1;

  int i, j;
  int draw_flag = 1;
  i = 0;
  RGB *t;
  while(draw_flag == 1 && i < CHARACTER_WIDTH)
  { 
    for(j = 0; j < CHARACTER_HEIGHT; j++)
    {
      if(character[ord][j][i] == 0)
        continue;
      else
      {
        t = screen_temp + (y + j) * SCREEN_HEIGHT + x + i;
        t->R = color->R;
        t->G = color->G;
        t->B = color->B;
        cprintf("d");
      }
    }    
    i++;
  }
  return (i);
}

void drawCharacters(int x,int y, char *str,struct RGB *color)
{
  int i;
  int pos_x = 0, pos_y = 0;
  for(i = 0; i < strlen(str); ++i)
  {
    pos_x += draw_character(x + pos_x, y + pos_y, str[i], color);
  }
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
        t->R = 145;
        t->G = 210;
        t->B = 228;
        t++;
      }
    }
    for (j = CAPTION_HEIGHT; j < window->height - BORDER_WIDTH; j++)
    {
      t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
      for (i = 0; i < window->width; i++)
      {
        t->R = 0xff;
        t->G = 0xff;
        t->B = 0xff;
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
        t->R = 0xff;
        t->G = 0xff;
        t->B = 0xff;
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
      t->R = 145;
      t->G = 210;
      t->B = 228;
      t++;
    }
  }
  for (j = CAPTION_HEIGHT; j <= window->height - BORDER_WIDTH; j++)
  {
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + window->width - BORDER_WIDTH;
    for (i = window->width - BORDER_WIDTH; i < window->width; i++)
    {
      t->R = 145;
      t->G = 210;
      t->B = 228;
      t++;
    }
  }
  for (j = window->height - BORDER_WIDTH - 1; j < window->height; j++)
  {
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
    for (i = 0; i < window->width; i++)
    {
      t->R = 145;
      t->G = 210;
      t->B = 228;
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
    t->R = 0xcc;
    t->G = 0xcc;
    t->B = 0xcc;
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
    t->R = 0xcc;
    t->G = 0xcc;
    t->B = 0xcc;
    t++;
  }
  for (j = 0; j < window->height; j++)
  {
    if (j < 10 || window->height - j < 10)
      continue;
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX;
    t->R = 0xcc;
    t->G = 0xcc;
    t->B = 0xcc;
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + window->width - 1;
    t->R = 0xcc;
    t->G = 0xcc;
    t->B = 0xcc;
  }
  t = screen_temp + (window->leftTopY - 1) * SCREEN_WIDTH + window->leftTopX - 1;
  for (i = -1 ; i < window->width+1; i++)
  {
    if (i < 10 || window->width - i < 10)
    {
      t++;
      continue;
    }
    t->R = 0xff;
    t->G = 0xff;
    t->B = 0xff;
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
    t->R = 0xff;
    t->G = 0xff;
    t->B = 0xff;
    t++;
  }
  for (j = -1; j < window->height + 1; j++)
  {
    if (j < 10 || window->height - j < 10)
      continue;
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX - 1;
    t->R = 0xff;
    t->G = 0xff;
    t->B = 0xff;
    t = screen_temp + (window->leftTopY + j) * SCREEN_WIDTH + window->leftTopX + window->width;
    t->R = 0xff;
    t->G = 0xff;
    t->B = 0xff;
  }
  //draw close
  for (i = 0; i < 10; i++)
  {
    t = screen_temp + (window->leftTopY + 10 + i) * SCREEN_WIDTH + window->leftTopX + window->width - BORDER_WIDTH - 10 + i;
    for (k = 0; k < 2; k++)
    {
      t->R = 0x00;
      t->G = 0x00;
      t->B = 0x00;
      t++;
    }
    t = screen_temp + (window->leftTopY + 10 + i) * SCREEN_WIDTH + window->leftTopX + window->width - BORDER_WIDTH - 1 - i;
    for (k = 0; k < 2; k++)
    {
      t->R = 0x00;
      t->G = 0x00;
      t->B = 0x00;
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
            t->R = 0xff;
            t->G = 0xff;
            t->B = 0xff;
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
            {
              t->R = 0xff;
              t->G = 0xff;
              t->B = 0xff;
            }
            else
            {
              t->R = 0x00;
              t->G = 0x00;
              t->B = 0x00;
            }
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
            {
              t->R = 0xff;
              t->G = 0xff;
              t->B = 0xff;
            }
            else
            {
              t->R = 0x00;
              t->G = 0x00;
              t->B = 0x00;
            }
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

