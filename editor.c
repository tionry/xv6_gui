#include "types.h"
#include "fcntl.h"
#include "uwindow.h"
#include "stat.h"
#include "user.h"

static TextBox text_box;

Window window;
static int hWind,fd;
RGB closeButtonImageViewTemp[100];
RGB saveButtonImageViewTemp[100];
ImageView closeButtonImageView;
ImageView saveButtonImageView;
Button saveFileButton;
TextBox fileNameBox;

void closeWindow(Widget *widget, Window *window);
void saveFile();

void cat(int fdi)
{
  int n;

  n = read(fdi, text_box.text, sizeof(text_box.text));
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
  text_box.cursor = n;
  text_box.textLength = n;
  //printf(1,"%d\n",n);
  //printf(1, "%s\n", text_box.text);
}

int main(int argc,char *argv[])
{
  memset(&window, 0, sizeof(Window));
  window.leftTopX = 300;
  window.leftTopY = 200;
  window.width = 600;
  window.height = 500;
  window.show = 1;
  window.hasCaption = 1;
  strcpy(window.caption, "TextEditor");
  saveFileButton.leftTopX = 280;
  saveFileButton.leftTopY = 420;
  saveFileButton.width = 40;
  saveFileButton.height = 30;
  strcpy(saveFileButton.text,"Save");

  addCloseButton(&window, &closeButtonImageView, closeButtonImageViewTemp);
  closeButtonImageView.onLeftClickHandler.handlerFunction = closeWindow;
  saveFileButton.onLeftClickHandler.handlerFunction = saveFile;
  
  printf(1,"%s\n",argv[0]);
  printf(1,"%s\n",argv[1]);

  memset(&text_box, 0, sizeof(TextBox));
  text_box.leftTopX = 0;
  text_box.leftTopY = 0;
  text_box.width = 580;
  text_box.height = 460;
  text_box.cursor = 0;
  text_box.semoph = 1;
  fileNameBox.width = 200;
  fileNameBox.height = 50;
  fileNameBox.leftTopX = 50;
  fileNameBox.leftTopY = 420;
  strcpy(fileNameBox.text, "NewFolderName");
  fileNameBox.cursor = 13;
  fileNameBox.textLength = 13;
  if (argv[1] != 0)
  {
    strcpy(window.caption, argv[1]);
    strcpy(fileNameBox.text, argv[1]);
    if((fd = open(argv[1], O_RDONLY)) < 0){
      printf(1, "cat: cannot open %s\n", argv[1]);
      exit();
    }
    cat(fd);
    close(fd);
  }
  else
  {
    text_box.text[0] = '\0';
    text_box.cursor = 0;
    text_box.textLength = 0;
  }
  
  text_box.leftTopX = 10;//(window.width >> 1) - (text_box.width >> 1);
  text_box.leftTopY = 30;//(window.height >> 1) - (text_box.height >> 1);
  window.widgets[window.widgetsNum].type = textBox;
  window.widgets[window.widgetsNum].context.textBox = &text_box;
  //printf(1, "length : %d\n", window.widgets[window.widgetsNum].context.textBox->textLength);
  window.widgetsNum++;
  window.widgets[window.widgetsNum].type = button;
  window.widgets[window.widgetsNum].context.button = &saveFileButton;
  window.widgetsNum++;
  window.widgets[window.widgetsNum].type = textBox;
  window.widgets[window.widgetsNum].context.textBox = &fileNameBox;
  window.widgetsNum++;
  
  hWind = createWindow(&window);
  while(1)
  {
    handleEvent(&window);
  }
  exit();
}

void closeWindow(Widget *widget, Window *window)
{
  deleteWindow(hWind);
  exit();
}

void saveFile()
{
  //file name :text1,text2...
  //file content text_box.content
  int fd = open(window.caption, O_WRONLY);
  
  if (fd < 0)
  {
    return;
  }

  printf(1,"save\n");
  write(fd, text_box.text, strlen(text_box.text));
  close(fd);
}

