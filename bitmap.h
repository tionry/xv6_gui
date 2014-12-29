typedef struct BITMAP_FILE_HEADER {
	unsigned short bfType;
	unsigned int bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int btOffBits;
} BITMAP_FILE_HEADER;

typedef struct BITMAP_INFO_HEADER {
	unsigned int biSize;
	int biWidth;
	int biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int biCompression;
	unsigned int biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	unsigned int biCirUserd;
	unsigned int biCirImportant;
} BITMAP_INFO_HEADER;
