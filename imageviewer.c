#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"
#include "bitmap.h"

Window window;
ImageView image;
struct RGB temp[1310720];

int main(int argc, char *argv[])
{
  memset(&window, 0, sizeof(Window));
  window.leftTopX = 300;
  window.leftTopY = 200;
  window.show = 1;
  window.hasCaption = 1;
  strcpy(window.caption, "ImageViewer");
  memset(&image, 0, sizeof(ImageView));
  image.leftTopX = window.leftTopX + 10;
  image.leftTopY = window.leftTopY + CAPTION_HEIGHT + 10;
  image.image = temp;
  readBitmapFile(argv[0], image.image, &image.height, &image.width);
  window.widgets[0].type = imageView;
  window.widgets[0].context.imageView = &image;
  window.widgetsNum = 1;
  window.width = image.width + 20;
  window.height = CAPTION_HEIGHT + image.height + 20;
  createWindow(&window);
  exit();
}

