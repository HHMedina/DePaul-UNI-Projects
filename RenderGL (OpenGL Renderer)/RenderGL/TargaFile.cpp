/*
   BitmapFile.cpp
     all targa-file manipulation routines.  Implimentation File
	 For the BitmapFile class.  This file only has the implementation
	 for non-colormaped TIFF files.  Find the colormaped Tiff methods 
	 in TargaMapped.cpp
        
     For further documentation, consult "Targa Library 1.0"
 
                                                     10 jan 92
     Reads/Writes Targa-24 files                     13 nov 92
     Reads/Write Targa-24 type 10 (compressed) 	      5 feb 93
	 Reads/Writes Type 1 (colormapped) 	             24 jan 94
     Reads images into preallocated memory areas     13 apr 94 
	 Reads/Writes Type 9 (compressed colormapped)     7 jun 94

	 Converted to a C++ object						 18 jul 01  jcm

*/

#include <stdlib.h>
#include <io.h>
#include <malloc.h>
#include <stdio.h> 
#include <fcntl.h>
#include <string.h>
#include "BitmapFile.h"
#include "jmisc.h"

int   AsMap = 0;
 
BitmapFile::BitmapFile(int w, int h, bool mapped)
{
	image = NULL;
	cmap = NULL;

	if ((w > 0) && (h > 0))
	{
		Create(w, h);
	}
}

/*
    These routines 
       1) access image stored as Targa-32 non-colormapped files. 
       2) manipulate image data while stored in memory.
                                                                      */
/**********************************************************************/
void BitmapFile::Create(int w, int h)
/**********************************************************************/
/*
   Given a width and height, creates a Tga struct.  Initializes
   the the fields of Tga struct for a Targa-32 type file (four 
   bytes per pixel).  Allocates memory to hold an image of the
   desired dimensions, and sets the memory to zero.

   Returns pointer to a Tga struct if function can successfully
   allocate the space.  Returns a null pointer for failure.
                                                                      */
{
	int  j;

	if (image != NULL)
		delete [] image;

	width = w;
	height = h;
	type = 2;
	bytes = 4;
	image = new uchar[width * height * bytes];
	cmap = NULL;
	maplen = 0;
	mappix = 0;
	mapoffset = 0;
	szImage = bytes * width * height;
	for(j = 0;j < szImage ; ++j)
		image[j] = 0;
	rgb = false;
}

/**********************************************************************/
BitmapFile::~BitmapFile()
/**********************************************************************/
/*
   return memory occupied by the tga struct to the memory allocator 
                                                                      */
{
	if (image != NULL)
		delete image; 
	if (cmap != NULL)
		delete cmap;
}

