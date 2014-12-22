#define MAX_WINDOW_NUM 32

typedef enum windowState
{
  none,
  hide,
  show
}windowState;

typedef struct Window
{
  windowState state;
  int leftTopX, leftTopY, width, height;
  char caption[256];
  struct Window *next;
} Window;

