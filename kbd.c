#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "traps.h"
#include "x86.h"
#include "kbd.h"
#include "spinlock.h"
#include "gui.h"
#include "mouse.h"
#include "window.h"

static uint shift;

extern WindowQueue *lastWindow;

void insertCharacter(TextBox *textbox, char ch)
{
  int i;
  int pos = textbox->cursor;
  int len = textbox->textLength++;
  for (i = len; i > pos; i--)
  {
    textbox->text[i] = textbox->text[i - 1];
  }
  textbox->text[i] = ch;
  textbox->cursor++;
  textbox->textLength++;
  updateLastWindow();
}

void deleteCharacter(TextBox *textbox)
{
  if (textbox->textLength == 0)
    return;
  int i;
  int pos = textbox->cursor;
  int len = textbox->textLength;
  cprintf("cursor = %d, length = %d\n", pos, len);
  for (i = pos-1; i < len; i++)
  {
    cprintf("i = %d\n", i);
    textbox->text[i] = textbox->text[i+1];
  }
  textbox->cursor--;
  textbox->textLength--;
  updateLastWindow();
}

void keyboardHandler(char ch)
{
  int i;
  TextBox *textbox = 0;
  switchuvm(lastWindow->proc);
  for (i = 0; i < lastWindow->window->widgetsNum; i++)
  {
    if (lastWindow->window->widgets[i].type == textBox)
    {
      textbox = lastWindow->window->widgets[i].context.textBox;
      break;
    }
  }
  if (textbox)
  {
    if (ch < 225 && ch !=8)
    {
      insertCharacter(textbox, ch);
    }
    if (ch == 8)
      deleteCharacter(textbox);
  }
  if (proc == 0)
    switchkvm();
  else
    switchuvm(proc);
}

void kbdintr(void)
{
  static uchar *charcode[4] = {
    normalmap, shiftmap, ctlmap, ctlmap
  };
  uint st, data, c;

  st = inb(KBSTATP);
  if((st & KBS_DIB) == 0)
    return;
  data = inb(KBDATAP);

  if(data == 0xE0){
    shift |= E0ESC;
    return;
  } else if(data & 0x80){
    // Key released
    data = (shift & E0ESC ? data : data & 0x7F);
    shift &= ~(shiftcode[data] | E0ESC);
    return;
  } else if(shift & E0ESC){
    // Last character was an E0 escape; or with 0x80
    data|= 0x80;
    shift &= ~E0ESC;
  }

  shift |= shiftcode[data];
  shift ^= togglecode[data];
  c = charcode[shift & (CTL | SHIFT)][data];
  if(shift & CAPSLOCK){
    if('a' <= c && c <= 'z')
      c += 'A' - 'a';
    else if('A' <= c && c <= 'Z')
      c += 'a' - 'A';
  }
  if (c)
  {
    //cprintf("%d\n", c);
    keyboardHandler(c);
  }
}