/**********************************************************************/
int BitmapFile::ReadTGA(char *n)
/**********************************************************************/
/*  
   Given a file name, 
      1. Open the file,
      2. Read 18-byte header to determine the dimensions of 
                the image,
      3. Read in the raw targa file,
      4. Record width, height, number of bytes per pixel,
                 and name of input file.

	Returns 0 in case of failure.
*/
{
	int    inp;                     /* file pointer                               */
	int    upper, lower;            /* aux vbls to determine w, h                 */
	char   header[18];              /* file header                                */
	int    wtga, htga;              /* image width, height                        */
	int    onepix;                  /* number of bytes in one pixel               */
	int    newSzImage;				/* New Image size (do we have to reallocate?) */

	strcpy(name, n);

/* open the file */
	inp = _open(n, O_RDONLY | O_BINARY);
	if(inp == -1) 
	{
		fatalerror("Couldn't open targa file", 2);
		return 0;
	}

/* pull in header information */
	if (_read(inp,header,18) <= 0) 
	{
		fatalerror("Syntax error in targa file: couldn't read header", 2);
		return 0;
	}

	upper = 0x00ff & header[13]; 
	lower = 0x00ff & header[12];
	wtga = (upper << 8) | lower;
	upper = 0x00ff & header[15]; 
	lower = 0x00ff & header[14];
	htga = (upper << 8) | lower;
	onepix = (int)(0x0ff & header[16]) / 8;
	
	newSzImage = wtga * htga * onepix;

	if (image != NULL)	
		delete [] image;
	image = new uchar[newSzImage];
	szImage = newSzImage;

	if (cmap != NULL)
	{
		delete [] cmap;
		cmap = NULL;
	}

	width = wtga;
	height = htga;
	type = (int)(0x0ff & header[2]);
	bytes = onepix;
	mappix = 0;
	mapoffset = 0;
	maplen = 0;

	switch ((int)header[2])
	{
	case 1:
		if(AsMap)
		{
			if(!ReadTGAAsMapped(inp, header)) 
			{
				fatalerror("Couldn't read colormapped file", 2);
				_close(inp);
				return 0; 
			}
		}
		else
		{
			if(!ReadTGAColormapFile(inp, header)) 
			{
				fatalerror("Couldn't read colormapped file", 2);
				_close(inp);
				return 0;
			}
		}
		break;		
	case 2:
		if (_read(inp, image, szImage) < 0) /* 24 bit image uncompressed.  Just read it! */
		{
			fatalerror("Improperly formatted image data", 2);
			_close(inp);
			return 0;
		}
		break;
	case 9:
		if(AsMap)
		{
			if(!ReadTGAAsMappedCompressed(inp, header))
			{
				fatalerror("Couldn't read colormapped file", 2);
				_close(inp);
				return 0;
			}
		}
		else
		{
			if(!ReadTGACompressedColormapFile(inp, header)) 
			{
				fatalerror("Couldn't read colormapped file", 2);
				return 0;
			}
		}
		break;
	case 10:
		if(!ReadTGA10File(inp)) 
		{	
			fatalerror("Couldn't read compressed targa file", 2);
			_close(inp);
			return 0;
		}
		break;		
	default:
		fatalerror("Targa file not a type-1, 2, 9, or 10\n",2);
		_close(inp);
		return 0;
	}
                    /* File has been successfully read.  
	                   Return Tga struct to calling function  */
	_close(inp);
	rgb = false;
	return 1;
}

/**********************************************************************/
int BitmapFile::ReadTGA10File(int in)
/**********************************************************************/
/*  
   Given a file pointer 
      Read in the raw targa file, and store as a rectangular
	     array of pixels.

      Returns 1 if successful.
      Returns 0 if failure.
                                                                      */
{
	int            rep;
	uchar		   buffer[512];
	int            npix, shouldbe;
	int            sizeofpix;
	int            j;

	shouldbe = width * height;
	sizeofpix = bytes;
	npix = 0;
	while(npix < shouldbe)
	{
		rep = readchar(in);
     
		if(rep < 0) 
			fatalerror("Premature End of File Reached", 2);

		if(rep < 128)                       /* raw packet */
		{
			++rep;
			if (_read(in, buffer, (int)(sizeofpix * rep)) < sizeofpix * rep)
				fatalerror("Premature End of File Reached", 2);
			memcpy(image + (sizeofpix * npix), buffer, sizeofpix * rep);
		}
		else	                          /* run packet */
		{
			rep -= 127;
			if (_read(in,buffer,sizeofpix) < sizeofpix)
				fatalerror("Premature End of File Reached", 2);
			for(j=0;j<rep;++j)
				memcpy(image + (sizeofpix * (npix + j)), buffer, sizeofpix);
		} 
		npix += rep;
	}
	return 1;
}

/**********************************************************************/
int BitmapFile::readchar(int in)
/**********************************************************************/\
{ 
	uchar ch;

	if (_read(in, &ch, 1) < 1)
		return -1;
	else
		return (int)(0x0ff & ch);
}

