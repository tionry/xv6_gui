#include "types.h"
#include "defs.h"
#include "traps.h"
#include "x86.h"
#include "spinlock.h"
#include "gui.h"
#include "mouse.h"

struct spinlock mouse_lock;
struct mouseinfo mouse_info;
//origin info
static int state = 1;
static int left_down = 0;
static int right_down = 0;
static int x_sign = 0;
static int y_sign = 0;
static int x_overflow = 0;
static int y_overflow = 0;

void mouseinit()
{
	outb(0x64, 0xa8);
  	outb(0x64, 0xd4);
  	outb(0x60, 0xf4);
  	outb(0x64, 0x60);
  	outb(0x60, 0x47);
  	cprintf("mouse initialized.\n");
  	setMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
  	initlock(&mouse_lock, "mouse");
  	picenable(IRQ_MOUSE);
  	ioapicenable(IRQ_MOUSE, 0);
}

void moveMousePosition(int x, int y)
{
	setMousePosition(mouse_info.x_position + x, mouse_info.y_position + y);
}

void setMousePosition(int x, int y)
{
	if (x < 0)
		x = 0;
	if (x > SCREEN_WIDTH)
		x = SCREEN_WIDTH;
	if (y < 0)
		y = 0;
	if (y > SCREEN_HEIGHT)
		y = SCREEN_HEIGHT;
	mouse_info.x_position = x;
	mouse_info.y_position = y;
	updateMouse();
}

void mouseint(uint tick)
{
	uint ch;
  	int dis;

  	ch = inb(0x64);
  	if ((ch & 0x01) == 0)
  	{
    		//cprintf("no data\n");
    		state = 1;
    		return;
  	}

  	acquire(&mouse_lock);
  	ch = inb(0x60);

  	if (state == 1)
  	{
    		if ((ch & 0x08) == 0 || (ch & 0x04) != 0)
    		{
      			release(&mouse_lock);
      			return;
    		}
    		left_down = (ch & 0x01) ? 1 : 0;
    		right_down = (ch & 0x02) ? 1 : 0;
    		x_sign = (ch & 0x10) ? 1 : 0;
    		y_sign = (ch & 0x20) ? 1 : 0;
    		x_overflow = (ch & 0x40) ? 1 : 0;
    		y_overflow = (ch & 0x80) ? 1 : 0;
    		// cprintf("mouse_down: left_down=%d, right_down=%d\n", left_down, right_down);
    		// cprintf("move direction: x_sign=%d, y_sign=%d\n", x_sign, y_sign);
    		// cprintf("overflow: x_overflow = %d, y_overflow = %d\n",x_overflow,  y_overflow);
    		state = 2;
    		release(&mouse_lock);
    		return;
  	}
  	else if (state == 2)
  	{
    		dis = ch;
    		if (x_sign == 1)
      			dis = ch - 256;
    		//cprintf("x_movement: %d\n", dis);
    		state = 3;
    		release(&mouse_lock);
    		moveMousePosition(dis, 0);
    		return;
	}
  	else if (state == 3)
  	{
    		dis = ch;
    		if (y_sign == 1)
      		dis = ch - 256;
    		dis = - dis;
    		//cprintf("y_movement: %d\n", dis);
    		state = 1;
    		release(&mouse_lock);
    		moveMousePosition(0, dis);
    		cprintf("x: %d, y: %d\n", mouse_info.x_position, mouse_info.y_position);
    		return;
  	}
}
