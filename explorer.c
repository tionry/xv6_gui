#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"

Window window;
Button newFolderButton;
RGB closeButtonImageViewTemp[100];
ImageView closeButtonImageView;
int hWind;

void closeWindow(Widget *widget, Window *window);
void newFolder(Widget *widget, Window *window);

int main(void)
{
  memset(&window, 0, sizeof(Window));
  window.leftTopX = 100;
  window.leftTopY = 100;
  window.width = 600;
  window.height = 400;
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
  while (1) handleEvent(&window);
}

void closeWindow(Widget *widget, Window *window)
{
  deleteWindow(hWind);
  exit();
}

void newFolder(Widget *widget, Window *window)
{
printf(1, "newFolder\n");
}

