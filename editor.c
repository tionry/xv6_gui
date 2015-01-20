#include "defs.h"
#include "gui.h"
#include "uwindow.h"
#include "stat.h"
#include "user.h"

static int text_length;
static TextBox text_box;
Window window;

void cat(int fd)
{
  int n;

  while((n = read(fd, text_box.text, sizeof(text_box.text))) > 0)
    write(1, text_box.text, n);
  if(n < 0){
    cprintf(1, "cat: read error\n");
    exit();
  }
}

int main(int argc,char *argv[])
{
  int fd;
  memset(&window, 0, sizeof(Window));
  window.leftTopX = 300;
  window.leftTopY = 200;
  window.width = 600;
  window.height = 500;
  window.show = 1;
  window.hasCaption = 1;
  strcpy(window.caption, "TextEditor");
  
  if (strcmp(argv[0], "texteditor") != 0)
  {
    initText();
    memset(&text_box, 0, sizeof(TextBox));
    if((fd = open(argv[1], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
      cat(fd);
      //close(fd);
	
    if ((text_box.width > window.width) || (text_box.height > window.height))
      exit();
    text_box.leftTopX = (window.width >> 1) - (text_box.width >> 1);
    text_box.leftTopY = (window.height >> 1) - (text_box.height >> 1);
    window.widgets[0].type = TextBox;
    window.widgets[0].context.textBox = &text_box;
    window.widgetsNum = 1;
  }
  createWindow(&window);
  while(1)
  {
    
  }
  close(fd);
  exit();
}
static int initText()
{
  text_box.leftTopx = 20;
  text_box.leftTopy = 20;
  text_box.width = 500;
  text_box.height = 400;
  text_box.text = '\0';
  text_box.cursor = 0;
  text_box.semoph = 1;
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



