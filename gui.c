#include "gui.h"

void initGUI()
{
	GUI_INFO = *((GUI_MODE_INFO *)(GUI_BUF << 4));
}

