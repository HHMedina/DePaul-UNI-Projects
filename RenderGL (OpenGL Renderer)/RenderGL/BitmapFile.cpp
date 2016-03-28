/*
   BitmapFile.cpp
     all Microsoft Bitmap-file manipulation routines.  Implimentation File
	 For the BitmapFile class.  
        
     For further documentation, consult Microsoft's documentation of the 
	 bitmap file format

*/

#include <stdlib.h>
#include <io.h>
#include <malloc.h>
#include <stdio.h> 
#include <fcntl.h>
#include <string.h>
#include "BitmapFile.h"

struct BitmapFileHeader
{
	__int8  type[2];
	__int32 size;
	__int16 reserved1;
	__int16 reserved2;
	__int32  offset;
};
 
struct BitmapInfoHeader
{
	__int32 size;
	__int32 width;
	__int32 height;
	__int16 planes;
	__int16 bitCount;
	__int32 compression;
	__int32 imageSize;
	__int32 xpixelsPerMeter;
	__int32 ypixelsPerMeter;
	__int32 colorsUsed;
	__int32 colorsImportant;
};

enum BitmapFormats { BMP_RGB = 0, BMP_RLE8 = 1, BMP_RLE4 = 2, BMP_BITFIELDS = 3 };

struct BitmapCoreHeader
{
	__int32 size;
	__int16 width;
	__int16 height;
	__int16 planes;
	__int16 bitCount;
};

struct RgbQuad
{
	uchar blue;
	uchar green;
	uchar red;
	uchar reserved;
};

long ConvertLong(const uchar &beginByte)
{
	return *((long *)(&beginByte));
}

int BitmapFile::Read(char *name)
{
	char extension[10];
	int i = 0;

	for (i = 0; name[i] != '.' && name[i] != '\0'; i++);
	
	strcpy(extension, name+i);

	if (strncmp(extension, ".bmp", 4) == 0)
		return ReadBMP(name);
	else if (strncmp(extension, ".tga", 4) == 0)
		return ReadTGA(name);
	else
		return 0;
}

int BitmapFile::ReadBMP(char *fname)
{
	int    inp;
	long   infoHeaderSize;
	uchar *tmpPointer;
	short  bitCount;
	long   offset;

	BitmapFormats    format;
	BitmapFileHeader bmpFileHeader;
	BitmapInfoHeader *bmpInfoHeader = NULL;
	BitmapCoreHeader *bmpCoreHeader = NULL;

	strcpy(name, fname);

	inp = _open(name, O_RDONLY | O_BINARY);
	if(inp == -1) 
	{
		fatalerror("Couldn't open bitmap file", 2);
		return 0;
	}

/* pull in header information */
	if ((_read(inp, &bmpFileHeader, 2) <= 0) || (_read(inp, &(bmpFileHeader.size), 12) <= 0) ||
		(bmpFileHeader.type[0] != 'B') ||
		(bmpFileHeader.type[1] != 'M'))
	{
		fatalerror("Syntax error in bitmap file: malformatted header", 2);
		return 0;
	}

	offset = bmpFileHeader.offset - 14;

	if (_read(inp, &infoHeaderSize, 4) <= 0)
	{
		fatalerror("Syntax error in bitmap file: couldn't read bitmap header", 2);
		return 0;
	}	

	if (infoHeaderSize == 12) // It's a BitmapCoreHeader
	{
		tmpPointer = new uchar[12];
		if (_read(inp, tmpPointer + 4, 8) <= 0)
		{
			fatalerror("Syntax error in bitmap file: couldn't read core header", 2);
			return 0;
		}
		bmpCoreHeader = (BitmapCoreHeader *)tmpPointer;
		bmpCoreHeader->size = infoHeaderSize;
		width = bmpCoreHeader->width;
		height = bmpCoreHeader->height;
		bitCount = bmpCoreHeader->bitCount;
		format = BMP_RGB;
	}
	else // It's a BitmapInfoHeader
	{
		tmpPointer = new uchar[infoHeaderSize];
		if (_read(inp, tmpPointer + 4, infoHeaderSize - 4) <= 0)
		{
			fatalerror("Syntax error in bitmap file: couldn't read info header", 2);
			return 0;
		}
		bmpInfoHeader = (BitmapInfoHeader *)tmpPointer;
		bmpInfoHeader->size = infoHeaderSize;
		width = bmpInfoHeader->width;
		height = bmpInfoHeader->height;
		bitCount = bmpInfoHeader->bitCount;
		format = (BitmapFormats)bmpInfoHeader->compression;

		if (format == BMP_RLE4 || format == BMP_RLE8)
		{
			fatalerror("compressed bitmaps not supported", 2);
			return 0;
		}
	}
	strcpy(BitmapFile::name, name);
	type = 2;
	if (cmap != NULL)
		delete [] cmap;
	szMap = 0;
	mappix = 0;
	mapoffset = 0;
	maplen = 0;

	offset = offset - infoHeaderSize;

	switch (bitCount)
	{
	case 1:
//		ReadBMP1File(inp, offset);
		fatalerror("Bitmaps with 1 bit per pixel not supported", 2);
		return 0;
		break;
	case 4:
//		ReadBMP4File(inp, offset);
		fatalerror("Bitmaps with 4 bit per pixel not supported", 2);
		return 0;
		break;
	case 8:
		ReadBMP8File(inp, (bmpInfoHeader == 0), offset);
		break;
	case 16:
		ReadBMP16File(inp, (format == BMP_BITFIELDS), offset);
		break;
	case 24:
		ReadBMP24File(inp, offset);
		break;
	case 32:
		ReadBMP32File(inp, (format == BMP_BITFIELDS), offset);
		break;
	default:
		fatalerror("Unknown bitmap(.bmp) file type", 2);
		return 0;
	}

	delete [] tmpPointer;
	_close(inp);

	return 1;
}

