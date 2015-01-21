#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"
#include "fs.h"
#include "bitmap.h"

Window window;
Button newFolderButton;
RGB closeButtonImageViewTemp[100];
ImageView closeButtonImageView;
IconView icon[50];
struct RGB folder[50][10000];
int hWind;

void closeWindow(Widget *widget, Window *window);
void newFolder(Widget *widget, Window *window);

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
//    icon[i].onLeftDoubleClickHandler.handlerFunction = iconOnLeftDoubleClick;
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
  }
}

int main(void)
{
  memset(&window, 0, sizeof(Window));
  window.leftTopX = 100;
  window.leftTopY = 100;
  window.width = 1000;
  window.height = 800;
  window.show = 1;
  window.hasCaption = 1;
  strcpy(window.caption, "Explorer");
  addCloseButton(&window, &closeButtonImageView, closeButtonImageViewTemp);
  closeButtonImageView.onLeftClickHandler.handlerFunction = closeWindow;
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
  ls(".");
  updateWindow();
  while (1) handleEvent(&window);
}

void closeWindow(Widget *widget, Window *window)
{
  deleteWindow(hWind);
  exit();
}

void newFolder(Widget *widget, Window *window)
{
  char *argv[] = { "mkdir", "aaa", 0 };

  if (fork() == 0)
  {
    exec(argv[0], argv);
    exit();
  }
  wait();
  window->widgetsNum = 2;
  updateWindow();
  ls(".");
  updateWindow();
}

