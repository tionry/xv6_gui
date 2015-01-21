#include "types.h"
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
    printf(1, "cat: read error\n");
    exit();
  }
}

static void initText();
static void closeTextBox();
static int insertCharacter(int pos,char ch);
static int deleteCharacter(int pos);

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
      printf(1, "cat: cannot open %s\n", argv[1]);
      exit();
    }
      cat(fd);
      //close(fd);
	
    if ((text_box.width > window.width) || (text_box.height > window.height))
      exit();
    text_box.leftTopX = (window.width >> 1) - (text_box.width >> 1);
    text_box.leftTopY = (window.height >> 1) - (text_box.height >> 1);
    window.widgets[0].type = textBox;
    window.widgets[0].context.textBox = &text_box;
    window.widgetsNum = 1;
  }
  createWindow(&window);
  while(text_box.semoph)
  {
    if(1==0)
    {
      insertCharacter(0,'a');
      deleteCharacter(0);
    }
  }
  close(fd);
  closeTextBox();
  exit();
}
static void initText()
{
  text_box.leftTopX = 20;
  text_box.leftTopY = 20;
  text_box.width = 500;
  text_box.height = 400;
  text_box.text[0] = '\0';
  text_box.cursor = 0;
  text_box.semoph = 1;
}

static void closeTextBox()
{
  text_box.semoph = 0;
}
static int deleteCharacter(int pos)
{
  if(pos < 0 || pos >= MAX_STRING_NUM)
    return -1;
  if(pos != text_box.cursor || pos != text_box.cursor + 1)
    return -2;
  int i;
  for(i = pos;i < MAX_STRING_NUM - 1;i++)
  {
    if(text_box.text[i] == '\0')
      break;
    text_box.text[i] = text_box.text[i + 1];
  }
  if(pos == text_box.cursor)
    text_box.cursor --;
  text_length --;
  window.widgets[0].context.textBox = &text_box;
  createWindow(&window);
  return 0;
}

static int insertCharacter(int pos,char ch)
{
  if(pos < 0 || pos >= MAX_STRING_NUM)
    return -1;
  if(pos != text_box.cursor)
    return -2;
  int i;
  for(i = text_length - 1;i > pos; i--)
  {
    text_box.text[i] = text_box.text[i - 1];
  }
  text_box.text[i] = ch;
  text_length ++;
  text_box.cursor ++;
  window.widgets[0].context.textBox = &text_box;
  createWindow(&window);
  return 0;
}



