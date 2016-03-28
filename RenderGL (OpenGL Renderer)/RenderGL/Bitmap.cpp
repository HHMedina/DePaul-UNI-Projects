          
#include <stdio.h>
#include "jmisc.h"
#include "bitmap.h"
 
#define min(x, y) (((x) < (y)) ? (x) : (y))
Bitmap::Bitmap()
{
	bit = NULL;
	defBit = NULL;
	hPal = NULL;
	bInfo = NULL;
	DefBrush = NULL;
	DefPen = NULL; 
	coreHeader = false;
	colorBits = 0;
	width = 0;
	height = 0;
	target = NULL;
}

Bitmap::Bitmap(Window &Target, int Width, int Height)
{
	if ((Width > 0) && (Height > 0))
	{
		width = Width;
		height = Height;
	}
	else
	{
		width = Target.Width();
		height = Target.Height();
	}

	target = &Target;
	HDC parentDC = GetDC((HWND)Target);
	colorBits = GetDeviceCaps(parentDC, BITSPIXEL);
	ReleaseDC((HWND)(*target), parentDC);

	SetupBitmap(width, height);
}

Bitmap::Bitmap(int Width, int Height, int BitsPerPixel)
{
	width = Width;
	height = Height;

	target = NULL;
	colorBits = BitsPerPixel;

	SetupBitmap(width, height);   
}

Bitmap::Bitmap(char *filename)
{
	target = NULL;  // Make it compatible with the screen

    HDC parentDC = CreateCompatibleDC(NULL);
	colorBits = GetDeviceCaps(parentDC, BITSPIXEL);
	ReleaseDC(NULL, parentDC);

	SetupBitmap(1, 1);
	Load(filename);
}

Bitmap::Bitmap(Bitmap &orig)
{
	target = orig.target;
	colorBits = orig.colorBits;

	SetupBitmap(orig.Width(), orig.Height());
	orig.Copy(*this, 0, 0);
}

Bitmap::~Bitmap()
{
	DeleteObject(bit);
	DeleteObject(hPal);
}

void Bitmap::SetupBitmap(Window &Target, int Width, int Height)
{
	if ((Width > 0) && (Height > 0))
	{
		width = Width;
		height = Height;
	}
	else
	{
		width = Target.Width();
		height = Target.Height();
	}

	target = &Target;
	HDC parentDC = GetDC((HWND)Target);
	colorBits = GetDeviceCaps(parentDC, BITSPIXEL);
	ReleaseDC((HWND)(*target), parentDC);

	SetupBitmap(width, height);
}

void Bitmap::SetupBitmap(int Width, int Height)
{
	HDC parentDC;
    int parentBits;

	if (target != NULL)
	{
		parentDC = GetDC((HWND)*target);
		parentBits = colorBits = GetDeviceCaps(parentDC, BITSPIXEL);
	}
	else
	{
		parentDC = GetDC(NULL);
		parentBits = GetDeviceCaps(parentDC, BITSPIXEL);
	}
	
	dc = CreateCompatibleDC(parentDC);

	if (!dc)
		complain("Couldn't create memory device Context!");

	DefBrush = (HBRUSH)SelectObject(dc, brush);
	DefPen = (HPEN)SelectObject(dc, pen);

	width = Width; 
	height = Height;

    if (parentBits == colorBits)
	    bit = CreateCompatibleBitmap(parentDC, width, height);
    else
        bit = CreateBitmap(width, height, 1, colorBits, NULL);

	if (!bit)
	{
 		MessageError();
		complain("Couldn't create back bitmap");
	}

	if (!(defBit = (HBITMAP)SelectObject(dc, bit)))
		complain("Couldn't select bitmap into device context");
	if (!SelectObject(dc, brush))
		complain("Couldn't select brush");
	if (!SelectObject(dc, pen))
		complain("Couldn't select pen");

	if (target)
		ReleaseDC((HWND)(*target), parentDC);
	else
		ReleaseDC(NULL, parentDC);
	Erase();
}

void Bitmap::Resize(int Width, int Height, bool preserve)
{
	HBITMAP old = bit;
    int oldWidth = width, oldHeight = height;

	bit = CreateCompatibleBitmap(dc, Width, Height);
	SelectObject(dc, bit);
	width = Width;
	height = Height;
	Erase();

    if (preserve)
	{
		HDC tmpDC = CreateCompatibleDC(dc);
		SelectObject(tmpDC, old);

		BitBlt(dc, 0, 0, min(width, oldWidth), min(height, oldHeight), tmpDC, 0, 0, SRCCOPY);
		DeleteDC(tmpDC);
	}
	DeleteObject(old);
}

