#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"
#include "uwindow.h" 
#include "bitmap.h"

void readBitmapHeader(int bmpFile, BITMAP_FILE_HEADER *bmpFileHeader, BITMAP_INFO_HEADER *bmpInfoHeader)
{
  // Read Bitmap file header
  read(bmpFile, bmpFileHeader, sizeof(BITMAP_FILE_HEADER));

  // Read Bitmap info header
  read(bmpFile, bmpInfoHeader, sizeof(BITMAP_INFO_HEADER));
}

int readBitmapFile(char *fileName, RGB *result, int *height, int *width)
{
  int i;
  int j;

  int bmpFile = open(fileName, 0);
  if (bmpFile < 0)
  {
    return -1;
  }

  BITMAP_FILE_HEADER bmpFileHeader;
  BITMAP_INFO_HEADER bmpInfoHeader;
  
  readBitmapHeader(bmpFile, &bmpFileHeader, &bmpInfoHeader);
printf(1, "%d\n", bmpFileHeader.bfSize);
printf(1, "%d\n", bmpFileHeader.bfReserved1);
printf(1, "%d\n", bmpFileHeader.bfReserved2);
return 0;
  *width = bmpInfoHeader.biWidth;
  *height = bmpInfoHeader.biHeight;
  int column = *width;
  int row = *height;
  char tmpBytes[3];

  result = (RGB *) malloc(sizeof(RGB) * column * row);
  for (i = 0; i < row; i++)
  {
    for (j = 0; j < column; j++)
    {
      read(bmpFile, result + i * column + j, 3);
    }
    read(bmpFile, tmpBytes, (4 - (column % 4))); // Add padding bytes
  }
  return 0;
}