/**********************************************************************/
Pixel BitmapFile::GetPixel(int x, int y)
/**********************************************************************/
/*     x,y;                      location (x,y) in image           

  returns the pixel information at location (j,k) in image.    
  Does not perform bounds checking.
                                                                      */
{
	Pixel  pix;
	int loc = imloc(y, x);

	if(type == 1)
	{
		int    index, offset;

		index = 0x0ff & (int)(image[loc]);
		offset = index * mappix;
		pix.b = cmap[offset];
		pix.g = cmap[offset+1];
		pix.r = cmap[offset+2];
		if(mappix == 4) 
			pix.alpha = cmap[offset+3];
		else
			pix.alpha = 255;
		return pix;
	}
	else
	{
		uchar *tmpImg = image + loc;
		pix.r = tmpImg[2];
		pix.g = tmpImg[1];
		pix.b = tmpImg[0];
		if(bytes > 3) 
			pix.alpha = tmpImg[3];
		else
			pix.alpha = 255;
		return pix;
	}
}

/**********************************************************************/
int BitmapFile::GetIndex(int x, int y)
/**********************************************************************/
/*
  returns the color index at location (x,y) in image.    
  Does not perform bounds checking.
                                                                      */
{
	if(type == 1)
		return  (int)(image[imloc(y,x)] & 0x0ff);
	else
		return -1;
}

/**********************************************************************/
void BitmapFile::PutPixel(int x, int y, Pixel pix)
/**********************************************************************/

{
	if(type == 1) return;

	int idx = imloc(y, x);

	if(bytes == 4)
		image[idx + 3] = pix.alpha;
	image[idx + 2] = pix.r;
	image[idx + 1] = pix.g;
	image[idx + 0] = pix.b; // 0 should get ignored by optimizer
	return;					// makes code more readable
}

/**********************************************************************/
void BitmapFile::PutIndex(int x, int y, int index)
/**********************************************************************/
{
	if(type != 1) 
		return;
	image[imloc(y,x)] = index;
	return;
}

/**********************************************************************/
void BitmapFile::CopyRGB(Pixel *to, Pixel from)
/**********************************************************************/
/* Pixel *to, from;                pixels                             */
/*
  Private. copies a pixel from one location to another.  Only for
  those people who know how to compute the address of individual
  pixels.
                                                                      */
{
	to->r = from.r;
	to->g = from.g;
	to->b = from.b;
}

/**********************************************************************/
void BitmapFile::CopyRGBA(Pixel *to, Pixel from)
/**********************************************************************/
/* Pixel *to, from;                pixels                             */
/*
  Private. copies a pixel from one location to another.  Only for
  those people who know how to compute the address of individual
  pixels.
                                                                      */
{
	to->r = from.r;
	to->g = from.g;
	to->b = from.b;
	to->alpha = from.alpha;
}