int BitmapFile::ReadBMP8File(int inp, bool OS2, int offset)
{
	RgbQuad Palette[256];
	uchar  *buf;
	long    index;

	if (!OS2)
	{
		if (_read(inp, Palette, 256 * 4) <= 0)
		{
			fatalerror("couldn't read colormap", 2);
			return 0;
		}
	}
	else
	{
		if (_read(inp, Palette, 256 * 3) <= 0)
		{
			fatalerror("couldn't read colormap", 2);
			return 0;
		}

		// Adjust the bytes in the Pallete to make them RgbQuad's
		// OS2 bitmaps have RgbTripples!
		uchar *tmp = (uchar *)&Palette;
		for (int i = 255; i > 0; i--)
		{
			for (int j = 2; j >= 0; j--)
			{
				tmp[i * 4 + j] = tmp[i * 3 + j];
			}
		}
	}

	offset = offset - 256 * 4;
	if (offset > 0)
	{
		uchar *tmp = new uchar[offset];
		if (_read(inp, tmp, offset) <= 0)
		{
			fatalerror("Premature end of file reached", 2);
			return 0;
		}
		delete tmp;
	}

	long bytesPerRow = width + ((4 - width % 4) % 4);  // Rows are always multiples of 4
	long bmpSize = bytesPerRow * height;
	buf = new uchar[bmpSize];
	if (_read(inp, buf, bmpSize) <= 0)
	{
		fatalerror("Premature end of file reached", 2);
		return 0;
	}

	if (image != NULL)
		delete image;

	bytes = 4;
	type = 2;
	szImage = width * height * 4;
	image = new uchar[szImage];

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			index = buf[j * bytesPerRow + i];
			memcpy(image + 4 * (j * width + i), &Palette[index], 4);
		}
	}
	delete buf;
	return 1;
}

int BitmapFile::ReadBMP16File(int inp, bool bitmasks, int offset)
{
	__int32 Masks[3];
	int	ShiftBits[3];
	int Max[3];
	int i;

	if (bitmasks == true)
	{
		_read(inp, Masks, 12);
		offset = offset - 12;
		for (i = 0; i < 3; i++)
		{
			ShiftBits[i] = 0;
			int tmp = Masks[i];
			while (tmp % 2 == 0)
			{
				tmp = tmp >> 1;
				ShiftBits[i]++;
			}
			Max[i] = tmp;
		}
	}
	else
	{
		Masks[2] = 0x1f;
		Masks[1] = 0x7e0;
		Masks[0] = 0x7c00;
		
		ShiftBits[2] = 0;
		ShiftBits[1] = 5;
		ShiftBits[0] = 10;

		Max[0] = Max[1] = Max[2] = 0x1f;
	}

	if (offset > 0)
	{
		uchar *tmp = new uchar[offset];
		if (_read(inp, tmp, offset) <= 0)
		{
			fatalerror("Premature end of file reached", 2);
			return 0;
		}
		delete tmp;
	}

	long bytesPerRow = width * 2;
	bytesPerRow = bytesPerRow + ((4 - bytesPerRow % 4) % 4);  // Rows are always multiples of 4
	long bmpSize = bytesPerRow * height;
	
	__int16 *buf = new __int16[bmpSize / 2];  // A multiple of four ... so also a multiple of 2!
	if (_read(inp, buf, bmpSize) <= 0)
	{
		fatalerror("Premature end of file reached", 2);
		return 0;
	}

	if (image != NULL)
		delete image;

	bytes = 4;
	type = 2;
	szImage = width * height * 4;
	image = new uchar[szImage];

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			__int16 tmp = buf[j * width + i];
			int Loc = 4 * (j * width + i);
			image[Loc + 0] = uchar(((tmp & Masks[2]) >> ShiftBits[2]) * 255 / Max[2]);
			image[Loc + 1] = uchar(((tmp & Masks[1]) >> ShiftBits[1]) * 255 / Max[1]);
			image[Loc + 2] = uchar(((tmp & Masks[0]) >> ShiftBits[0]) * 255 / Max[0]);
			image[Loc + 3] = 255;
		}
	}
	delete buf;

	return 1;
}

