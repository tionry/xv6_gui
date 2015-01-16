#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"
#include "bitmap.h"

Window window;
ImageView image;
struct RGB temp[1310720];

int main()
{
  initWindow(&window);
  window.leftTopX = 0;
  window.leftTopY = 0;
  window.width = 1280;
  window.height = 1024;
  window.show = 1;
  window.hasCaption = 0;
  image.leftTopX = 0;
  image.leftTopY = 0;
  image.image = temp;
  readBitmapFile("desktop_background.bmp", image.image, &image.height, &image.width);
  window.widgetsNum = 1;
  window.widgets[0].type = imageView;
  window.widgets[0].context.imageView = &image;
  createWindow(&window);
  exit();
}

