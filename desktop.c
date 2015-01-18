#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"
#include "bitmap.h"

Window window;
ImageView image;
IconView icon[5];
struct RGB temp[1310720];
struct RGB folder[5][10000];

int main(void)
{
  int i;

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
  window.widgets[0].type = imageView;
  window.widgets[0].context.imageView = &image;
  for (i = 0; i < 3; i++)
  {
    icon[i].leftTopX = 50;
    icon[i].leftTopY = 50 + i * 120;
    icon[i].image = folder[i];
    readBitmapFile("folder.bmp", icon[i].image, &icon[i].height, &icon[i].width);
    window.widgets[i + 1].type = iconView;
    window.widgets[i + 1].context.iconView = &icon[i];
  }
  for (i = 3; i < 5; i++)
  {
    icon[i].leftTopX = 50;
    icon[i].leftTopY = 50 + i * 120;
    icon[i].image = folder[i];
    readBitmapFile("file.bmp", icon[i].image, &icon[i].height, &icon[i].width);
    window.widgets[i + 1].type = iconView;
    window.widgets[i + 1].context.iconView = &icon[i];
  }
  window.widgetsNum = 6;
  createWindow(&window);
  while (1) ;
}

