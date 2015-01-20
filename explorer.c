#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"

Window window;
ImageView image;
RGB temp[100];

int main(void)
{
  int i, j;
  memset(&window, 0, sizeof(Window));
  window.leftTopX = 100;
  window.leftTopY = 100;
  window.width = 600;
  window.height = 400;
  window.show = 1;
  window.hasCaption = 1;
  window.widgetsNum = 0;

  //add "X"
  RGB *t;
  image.leftTopX = window.leftTopX+window.width - 20;
  image.leftTopY = window.leftTopY+10;
  image.width = 10;
  image.height = 10;
  image.image = temp;
  t = temp;
  for (j = 0; j < image.height; j++)
    for (i = 0; i < image.width; i++)
    {
      if ( i >=2 && i <= 7 && (i == j  || (i+j) == 9))
      {
        t->R = 0x00;
        t->G = 0x00;
        t->B = 0x00;
      }
      else
      {
        t->R = 0xff;
        t->G = 0xff;
        t->B = 0xff;
      }
      t++;
    }
  window.widgets[window.widgetsNum].type = imageView;
  window.widgets[window.widgetsNum].context.imageView = &image;  
  window.widgetsNum++;

  strcpy(window.caption, "Explorer");
  createWindow(&window);
  exit();
}

