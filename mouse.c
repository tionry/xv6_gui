#include "types.h"
#include "defs.h"
#include "traps.h"
#include "x86.h"
#include "spinlock.h"
#include "gui.h"
#include "mouse.h"

struct spinlock mouse_lock;
struct mouseinfo mouse_info;
static int to_read = 1;
static int left_down = 0;
static int right_down = 0;
static int x_sign = 0;
static int y_sign = 0;
static int x_overflow = 0;
static int y_overflow = 0;
//static int counter = 0;
static int dis_x = 0 , dis_y = 0;

//mouse event
static int left_btn_down = 0;
static int right_btn_down = 0;
//static int mouse_moved = 0;
static int last_tick = -20;
static int event = 0;

void mouseinit()
{
  outb(0x64, 0xa8);
  outb(0x64, 0xd4);
  outb(0x60, 0xf4);
  outb(0x64, 0x60);
  outb(0x60, 0x47);
  cprintf("mouse initialized.\n");
  setMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1);
  initlock(&mouse_lock, "mouse");
  picenable(IRQ_MOUSE);
  ioapicenable(IRQ_MOUSE, 0);
}

void moveMousePosition(int x, int y, int isdraw)
{
  setMousePosition(mouse_info.x_position + x, mouse_info.y_position + y, isdraw);
}

void setMousePosition(int x, int y, int isdraw)
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
  if (isdraw)
    updateMouse();
}

void updateMouseEvent(uint tick)
{
  //cprintf("tick = %d\n", tick);
  event = 0;
  if (left_down)
  {
    if (!left_btn_down)
    {
      left_btn_down = 1;
    }
    else
    {
      event = MOUSE_DRAGGING;
    }
  }
  else
  {
    if (left_btn_down)
    {
      left_btn_down = 0;
      if (tick - last_tick <= 20)
        event = LEFT_DOUBLE_CLICK;
      else
      {
        if (mouse_info.event == MOUSE_DRAGGING)
          event = 0;
        else
          event = LEFT_CLICK;
      }
      if (event == LEFT_DOUBLE_CLICK)
        last_tick = -20;
      else
        last_tick = tick;
    }
  }
  if (right_down)
  {
    if (!right_btn_down)
      right_btn_down = 1;
  }
  else
  {
    if (right_btn_down)
    {
      right_btn_down = 0;
      event = RIGHT_CLICK;
    }
  }
  mouse_info.event = event;
  if (event == LEFT_CLICK)
    cprintf("LEFT_CLICK\n");
  if (event == LEFT_DOUBLE_CLICK)
    cprintf("LEFT_DOUBLE_CLICK\n");
  if (event == MOUSE_DRAGGING)
    cprintf("MOUSE_DRAGGING\n");
  if (event == RIGHT_CLICK)
    cprintf("RIGHT_CLICK\n");
}

void mouseint(uint tick)
{
  uint ch;

  ch = inb(0x64);
  if ((ch & 0x01) == 0)
  {
    //cprintf("no data\n");
    to_read = READ_MOUSE_INFO;
    return;
  }

  acquire(&mouse_lock);
  ch = inb(0x60);
  //cprintf("ch=%d\n", ch);
  if (to_read == READ_MOUSE_INFO)
  {
    if ((ch & 0x08) == 0 || (ch & 0x04) != 0)
    {
       cprintf("Error\n");
      	release(&mouse_lock);
      	return;
    }
    //cprintf("Counter: %d\n", counter++);
    left_down = (ch & 0x01) ? 1 : 0;
    right_down = (ch & 0x02) ? 1 : 0;
    x_sign = (ch & 0x10) ? 1 : 0;
    y_sign = (ch & 0x20) ? 1 : 0;
    x_overflow = (ch & 0x40) ? 1 : 0;
    y_overflow = (ch & 0x80) ? 1 : 0;

    //cprintf("mouse_down: left_down=%d, right_down=%d\n", left_down, right_down);
    //cprintf("move direction: x_sign=%d, y_sign=%d\n", x_sign, y_sign);
    //cprintf("overflow: x_overflow = %d, y_overflow = %d\n",x_overflow,  y_overflow);
    to_read = READ_X_MOVEMENT;
    release(&mouse_lock);
    return;
  }
  else if (to_read == READ_X_MOVEMENT)
  {
    dis_x = ch;
    if (x_sign == 1)
      	dis_x = ch - 256;
    	//cprintf("x_movement: %d\n", dis);
    	to_read = READ_Y_MOVEMENT;
    	release(&mouse_lock);
    	//moveMousePosition(dis, 0);
    	return;
  }
  else if (to_read == READ_Y_MOVEMENT)
  {
    dis_y = ch;
    if (y_sign == 1)
      dis_y = ch - 256;
    dis_y = - dis_y;
    //cprintf("y_movement: %d\n", dis);
    to_read = READ_MOUSE_INFO;
    release(&mouse_lock);
    //cprintf("x: %d, y: %d\n", mouse_info.x_position, mouse_info.y_position);
    updateMouseEvent(tick);
    moveMousePosition(dis_x, dis_y,isdraw);
  }
}