int BitmapFile::ReadBMP24File(int inp, int offset)
{
	if (offset > 0)
	{
		uchar *tmp = new uchar[offset];
		if (_read(inp, tmp, offset) <= 0)
		{
			fatalerror("Premature end of file reached", 2);
			return 0;
		}
		delete tmp;
	}

	long bytesPerRow = width * 3;
	bytesPerRow = bytesPerRow + ((4 - bytesPerRow % 4) % 4);  // Rows are always multiples of 4
	long bmpSize = bytesPerRow * height;
	
	uchar *buf = new uchar[bmpSize];  // A multiple of four ... so also a multiple of 2!
	if (_read(inp, buf, bmpSize) <= 0)
	{
		fatalerror("Premature end of file reached", 2);
		return 0;
	}

	if (image != NULL)
		delete image;

	bytes = 4;
	type = 2;
	szImage = width * height * 4;
	image = new uchar[szImage];

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int locBuf = j * bytesPerRow + 3 * i;
			int locImage = 4 * (j * width + i);
			memcpy(image + locImage, buf + locBuf, 3);
			image[locImage + 3] = 255;
		}
	}
	delete buf;
	return 1;
}

int BitmapFile::ReadBMP32File(int inp, bool bitmasks, int offset)
{
	__int32 Masks[3];
	int	ShiftBits[3];
	int Max[3];
	int i;

	if (bitmasks == true)
	{
		_read(inp, Masks, 12);
		offset = offset - 12;
		for (i = 0; i < 3; i++)
		{
			ShiftBits[i] = 0;
			int tmp = Masks[i];
			while (tmp % 2 == 0)
			{
				tmp = tmp >> 1;
				ShiftBits[i]++;
			}
			Max[i] = tmp;
		}
	}
	else
	{
		Masks[2] = 0xff;
		Masks[1] = 0xff00;
		Masks[0] = 0xff0000;
		
		ShiftBits[2] = 0;
		ShiftBits[1] = 8;
		ShiftBits[0] = 16;

		Max[0] = Max[1] = Max[2] = 0xff;
	}

	if (offset > 0)
	{
		uchar *tmp = new uchar[offset - 256 * 4];
		if (_read(inp, tmp, offset) <= 0)
		{
			fatalerror("Premature end of file reached", 2);
			return 0;
		}
		delete tmp;
	}

	long bytesPerRow = width * 4;
	bytesPerRow = bytesPerRow + ((4 - bytesPerRow % 4) % 4);  // Rows are always multiples of 4
	long bmpSize = bytesPerRow * height;
	
	__int32 *buf = new __int32[bmpSize / 4];  // bmpSize is a multiple of 4!
	if (_read(inp, buf, bmpSize) <= 0)
	{
		fatalerror("Premature end of file reached", 2);
		return 0;
	}

	if (image != NULL)
		delete image;

	bytes = 4;
	type = 2;
	szImage = width * height * 4;
	image = new uchar[szImage];

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			__int32 tmp = buf[j * width + i];
			int Loc = 4 * (j * width + i);
			image[Loc + 0] = uchar(((tmp & Masks[2]) >> ShiftBits[2]) * 255 / Max[2]);
			image[Loc + 1] = uchar(((tmp & Masks[1]) >> ShiftBits[1]) * 255 / Max[1]);
			image[Loc + 2] = uchar(((tmp & Masks[0]) >> ShiftBits[0]) * 255 / Max[0]);
			image[Loc + 3] = 255;
		}
	}
	delete buf;
	return 1;
}

int BitmapFile::Write(char *name)
{
	char extension[10];
	int i = 0;

	for (i = 0; name[i] != '.' && name[i] != '\0'; i++);
	
	strcpy(extension, name+i);

	if (strncmp(extension, ".bmp", 4) == 0)
		return WriteBMP(name);
	else if (strncmp(extension, ".tga", 4) == 0)
		return WriteTGA(name);
	else
		return 0;
}

int BitmapFile::WriteBMP(char *name)
{
	BitmapFileHeader file;
	BitmapInfoHeader info;
	int out;

	long rowSize = width * 3;
	long extra = ((4 - rowSize % 4) % 4);
	long bytesPerRow = rowSize + extra;  // Rows are always multiples of 4
	long bmpSize = bytesPerRow * height;
	uchar filler[] = { uchar(0), uchar(0), uchar(0), uchar(0) };

	file.size = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + bmpSize - 2;
	file.type[0] = 'B';
	file.type[1] = 'M';
	file.offset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) - 2;

	info.size = sizeof(BitmapInfoHeader);
	info.width = width;
	info.height = height;
	info.planes = 1;
	info.bitCount = 24;
	info.compression = BMP_RGB;
	info.imageSize = bmpSize;
	info.xpixelsPerMeter = 1053;
	info.ypixelsPerMeter = 1053;
	info.colorsUsed = 0;
	info.colorsImportant = 0;

	out = _open(name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);

	_write(out, file.type, 2); // In Windows, the Size is always on a 4 byte boundary
	_write(out, &file.size, 12);
	_write(out, &info, sizeof(BitmapInfoHeader));
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			_write(out, image + bytes * (j * width + i), 3);			
		}
		_write(out, filler, extra);
	}
	_close(out);
	return 1;
}
