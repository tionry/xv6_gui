#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"
#include "bitmap.h"

Window window;
ImageView image;
struct RGB temp[10001];

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
  image.image = temp;
  readBitmapFile("test.bmp", image.image, &image.height, &image.width);
  window.widgetsNum = 1;
  window.widgets[0].type = imageView;
  window.widgets[0].context.imageView = &image;
  createWindow(&window);
  exit();
}