/**********************************************************************/
int BitmapFile::WriteTGA(char *name)
/**********************************************************************/
/*
  Given a name (name) and an image expressed as a Tga struct (tga),
  writes the image to the named file.  

  Function returns 1 if it successfully completes file write.
           returns 0 for failure.
                                                                      */
{
	static char lheader[18] =  {(char)0x00, (char)0x00, (char)0x02, (char)0x00,
								(char)0x00, (char)0x00, (char)0x00, (char)0x00,
								(char)0x00, (char)0x01, (char)0x90, (char)0x01,
								(char)0x00, (char)0x00, (char)0x00, (char)0x00,
								(char)0x20, (char)0x08};

	int out;                      /* file pointer                       */
	char  msg[256];

	if(type != 2 && type != 10)
	{
		sprintf(msg,"%s is a colormapped file\n%s", name,
				"WriteTGATgaFile can't write it as a type-2 file\n");
		fatalerror(msg,2);
		return 0;
	}
	lheader[16] = (uchar)(bytes * 8);
	lheader[17] = (uchar)(bytes * 8 - 24);

	out = _open(name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
	if(out == -1)
	{
		fatalerror("Couldn't create file", 2);
		return 0;
	}

	lheader[12] = lower(width);
	lheader[13] = upper(width);
	lheader[14] = lower(height);
	lheader[15] = upper(height);
	_write(out, lheader, 18);                   /* write header                 */
	_write(out, image, width * height * bytes); /* write image data             */
	_close(out);

	return 1;
}

/**********************************************************************/
int BitmapFile::WriteTGA24(char *name)
/**********************************************************************/
/*
  Given a name (name) and an image expressed as a Tga struct (tga),
  writes the image to the named file in Targa24 format.  

  Function returns 1 if it successfully completes file write.
           returns 0 for failure.
                                                                      */
{
	static char lheader[18] =  {(char)0x00, (char)0x00, (char)0x02, (char)0x00,
								(char)0x00, (char)0x00, (char)0x00, (char)0x00,
								(char)0x00, (char)0x01, (char)0x90, (char)0x01,
								(char)0x00, (char)0x00, (char)0x00, (char)0x00,
								(char)0x18, (char)0x00};

	int out;                      /* file pointer                     */
	int j,k;
	uchar *tmp;
	char  msg[256];

	if(type != 2 && type != 10)
	{
		sprintf(msg,"%s is a colormapped file\n", name,
				"WriteTGA24 can't write it as a type-2 file\n");
		fatalerror(msg,2);
		return 0;
	}

	tmp = new uchar[width * height * 3];

	for(j = 0; j < height; ++j)
		for(k = 0; k < width; ++k)
			CopyRGB((Pixel *)(tmp + loc24(j, k)), GetPixel(k, j));

	out = _open(name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
	if(out == -1)
	{
		fatalerror("Couldn't Create File", 2);
		return 0;
	}

	lheader[12] = lower(width);
	lheader[13] = upper(width);
	lheader[14] = lower(height);
	lheader[15] = upper(height);
	_write(out, lheader, 18);                      /* write header     */
	_write(out, tmp, width * height * 3);        /* write image data */
	_close(out);

	delete [] image;

	_close(out);

	return 1;
}

/**********************************************************************/
int BitmapFile::WriteTGA10(char *name)
/**********************************************************************/
/*
  Given a name (name) writes the image to the named file in Targa24  
  compressed (type 10) format.  

  Function returns 1 if it successfully completes file write.
           returns 0 for failure.
                                                                      */
{
	static char lheader[18] =  {(char)0x00, (char)0x00, (char)0x0a, (char)0x00,
								(char)0x00, (char)0x00, (char)0x00, (char)0x00,
								(char)0x00, (char)0x01, (char)0x90, (char)0x01,
								(char)0x00, (char)0x00, (char)0x00, (char)0x00,
								(char)0x18, (char)0x00};

	unsigned int out;                    /* file pointer                       */
	int   x,y;
	Pixel raster[128];
	Pixel current, last;
	char  msg[256];
	unsigned int nraw, run;

	if(type != 2 && type != 10)
	{
		sprintf(msg,"%s is a colormapped file\nWriteTGATga10 can't write it as a type-10 file.",
				name);
		fatalerror(msg,2);
		return 0;
	}

	out = _open(name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
	if(out == -1)
	{
		sprintf(msg,"can't open %s \n", name);
		fatalerror(msg,2);
		return 0;
	}
	lheader[12] = lower(width);
	lheader[13] = upper(width);
	lheader[14] = lower(height);
	lheader[15] = upper(height);
	_write(out,lheader,18);        /* write header                       */

	CopyRGB(&last, GetPixel(0,0));
	run = 0;
	nraw = 0;

	for(y = 0; y < height; ++y)
	{
		for(x = 0; x < width; ++x)
		{
			CopyRGB(&current, GetPixel(x,y));
			if (match(last, current))
			{
				if (run < 128)
					++run;
				else
				{
					if (nraw) 
						flushraw(out, raster, nraw);
					nraw = 0;
					flushrun(out, run, last);
					run = 1;
				}
			}
			else
			{ 
				if(run > 5)
				{
					if(nraw)
						flushraw(out,raster,nraw);
					nraw = 0;
					flushrun(out,run,last);
				}
				else if(run + nraw > 127)
       		    {
					if(nraw)
						flushraw(out,raster,nraw);
					nraw = 0;
					addraw(raster,nraw,last,run);
					nraw += run;
				}
				else
				{
					addraw(raster,nraw,last,run);
					nraw += run;
				}
				run = 1;
				CopyRGB(&last,current);
			}
		}
	}
	if (nraw)
		flushraw(out,raster,nraw);
	flushrun(out,run,last);
	_close(out);

	return 1;
}

/**********************************************************************/
int BitmapFile::flushrun(int out, int run, Pixel last)
/**********************************************************************/
{
	uchar arun[4];

	arun[0] = (uchar)(run + 127);
	arun[1] = last.b;
	arun[2] = last.g;
	arun[3] = last.r;

	_write(out, arun, 4);
	return 1;
}

/**********************************************************************/
int BitmapFile::flushraw(int out, Pixel pix[128], int nraw)
/**********************************************************************/
{
	uchar raster[385];
	uchar j;

	raster[0] = (uchar) (nraw - 1);
	for(j=0;j<nraw;++j)
	{
		raster[3*j + 1] = pix[j].b;
		raster[3*j + 2] = pix[j].g;
		raster[3*j + 3] = pix[j].r;
	}
	_write(out,raster,(int)(3*nraw+1));
	return 1;
}

/**********************************************************************/
int BitmapFile::addraw(Pixel pix[128], int n, Pixel apix, int rep)
/**********************************************************************/
{
	uchar j;

	for(j=0;j<rep;++j)
		CopyRGB(&(pix[n+j]),apix);

	return 1;
}

/**********************************************************************/
int BitmapFile::match(Pixel x,Pixel y)
/**********************************************************************/
{
	return (x.r == y.r && x.g == y.g && x.b == y.b) ;
}

/**********************************************************************/
uchar BitmapFile::Red(int k, int j)
/**********************************************************************/
/*
   Given an image expressed as a Tga struct and a location (j,k),
   returns the red component of the pixel located at j,k
                                                                      */
{
	return *(image + (j*width + k) * bytes + 2);
}

/**********************************************************************/
uchar BitmapFile::Green(int k, int j)
/**********************************************************************/
{
	return *(image + (j*width + k) * bytes + 1);
}

/**********************************************************************/
uchar BitmapFile::Blue(int k, int j)
/**********************************************************************/
{
	return *(image + (j*width + k) * bytes);
}

/**********************************************************************/
uchar BitmapFile::Alpha(int k, int j)
/**********************************************************************/
{
	return *(image + (j*width + k) * bytes + 3);
}

/*****************************************************************************/
void BitmapFile::fatalerror(const char *msg,int exitnumber)
/*****************************************************************************/
/*
   prints error message to output and log file.  
                                                                             */
{ 
	char totalMsg[1024];

	sprintf(totalMsg, "%s\nWhile trying to read file '%s'", msg, name);
	complain(totalMsg);
}

/*****************************************************************************/
void BitmapFile::ReverseColors(void)
/*****************************************************************************/
/*
   Reverses the BGR format of the bitmap
                                                                             */
{ 
	for (int j = 0; j < height; j++) 
	{ 
		for (int i = 0; i < width; i++) 
		{
			int loc = bytes * (j * width + i); 
			int tmp = image[loc]; 
			image[loc] = image[loc + 2]; 
			image[loc + 2] = tmp; 
		} 
	}
}

/*****************************************************************************/
void BitmapFile::Resize(int w, int h)
/*****************************************************************************/
/*
   Enlarges the bitmap while preserving as much of the image as possible
   Does not rescale!
                                                                             */
{
	int minW, minH;
	uchar *buf;

	minW = (w < width ? w : width);
	minH = (h < height ? h : height);

	buf = new uchar[w * h * bytes];
	memset(buf, 0, w * h * bytes);
	for (int j = height - 1; j >= height - minH; j--)
	{
		memcpy(buf + bytes * (h - height + j) * w, image + bytes * j * width, bytes * minW);
	}
	delete [] image;
	image = buf;
	width = w;
	height = h;
	szImage = width * height * bytes;
}

static double min(double x, double y)
{
	return (x < y ? x : y);
}

static double max(double x, double y)
{
	return (x < y ? y : x);
}

/*****************************************************************************/
void BitmapFile::Filter(int r, int g, int b, int a)
/*
   Multiplies all the colors in the file by the given color. 
   To do the multiply it divides the colors to put them into a 0 --> 1
   Range, multiplies them and then scales them back to 0 --> 255
                                                                             */
{
	double B = b / 255.0, G = g / 255.0, R = r / 255.0, A = a / 255.0;

	for (int i = 0; i < width * height; i++)
	{
		image[bytes * i + 0] = (uchar)(min(max(image[bytes * i + 0] / 255.0 * B, 0.0), 1.0) * 255.0);
		image[bytes * i + 1] = (uchar)(min(max(image[bytes * i + 1] / 255.0 * G, 0.0), 1.0) * 255.0);
		image[bytes * i + 2] = (uchar)(min(max(image[bytes * i + 2] / 255.0 * R, 0.0), 1.0) * 255.0);
		if (bytes == 4)
			image[bytes * i + 3] = (uchar)(min(max(image[bytes * i + 3] / 255.0 * A, 0.0), 1.0) * 255.0);
	}
}

/*****************************************************************************/
void BitmapFile::Filter(double R, double G, double B, double A)
/*
   Multiplies all the colors in the file by the given color. 
   To do the multiply it divides the colors to put them into a 0 --> 1
   Range, multiplies them and then scales them back to 0 --> 255
                                                                             */
{
	for (int i = 0; i < width * height; i++)
	{
		image[bytes * i + 0] = (uchar)(min(max(image[bytes * i + 0] / 255.0 * B, 0.0), 1.0) * 255.0);
		image[bytes * i + 1] = (uchar)(min(max(image[bytes * i + 1] / 255.0 * G, 0.0), 1.0) * 255.0);
		image[bytes * i + 2] = (uchar)(min(max(image[bytes * i + 2] / 255.0 * R, 0.0), 1.0) * 255.0);
		if (bytes == 4)
			image[bytes * i + 3] = (uchar)(min(max(image[bytes * i + 3] / 255.0 * A, 0.0), 1.0) * 255.0);
	}
}

/*****************************************************************************/
void BitmapFile::Add(int r, int g, int b, int a)
/*
   Adds the constant color to all the colors in the file. 
   To do the add it divides the colors to put them into a 0 --> 1
   Range, adds them and then scales them back to 0 --> 255
                                                                             */
{
	double B = b / 255.0, G = g / 255.0, R = r / 255.0, A = a / 255.0;

	for (int i = 0; i < width * height; i++)
	{
		image[bytes * i + 0] = (uchar)(min(max(image[bytes * i + 0] / 255.0 + B, 0.0), 1.0) * 255.0);
		image[bytes * i + 1] = (uchar)(min(max(image[bytes * i + 1] / 255.0 + G, 0.0), 1.0) * 255.0);
		image[bytes * i + 2] = (uchar)(min(max(image[bytes * i + 2] / 255.0 + R, 0.0), 1.0) * 255.0);
		if (bytes == 4)
			image[bytes * i + 3] = (uchar)(min(max(image[bytes * i + 3] / 255.0 + A, 0.0), 1.0) * 255.0);
	}
}

/*****************************************************************************/
void BitmapFile::Add(double R, double G, double B, double A)
/*
   Adds the constant color to all the colors in the file. 
   To do the add it divides the colors to put them into a 0 --> 1
   Range, adds them and then scales them back to 0 --> 255
                                                                             */
{
	for (int i = 0; i < width * height; i++)
	{
		image[bytes * i + 0] = (uchar)(min(max(image[bytes * i + 0] / 255.0 + B, 0.0), 1.0) * 255.0);
		image[bytes * i + 1] = (uchar)(min(max(image[bytes * i + 1] / 255.0 + G, 0.0), 1.0) * 255.0);
		image[bytes * i + 2] = (uchar)(min(max(image[bytes * i + 2] / 255.0 + R, 0.0), 1.0) * 255.0);
		if (bytes == 4)
			image[bytes * i + 3] = (uchar)(min(max(image[bytes * i + 3] / 255.0 + A, 0.0), 1.0) * 255.0);
	}
}