void Bitmap::GetBitmapData(unsigned char *data)
{
	BITMAPINFO bitInfo = GetBitmapInfo();

	GetDIBits(dc, bit, 0, height, data, &bitInfo, DIB_RGB_COLORS);
}

BITMAPINFO Bitmap::GetBitmapInfo(void)
{
	BITMAPINFO bitInfo;

	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biBitCount = 0;

	GetDIBits(dc, bit, 0, height, NULL, &bitInfo, DIB_RGB_COLORS);

	bitInfo.bmiHeader.biCompression = BI_RGB;

	return bitInfo;
}

void Bitmap::Save(char *fileName, bool Compression)
{
	BITMAPINFO bitInfo;
	BITMAPINFO *pbmi;
	BITMAPFILEHEADER fileHeader;
	int sizeBMI;
	int lines = 0;

	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biBitCount = 0;

	lines = GetDIBits(dc, bit, 0, height, NULL, &bitInfo, DIB_RGB_COLORS);

	pbmi = &bitInfo;

	int widthBytes;
	switch(bitInfo.bmiHeader.biBitCount)
	{
		case 24:
			sizeBMI = sizeof(BITMAPINFOHEADER);
		case 16:
		case 32:
			//sizeBMI = sizeof(BITMAPINFOHEADER)+sizeof(DWORD)*3;
			sizeBMI = sizeof(BITMAPINFOHEADER);
			bitInfo.bmiHeader.biBitCount = 24;
			bitInfo.bmiHeader.biCompression = BI_RGB;
			widthBytes = bitInfo.bmiHeader.biWidth * 3;
			if (widthBytes % 2 == 1)
				widthBytes++;
			bitInfo.bmiHeader.biSizeImage = widthBytes * bitInfo.bmiHeader.biHeight;
			break;
		break;
		default:
			sizeBMI = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*
							(1<<bitInfo.bmiHeader.biBitCount);
		break;
	}

	BYTE *bits = new BYTE[bitInfo.bmiHeader.biSizeImage];

	if (!(pbmi = (BITMAPINFO *)malloc(sizeBMI)))
		complain("Couldn't allocate Bitmap Information Structure");

	memcpy(pbmi, &bitInfo, sizeof(BITMAPINFOHEADER));

	FILE *outputFile;

	outputFile = fopen(fileName, "wb");
	if (!outputFile)
		complain("Couldn't open output file");

	fileHeader.bfType = 0x4D42;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeBMI + 
						pbmi->bmiHeader.biSizeImage;
	fileHeader.bfReserved1 = fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeBMI;

	if (!fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, outputFile))
		complain("Couldn't write file header");

	SelectObject(dc, defBit);
	lines = GetDIBits(dc, bit, 0, height, bits, pbmi, DIB_RGB_COLORS);
	SelectObject(dc, bit);

	if (!fwrite(pbmi, sizeBMI, 1, outputFile))
		complain("Couldn't write bitmap header");

	if (!fwrite(bits, bitInfo.bmiHeader.biSizeImage, 1, outputFile))
		complain("Couldn't write bitmap data");

	fclose(outputFile);

	delete bits;
	free(pbmi); 
}

