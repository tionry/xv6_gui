#include "window.h"
#define MAX_WINDOW_NUM 256
#define SCREEN_MEMORY 0xfc000000

char*
mystrcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

Window windowQueue[MAX_WINDOW_NUM];
unsigned short screen_temp[SCREEN_WIDTH][SCREEN_HEIGHT];
int windowNum = 0;

void updateWindow()
{
	unsigned short *screen = (unsigned short *)SCREEN_MEMORY;
	int i, j, k;

	for (k = 0; k < windowNum; k++)
		if (windowQueue[k].show == 1)
			for (i = 0; i < windowQueue[k].width; i++)
				for (j = 0; j < windowQueue[k].height; j++)
					screen_temp[windowQueue[k].leftTopX + i][windowQueue[k].leftTopY + j] = 0x8888;

	for (i = 0; i < SCREEN_HEIGHT; i++)
		for (j = 0; j < SCREEN_WIDTH; j++)
			screen[SCREEN_WIDTH * i + j] = screen_temp[j][i];
}

Window* createWindow(int leftTopX, int leftTopY, int width, int height)
{
	if (windowNum == MAX_WINDOW_NUM)
		return (Window*)-1;

	windowQueue[windowNum].leftTopX = leftTopX;
	windowQueue[windowNum].leftTopY = leftTopY;
	windowQueue[windowNum].width = width;
	windowQueue[windowNum].height = height;
	mystrcpy(windowQueue[windowNum].caption, "new window");
	windowQueue[windowNum].show = 1;
	windowNum++;
	return &windowQueue[windowNum - 1];
}

