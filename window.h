typedef struct Window
{
	int leftTopX, leftTopY, width, height, show;
	char caption[256];
} Window;

Window* createWindow(int leftTopX, int leftTopY, int width, int height);
void updateWindow();
