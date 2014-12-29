#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"

int main()
{
  Window window;
  initWindow(&window);
  window.leftTopX = 200;
  window.leftTopY = 100;
  window.width = 800;
  window.height = 600;
  window.show = 1;
  createWindow(&window);
  int i;
  for (i = 0; i < 50000000; i++)
    ;
  deleteWindow(&window);
  exit();
}

