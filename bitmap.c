#include "gui.h"
#include "bitmap.h"

void readBitmapHeader(FILE *bmpFile, BITMAP_FILE_HEADER *bmpFileHeader, BITMAP_INFO_HEADER *bmpInfoHeader)
{
  // Read Bitmap file header
  fread(bmpFileHeader, sizeof(BITMAP_FILE_HEADER), 1, bmpFile);

  // Skip Bitmap file header
  fseek(bmpFile, sizeof(BMPMAP_FILE_HEADER), 0);

  // Read Bitmap info header
  fread(bmpInfoHeader, sizeof(BITMAP_INFO_HEADER), 1, bmpFile);
}

int readBitmapFile(char *fileName, RGB *result, int *height, int *width);
{
  FILE *bmpFile = fopen(fileName, "r");
  if (bmpFile == NULL)
  {
    return -1;
  }

  BITMAP_FILE_HEADER bmpFileHeader;
  BITMAP_INFO_HEADER bmpInfoHeader;
  
  readBitmapHeader(bmpFile, &bmpFileHeader, &bmpInfoHeader);
  *width = bmpInfoHeader.biWidth;
  *height = bmpInfoHeader.biHeight;
  int column = *width;
  int row = *height;

  result = (RGB *) malloc(sizeof(RGB) * column * row);
  fseek(bmp, bmpFileHeader.bfOffbits, 0);
  for (i = 0; i < row; i++)
  {
    for (j = 0; j < column; j++)
    {
      fread(result + i * row + j, 3, 1, bmp);
    }
    fseek(bmp, (4 - (column % 4)), SEEK_CUR); // Add padding bytes
  }
  return 0;
}
