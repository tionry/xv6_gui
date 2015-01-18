typedef struct mouseinfo
{
  int x_position;
  int y_position;
  int event;
}mouseinfo;

//read info
#define READ_MOUSE_INFO 1
#define READ_X_MOVEMENT 2
#define READ_Y_MOVEMENT 3

//define events
#define MOUSE_UP 1
#define MOUSE_DOWN 2
#define MOUSE_MOVE 3
#define LEFT_CLICK 4
#define RIGHT_CLICK 5
#define LEFT_DOUBLE_CLICK 6
#define MOUSE_DRAGGING 7

