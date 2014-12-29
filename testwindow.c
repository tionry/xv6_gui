#include "types.h"
#include "stat.h"
#include "user.h"
#include "uwindow.h"

int main()
{
  Window window;
  initWindow(&window);
  window.leftTopX = 0;
  window.leftTopY = 0;
  window.width = 40;
  window.height = 30;
  window.show = 1;
  createWindow(&window);
  exit();
}

