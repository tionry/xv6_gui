#define MAX_WINDOW_NUM 256

typedef enum windowType
{
  none = -1,
  label,
  textbox,
  button,
  imageView
}windowType;

typedef struct Window
{
  windowType type;
  int leftTopX, leftTopY, width, height, show;
  char caption[256];
} Window;

