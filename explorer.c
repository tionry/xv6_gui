#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"
#include "fs.h"
#include "bitmap.h"

Window window;
TextBox filenameBox;
Button newFolderButton;
RGB closeButtonImageViewTemp[100];
ImageView closeButtonImageView;
IconView icon[50];
struct RGB folder[50][10000];
char wd[256];
char dir[256];
int hWind;

void closeWindow(Widget *widget, Window *window);
void newFolder(Widget *widget, Window *window);
void refresh(Widget *widget, Window *window);

//Result: T_DIR || T_FILE || T_DEV, define in stat.h
//-1: error occured.
int
getiNodeType(char *path)
{
  struct stat st;
  
  if (stat(path, &st) < 0)
  {
    return -1;
  }
  
  return st.type;
}

char*
fmtname(char *path)
{
  char *p;
  
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void suffix(char *t, char *s)
{
  int point = 0;

  while (*s != 0)
  {
    if (*s == '.')
      point = 1;
    s++;
  }
  if (point == 0)
  {
    strcpy(t, "");
    return;
  }
  while (*s != '.')
    s--;
  s++;
  strcpy(t, s);
}

void iconOnLeftDoubleClick(Widget *widget, Window *window)
{
  char *s = widget->context.iconView->text;
  char *argv1[] = { s, 0 };
  char *argv2[] = { "", s, 0};
  char t[256];

  if (getiNodeType(s) == T_DIR)
  {
    strcat(wd, "/");
    strcat(wd, s);
    strcat(window->caption, s);
    strcat(window->caption, "/");
    strcat(dir, s);
    strcat(dir, "/");
    refresh(window->widgets, window);
    return;
  }
  suffix(t, s);
  if (strcmp(t, "") == 0)
  {
    if (fork() == 0)
    {
      exec(argv1[0], argv1);
      exit();
    }
  }
  else
    if (strcmp(t, "bmp") == 0)
    {
      if (fork() == 0)
      {
        strcpy(argv2[0], "imageviewer");
        exec(argv2[0], argv2);
        exit();
      }
    }
    else
    if (strcmp(t, "txt") == 0)
    {
      if (fork() == 0)
      {
        strcpy(argv2[0], "editor");
        exec(argv2[0], argv2);
        exit();
      }
    }
}

void
ls(char *path)
{
  char buf[512], *p, *tmpName;
  int fd;
  struct dirent de;
  struct stat st;
  int i = 0;

  if ((fd = open(path, 0)) < 0) 
  {
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    return;
  }
  
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';
  while (read(fd, &de, sizeof(de)) == sizeof(de))
  {
    if (de.inum == 0)
    {
      continue;
    }
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if (stat(buf, &st) < 0)
    {
      continue;
    }
    tmpName = fmtname(buf);
    if (strcmp(tmpName, ".") == 0 || strcmp(tmpName, "..") == 0 || st.type == T_DEV)
    {
      continue;
    }
    memset(icon + i, 0, sizeof(IconView));
    icon[i].leftTopX = 50 + (i % 6) * 140;
    icon[i].leftTopY = 50 + (i / 6) * 140;
    icon[i].image = folder[i];
    icon[i].onLeftDoubleClickHandler.handlerFunction = iconOnLeftDoubleClick;
    switch (st.type)
    {
      case T_DIR:
        readBitmapFile("folder.bmp", icon[i].image, &icon[i].height, &icon[i].width);
        break;
      case T_FILE:
        readBitmapFile("file.bmp", icon[i].image, &icon[i].height, &icon[i].width);
        break;
    }
    strcpy(icon[i].text, tmpName);
    window.widgets[window.widgetsNum].type = iconView;
    window.widgets[window.widgetsNum].context.iconView = &icon[i];
    i++;
    window.widgetsNum++;
    if (i % 2 == 0) updateWindow();
  }
}

int main(int argc, char *argv[])
{
  memset(&window, 0, sizeof(Window));
  window.leftTopX = 100;
  window.leftTopY = 100;
  window.width = 900;
  window.height = 800;
  window.show = 1;
  window.hasCaption = 1;
  strcpy(window.caption, "Explorer");
  window.onFileSystemChangedHandler.handlerFunction = refresh;
  addCloseButton(&window, &closeButtonImageView, closeButtonImageViewTemp);
  closeButtonImageView.onLeftClickHandler.handlerFunction = closeWindow;
  strcpy(wd, ".");
  strcpy(dir, ":/");
  if (argv[1] != 0)
  {
    strcpy(wd, argv[1]);
    strcat(dir, argv[1]);
  }
  strcat(window.caption, dir);
  filenameBox.width = 200;
  filenameBox.height = 50;
  filenameBox.leftTopX = 20;
  filenameBox.leftTopY = window.height - BORDER_WIDTH - filenameBox.height - 10;
  strcpy(filenameBox.text, "NewFolderName");
  filenameBox.cursor = 13;
  filenameBox.textLength = 13;
  window.widgets[window.widgetsNum].type = textBox;
  window.widgets[window.widgetsNum].context.textBox = &filenameBox;
  window.widgetsNum++;
  newFolderButton.width = 100;
  newFolderButton.height = 50;
  newFolderButton.leftTopX = (window.width >> 1) - (newFolderButton.width >> 1);
  newFolderButton.leftTopY = window.height - BORDER_WIDTH - newFolderButton.height - 10;
  strcpy(newFolderButton.text, "New Folder");
  newFolderButton.onLeftClickHandler.handlerFunction = newFolder;
  window.widgets[window.widgetsNum].type = button;
  window.widgets[window.widgetsNum].context.button = &newFolderButton;
  window.widgetsNum++;
  hWind = createWindow(&window);
  ls(wd);
  updateWindow();
  while (1) handleEvent(&window);
}

void closeWindow(Widget *widget, Window *window)
{
  wait();
  deleteWindow(hWind);
  exit();
}

void newFolder(Widget *widget, Window *window)
{
  char s[256];
  strcpy(s, wd);
  strcat(s, "/");
  strcat(s, filenameBox.text);
  mkdir(s);
  fileSystemChanged();
}

void refresh(Widget *widget, Window *window)
{
  window->widgetsNum = 3;
  updateWindow();
  ls(wd);
  updateWindow();
}