void Bitmap::Load(char *fileName)
{
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	FILE *inputFile;

	inputFile = fopen(fileName, "rb");
	if (!inputFile)
		complain("Couldn't open input file");

	if (!fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, inputFile))
		complain("Couldn't read file header");

	if (fileHeader.bfType != 0x4D42)
		complain("Wrong File Type");

	if (!fread(&infoHeader, sizeof(BITMAPCOREHEADER), 1, inputFile))
		complain("Coudn't read first half of header");

	int seekSize = sizeof(BITMAPFILEHEADER);
	int sizeOfInfo = 0;

	if (infoHeader.biSize == sizeof(BITMAPCOREHEADER))
	{
		WORD bitCount = infoHeader.biBitCount;

		seekSize = sizeof(BITMAPCOREHEADER);

		int numColors = ((bitCount == 24) ? 0 : (1 << bitCount));
		sizeOfInfo = sizeof(BITMAPCOREHEADER) + sizeof(RGBTRIPLE)*numColors;
		coreHeader = true;
	}
	else if (infoHeader.biSize == sizeof(BITMAPINFOHEADER))
	{
		if (fseek(inputFile, -(int)sizeof(BITMAPCOREHEADER), SEEK_CUR) > 0)
			complain("couldn't reset file");

		if (!fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, inputFile))
			complain("Couldn't read Bitmap Info Header");

		seekSize = sizeof(BITMAPINFOHEADER);
	
		WORD bitCount = infoHeader.biBitCount;
		int numColors;

		switch (bitCount)
		{
		case 16:
		case 32:
			sizeOfInfo = sizeof(BITMAPINFOHEADER) + sizeof(DWORD)*3;
			break;
		default:
			numColors = ((bitCount == 24) ? 0 : (1 << bitCount)); 
			sizeOfInfo = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*numColors;
			break;
		}
		coreHeader = false;
	}

	colorBits = infoHeader.biBitCount;

	if (fseek(inputFile, -seekSize, SEEK_CUR) > 0)
		complain("Couldn't reset file");

	if (!(bInfo = (BITMAPINFO *)malloc(sizeof(BYTE) * sizeOfInfo)))
		complain("Couldn't allocate bitmap info");

	if (!fread(bInfo, sizeOfInfo, 1, inputFile))
		complain("Couldn't read info header");

	BYTE *bits = new BYTE[bInfo->bmiHeader.biSizeImage];

	if (!fread(bits, bInfo->bmiHeader.biSizeImage, 1, inputFile))
		complain("Couldn't read file data");

	fclose(inputFile);

	width = bInfo->bmiHeader.biWidth;
	height = bInfo->bmiHeader.biHeight;

	DeleteObject(bit);
	bit = CreateDIBitmap(dc, (LPBITMAPINFOHEADER)bInfo, CBM_INIT, bits, bInfo, DIB_RGB_COLORS);
	if (!bit)
	{
		MessageError();
		complain("Couldn't Create Bitmap");
	}
	SelectObject(dc, bit);
	SelectDIBPalette(); 

	delete bits;
}

bool Bitmap::SelectDIBPalette(void) 
{   
	LOGPALETTE    *plogPal;   
	UINT          uiSizPal;   
	INT           i, iNumClr;   
	WORD          wBitCount;
    
	if (coreHeader) 
	{     
		wBitCount = ((LPBITMAPCOREINFO)bInfo)->bmciHeader.bcBitCount;   
	} 
	else 
	{     
		wBitCount = bInfo->bmiHeader.biBitCount;   
	}    
	
	switch (wBitCount) 
	{     
	case 16:     
	case 24:     
	case 32:                            
		return false;     
	default: 
		iNumClr = (1 << wBitCount); 
		break;  
	}    
	uiSizPal = sizeof(WORD)*2 + sizeof(PALETTEENTRY)*iNumClr;   
	
	if ((plogPal = (LOGPALETTE *) LocalAlloc(LMEM_FIXED,uiSizPal)) == NULL) 
	{       
		hPal = NULL;       
		return false;   
	}    
	plogPal->palVersion = 0x300;   
	plogPal->palNumEntries = (WORD) iNumClr;    
	
	if (coreHeader) 
	{     
		for (i = 0; i < iNumClr; i++) 
		{ 
			plogPal->palPalEntry[i].peRed = ((LPBITMAPCOREINFO)bInfo)->bmciColors[i].rgbtRed; 
			plogPal->palPalEntry[i].peGreen = ((LPBITMAPCOREINFO)bInfo)->bmciColors[i].rgbtGreen; 
			plogPal->palPalEntry[i].peBlue  = ((LPBITMAPCOREINFO)bInfo)->bmciColors[i].rgbtBlue; 
			plogPal->palPalEntry[i].peFlags = PC_RESERVED;     
		}   
	} 
	else 
	{     
		for (i = 0; i < iNumClr; i++) 
		{ 
			plogPal->palPalEntry[i].peRed = bInfo->bmiColors[i].rgbRed; 
			plogPal->palPalEntry[i].peGreen = bInfo->bmiColors[i].rgbGreen; 
			plogPal->palPalEntry[i].peBlue  = bInfo->bmiColors[i].rgbBlue; 
			plogPal->palPalEntry[i].peFlags = PC_RESERVED;     
		}  
	}    
	
	DeleteObject(hPal);   

	hPal = CreatePalette((LPLOGPALETTE)plogPal);   
	if (hPal == NULL) 
	{       
		return false;   
	}    
	
	if ((GetDeviceCaps(dc, RASTERCAPS)) & RC_PALETTE) 
	{     
		SelectPalette(dc, hPal, false);     
		RealizePalette(dc);   
	}    
	GlobalFree(plogPal);    

	return true; 
}  

