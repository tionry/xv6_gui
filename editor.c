#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"
#include "gui.h"
#include "window.h"

#define text_max_length 4000
#define text_width 1080
#define text_height 720

static int cur_cursor;
static int text_length;
static char text_content[text_max_length];
static pos_x = 100;
static pos_y = 100;
static RGB backgroud;

static int initText(int x,int y)
{
  background.R = 0x00;
  background.G = 0x00;
  background.B = 0x00;
  text_content[0] = '\0';
  text_length = 0;
}

static int saveText(char *name)
{
  
}

static int deleteChar(int pos)
{
  if(pos < 0 || pos >= text_max_length)
    return -1;
  if(pos != cur_cursor || pos != cur_cursor + 1)
    return -2;
  int i;
  for(i = pos;i < text_max_length - 1;i++)
  {
    if(text_content[i] == '\0')
      break;
    text_content[i] = text_content[i + 1];
  }
}

static int insertCharacter(int pos,char ch)
{
  if(pos < 0 || pos >= text_max_length)
    return -1;
  if(pos != cur_cursor)
    return -2;
  int i;
  for(i = text_length;i > pos; i--)
  {
    text_content[i] = text_content[i - 1];
  }
  text_content[i] = ch;
  text_length ++;
}

int redrawSuffixString(int pos)
{

}
int insertCharacter(char ch,int pos)
{
  
}

int deleteCharacter(int pos)
{

}


