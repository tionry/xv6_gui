#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"

Window window;
int main(void)
{
  initWindow(&window);
  window.leftTopX = 100;
  window.leftTopY = 100;
  window.width = 600;
  window.height = 400;
  window.show = 1;
  window.hasCaption = 1;
  createWindow(&window);
  exit();
}

