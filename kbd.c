#include "types.h"
#include "x86.h"
#include "defs.h"
#include "kbd.h"
#include "window.h"
#include "gui.h"

static uint shift;

extern WindowQueue *lastWindow;
static TextBox *textbox;

void insertCharacter(char ch)
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
}

void deleteCharacter()
{
  int i;
  int pos = textbox->cursor;
  int len = textbox->textLength;
  for (i = pos; i < len - 1; i++)
  {
    textbox->text[i-1] = textbox->text[i];
  }
  textbox->cursor--;
  textbox->textLength--;
}

void keyboardHandler(char ch)
{
  int i;
  Window *window = lastWindow->window;
  textbox = 0;
  for (i = 0; i < window->widgetsNum; i++)
    if (window->widgets[i].type == textBox)
    {
      if (window->widgets[i].context.textBox->fixed)
      {
        textbox = window->widgets[i].context.textBox;
        break;
      }
    }
  if (textbox)
  {
    if (ch < 225 && ch !=9)
      insertCharacter(ch);
    if (ch == 9)
      deleteCharacter();
  }
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
    cprintf("%d\n", c);
    //keyboardHandler(c);
  }
}
