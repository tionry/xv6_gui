#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "gui.h"
#include "uwindow.h"


static int cur_cursor;
static int text_length;
static TextBox text_box;

static int initText(int x,int y)
{
  text_box.leftTopx = 100;
  text_box.leftTopy = 100;
  text_box.width = 1080;
  text_box.height = 720;
  text_box.text = '\0';
  cur_cursor = 0;
}

static int saveText(char *name)
{
  
}

void drawCursor()
{
  
}

static int deleteChar(int pos)
{
  if(pos < 0 || pos >= MAX_STRING_NUM)
    return -1;
  if(pos != cur_cursor || pos != cur_cursor + 1)
    return -2;
  int i;
  for(i = pos;i < MAX_STRING_NUM - 1;i++)
  {
    if(text_box.text[i] == '\0')
      break;
    text_box.text[i] = text_box.text[i + 1];
  }
  text_length --;
  updateLastWindow();
}

static int insertCharacter(int pos,char ch)
{
  if(pos < 0 || pos >= MAX_STRING_NUM)
    return -1;
  if(pos != cur_cursor)
    return -2;
  int i;
  for(i = text_length - 1;i > pos; i--)
  {
    text_box.text[i] = text_box.text[i - 1];
  }
  text_box.text[i] = ch;
  text_length ++;
  updateLastWindow();
}



