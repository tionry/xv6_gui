#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"
#include "bitmap.h"

Window window;
ImageView image;

int main()
{
  initWindow(&window);
  window.leftTopX = 0;
  window.leftTopY = 0;
  window.width = 1280;
  window.height = 1024;
  window.show = 1;
  image.leftTopX = 100;
  image.leftTopY = 100;
  readBitmapFile("test.bmp", image.image, &image.width, &image.height);
  window.widgetsNum = 1;
  window.widgets[0].type = imageView;
  window.widgets[0].context.imageView = &image;
  createWindow(&window);
  exit();
}

