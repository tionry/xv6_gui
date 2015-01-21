#include "types.h"
#include "uwindow.h"
#include "stat.h"
#include "user.h"


static int text_length;
static TextBox text_box;
Window window;
static hwind;

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

static void closeTextBox();

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
  while(1)
  {
    handleEvent(&window);
  }
  exit();
}

static void closeTextBox()
{
  text_box.semoph = 0;
  close(fd);
  deleteWindow(hWind);
  exit();
}



