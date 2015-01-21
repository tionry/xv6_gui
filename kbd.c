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

void deleteCharacter(TextBox *textbox, unsigned char ch)
{
  int i;//, lineIndex;
  if (textbox->textLength == 0  || textbox->cursor == 0)
    return;
  if (ch == 228)
  {
    textbox->cursor--;
    updateLastWindow();
    return;
  }
  if (ch == 229)
  {
    if (textbox->text[textbox->cursor] != '\0' && textbox->text[textbox->cursor] != '\n')
    {
      textbox->cursor++;
      updateLastWindow();
    }
    return;
  }
  if (ch == 226)
  {
    // lineIndex = 0;
    // for (i = textbox->cursor-1; i >=0; i--)
    // {
    //   if (textbox->text[i] != '\n')
    //   {
    //     lineIndex++;
    //   }
    //   else
    //   {
    //     break;
    //   }
    // }
    // if (i == -1)
    //   break;
  }
  int pos = textbox->cursor;
  int len = textbox->textLength;
  for (i = pos-1; i < len; i++)
  {
    textbox->text[i] = textbox->text[i+1];
  }
  textbox->cursor--;
  textbox->textLength--;
  updateLastWindow();
}

void keyboardHandler(unsigned char ch)
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
  //cprintf("%d\n", ch);
  if (textbox)
  {
    if (ch < 225 && ch !=8)
    {
      insertCharacter(textbox, ch);
    }
    else
    if (ch == 8|| ch == 228 || ch == 229)
      deleteCharacter(textbox, ch);

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
