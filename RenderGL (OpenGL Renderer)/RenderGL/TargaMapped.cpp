/*
   BitmapFile.cpp
     all targa-file manipulation routines.  Implimentation File
	 For the BitmapFile class.  This file only has the implementation
	 for colormaped TIFF files.  Find the non-colormaped Tiff methods 
	 in BitmapFile.cpp
        
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
#include <stdio.h> 
#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include "BitmapFile.h"

int  readchar(int in);
int  hash(Pixel pix);

int  flushbyteraw(int out, uchar raster[128],
				           int nraw);
int  flushbyterun(int out, int run,uchar last);
int  addbyteraw(uchar raster [128],int n,
			               uchar index,int rep);
void fatalerror(char *msg, int severity);

/**********************************************************************/
void BitmapFile::CreateMapped(int w, int h)
/**********************************************************************/
{
   int  j;

   width = w;
   height = h;
   type = 1;
   bytes = 1;      
                                                        /* bitmap */
   szImage = w * h * sizeof(uchar);
   image = new uchar[szImage];
   for(j = 0; j < szImage; ++j)
        image[j] = (uchar)0;
                                                        /* colormap */
   szMap = 256 * 3 * sizeof(uchar);
   cmap = new uchar[szMap]; 
   for(j = 0; j < szMap; ++j)
        cmap[j] = (uchar)0;
   maplen = 256;
   mappix = 3;
   mapoffset = 0;
}

/**********************************************************************/
int BitmapFile::ReadTGAColormapFile(int in, char header[18])
/**********************************************************************/
{
	uchar   *buf;
	int     somap;
	int     loc, index;
	int     x,y;

	/* get color map     */
	mappix = header[7] / 8;
	mapoffset = (0x00ff & header[4]) << 8 | (0x00ff & header[3]);
	maplen = (0x00ff & header[6]) << 8 | (0x00ff & header[5]);
	somap = mappix * (mapoffset + maplen);

	if(cmap == (uchar *)0)
	{
		cmap = new uchar[somap]; 
	}
	else if (somap != szMap)
	{
		delete [] cmap;
		cmap = new uchar[somap];
	}
	szMap = somap;

	if (_read(in, cmap + mappix * mapoffset, mappix * maplen) < 0)
		return 0;

	/* get pixel info    */ 
	if(image != (uchar *) 0)
		delete [] image;
	image = new uchar[width * height * mappix]; 

	buf = new uchar[width * height];
	if (_read(in, buf, width * height) < 0) 
		return 0;

	for(y = 0; y < height; ++y)
	{
		for(x = 0; x < width; ++x)
		{
			loc = y * width + x;
			index = (int)(0x0ff & buf[loc]); // should we add mapoffset here?
			memcpy(image + (loc * mappix), cmap + index * mappix, mappix);
		}
	}
	free(buf);

	bytes = mappix;
	type = 2;

	return 1;
}

/**********************************************************************/
int BitmapFile::ReadTGAAsMapped(int in, char header[18])
/**********************************************************************/
{
	int somap;

	/* get color map     */
	mappix = header[7] / 8;
	mapoffset = (0x00ff & header[4]) << 8 | (0x00ff & header[3]);
	maplen = (0x00ff & header[6]) << 8 | ( 0x00ff & header[5]);
	somap = mappix * (mapoffset + maplen);

	if(cmap == (uchar *)0)
	{
		cmap = new uchar[somap]; 
	}
	else if (somap != szMap)
	{
		delete [] cmap;
		cmap = new uchar[somap];
	}
	szMap = somap;

	if (_read(in, cmap + mappix * mapoffset, mappix * maplen) < 0) 
		return 0;

	/* get pixel info    */ 
	image = new uchar[width * height]; 
	if (_read(in, image, width * height) < 0) 
		return 0;

	bytes = 1;
	type = 1;
	
	return 1;
}

/**********************************************************************/
int BitmapFile::ReadTGACompressedColormapFile(int in, char header[18])
/**********************************************************************/
{
	uchar buf[128];
	int   somap;
	int   index;
	int   j;
	int   rep, npix, shouldbe;
						/* get colormap       */
    mappix = header[7]/8;
    mapoffset = (0x0ff & (int) header[4]) << 8 |
             (0x0ff & (int) header[3]);
    maplen = (0x0ff & (int) header[6]) << 8 |
             (0x0ff & (int) header[5]);
    somap = mappix * (mapoffset + maplen);

	if(cmap == (uchar *)0)
	{
		cmap = new uchar[somap]; 
	}
	else if (somap != szMap)
	{
		delete [] cmap;
		cmap = new uchar[somap];
	}
	szMap = somap;

    if (_read(in, cmap + mappix * mapoffset, mappix * maplen) < 0) 
		return 0;
					        /* get image          */

    image = new uchar[width * height * mappix];
    shouldbe = width * height;

    npix = 0;
    while (npix < shouldbe)
	{
		rep = (int) readchar(in) & 0x0ff;

		if(rep < 128)
		{				/* raw sequence       */
			++rep;
			if (_read(in, buf, rep) < rep) 
			return 0;
			for (j = 0; j < rep; ++j)
			{
				index = (int) buf[j] & 0x0ff;
				memcpy(image + npix * mappix, cmap + index * mappix, mappix);
				++npix;
			}
		} 
		else
		{					/* run               */
			rep -= 127;
			if (_read(in, buf, 1) < 1) 
				return 0;
			index = (int) buf[0] & 0x0ff;
			for(j = 0;j < rep; ++j)
			{
				memcpy(image + npix * mappix, cmap + index * mappix, mappix);
				++npix;
			}
		}
	}

	bytes = mappix;
	type = 2;                  

	return 1;
}

/**********************************************************************/
int BitmapFile::ReadTGAAsMappedCompressed(int in, char header[18])
/**********************************************************************/
{
	uchar buf[1];
	int   somap;
	int   j;
	int   rep, shouldbe, npix;

						/* get colormap       */
	mappix = header[7]/8;
	mapoffset = (0x0ff & (int) header[4]) << 8 |
				(0x0ff & (int) header[3]);
	maplen = (0x0ff & (int) header[6]) << 8 |
			 (0x0ff & (int) header[5]);
	somap = mappix * (mapoffset + maplen);

	if(cmap == (uchar *)0)
	{
		cmap = new uchar[somap]; 
	}
	else if (somap != szMap)
	{
		delete [] cmap;
		cmap = new uchar[somap];
	}
	szMap = somap;
    
	if (_read(in, cmap + mappix * mapoffset, mappix * maplen) < 0) 
		return 0;
						/* get image          */

	shouldbe = width * height;
	image = new uchar[width * height];

	npix = 0;
	while (npix < shouldbe)
	{
		rep = (int)readchar(in) & 0x0ff;
		if(rep < 128)
		{				/* raw sequence       */
			++rep;
			if (_read(in, image + npix, rep) < rep) 
				return 0;
		}
		else
		{					/* run		      */
			rep -= 127;
			if (_read(in, buf, 1) < 1) 
				return 0;
			for(j = 0; j < rep; ++j) 
				memcpy(image + npix + j, buf, 1);
		}
		npix += rep;
	}
	bytes = 1;
	type = 1; 
	return 1;
}

/**********************************************************************/
int BitmapFile::WriteTGAColormapFile(char *name)
/**********************************************************************/
/*
   writes color-mapped (type 1) files
								      */
{
	static char chead[] =   {0x00, 0x01, 0x01, 0x00,  
							/* type(0-2), colormap   */
							 0x00, 0x00, 0x01, 0x18,  
							/* offset, number, length*/
							 0x00, 0x00, 0x00, 0x00,
							/* xorg, yorg            */
							 0x00, 0x00, 0x00, 0x00, 
							/* width, height         */
							 0x08, 0x00};
	Pixel  pix;
	uchar *map;
	int    ncolors, j, k, m;
	int    out;
	int    index;
	uchar *MapImage;

	out = _open(name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
	if(out == -1) 
		return 0;
							 /* record dimensions of image in header */  
	chead[12] = lower(width);
	chead[13] = upper(width);
	chead[14] = lower(height);
	chead[15] = upper(height);
	chead[7] = bytes * 8;
	chead[16] = 8;
	_write(out, chead, 18);

	ncolors = 0;
	map = new uchar[256 * bytes]; 

	for(j = 0; j < 8; ++j)
	{
		for(k = 0; k < 8; ++k)
		{
			for(m = 0; m < 4; ++m)
			{
				pix.r = j << 5;
				pix.g = k << 5;
				pix.b = m << 6;
				CopyRGB((Pixel *)(map + (ncolors * bytes)), pix);
				++ncolors;
			}
		}
	}

	_write(out, map, 256 * bytes);

	MapImage = new uchar[width * height]; 
	if(MapImage == (uchar *)NULL) 
		return 0;

	for(j = 0; j < height; ++j)
	{
		for(k = 0; k < width; ++k)
		{
			pix = GetPixel(k, j);
			index = hash(pix);
			MapImage[j * width + k] = (uchar)index;
		}
	}
	_write(out, MapImage, width * height);     
	delete [] MapImage;
	delete [] map;

	_close(out);
	return 1;
}

/**********************************************************************/
int BitmapFile::WriteTGAMappedFile(char *name)
/**********************************************************************/
/*
   writes color-mapped (type 1) files  Assumes that the tga->cmap
   holds the desired colormap >and< tga->image holds indices to
   colormap
								      */
{
	static char chead[] =   {0x00, 0x01, 0x01, 0x00,  
							/* type(0-2), colormap   */
							 0x00, 0x00, 0x01, 0x18,  
							/* offset, number, length*/
							 0x00, 0x00, 0x00, 0x00,
							/* xorg, yorg            */
							 0x00, 0x00, 0x00, 0x00, 
							/* width, height         */
							 0x08, 0x00};

	char  msg[256];
	int   ncolors;
	int   out;

	if(type != 1)
	{ 
		sprintf(msg,"%s is not stored in memory as colormapped file.\n%s",
				name, "WriteTGAMappedFile can't write it\n");
		fatalerror(msg,2);
		return 0;
	}

	out = _open(name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
	if(out == -1)  
		return 0;

	chead[3] = lower(mapoffset);
	chead[4] = upper(mapoffset);
	chead[5] = lower(maplen);
	chead[6] = upper(maplen);
                             /* record dimensions of image in header */  
	chead[12] = lower(width);
	chead[13] = upper(width);
	chead[14] = lower(height);
	chead[15] = upper(height);
	chead[7] = mappix * 8;
	chead[16] = 8;
	_write(out, chead, 18);
  
	ncolors = 0;
	_write(out, cmap + mapoffset * mappix, maplen * mappix);
	_write(out, image, width * height);     
	_close(out);
	return 1;
}

/**********************************************************************/
int BitmapFile::WriteTGACompressedMappedFile(char *name)
/**********************************************************************/
/*
   writes compressed colormapped (type 9) files  Assumes that the 
   tga->cmap holds the desired colormap and tga->image holds indices 
   to colormap
								      */
{
	static char chead[] =   {0x00, 0x01, 0x09, 0x00,  
							/* type(0-2), colormap   */
							 0x00, 0x00, 0x01, 0x18,  
							/* offset, number, length*/
							 0x00, 0x00, 0x00, 0x00,
							/* xorg, yorg            */
							 0x00, 0x00, 0x00, 0x00, 
							/* width, height         */
							 0x08, 0x00};

	int           out;
	long          nraw, run;
	int           x, y;
	uchar last, current;
	uchar raster[128];
	char  msg[256];

	if(type != 1)
	{
		sprintf(msg,"%s is not stored in memory as colormapped file\n%s",
				name, "WriteTGACompressedMappedFile can't write it\n");
		fatalerror(msg,2);
		return 0;
	}
	out = _open(name, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
	if(out == -1) 
		return 0;
			     /* colormap statistics */
	chead[3] = lower(mapoffset);
	chead[4] = upper(mapoffset);
	chead[5] = lower(maplen);
	chead[6] = upper(maplen);
                             /* record dimensions of image in header */  
	chead[12] = lower(width);
	chead[13] = upper(width);
	chead[14] = lower(height);
	chead[15] = upper(height);
	chead[7] = mappix * 8;
	chead[16] = 8;
	_write(out, chead, 18);
 			    /* write colormap */ 
	_write(out, cmap + mappix * mapoffset, maplen * mappix);

	run = 0;
	nraw = 0;
	last = (uchar)GetIndex(0, 0);

	for(y = 0; y < height; ++y)
	{
		for(x = 0; x < width; ++x)
		{
			current = (uchar)GetIndex(x, y);
			if (current == last)
			{
				if (run < 128)
					++run;
				else
				{		/* repeat count too big -- output run  */
				        /* and any preceding raw sequence      */
					if (nraw) 
						flushbyteraw(out,raster,nraw); 
					nraw = 0;
					flushbyterun(out, run, last);
					run = 1;
				}
			}
			else 
			{			/* different pixel.                    */
				if (run > 2)
				{        /* repeat count big enough to encode as*/
						 /* run.  Output any raw sequence first */
					if(nraw) 
						flushbyteraw(out, raster, nraw);
					nraw = 0;
					flushbyterun(out, run, last);
				}
				else
				{               /* dinky run. encode as raw sequence   */
								/* if raw sequence is too long, output */
								/* a raw sequence before encoding raw  */
					if(run + nraw > 127) 
					{
						if (nraw) 
							flushbyteraw(out, raster, nraw);
						nraw = 0;
					}
					addbyteraw(raster, nraw, last, run);
					nraw += run;	
				}
				run = 1;            /* we have a new run of one            */
				last = current;
			}
		}
	} /* end for */

	if(run > 2)
	{
		if (nraw) 
			flushbyteraw(out, raster, nraw);
		nraw = 0;
		flushbyterun(out, run, last);
	} 
	else
	{
		if(run + nraw > 127)
		{
			if (nraw) 
				flushbyteraw(out, raster, nraw);
			nraw = 0;
		}
		addbyteraw(raster, nraw, last, run);
		nraw += run;
		flushbyteraw(out, raster, nraw);
	}
	_close(out);
	return 1;
}

/******************************************************************/
int BitmapFile::hash(Pixel pix)
/******************************************************************/
{
	int value;

	value = (pix.r & 0x0e0) ;
	value = value | (((int) pix.g & 0x0e0) >> 3);
	value = value | (((int) pix.b & 0x0c0) >> 6);
	return value;
}

/******************************************************************/
int BitmapFile::flushbyterun(int out, int run,uchar last)
/******************************************************************/
{
	uchar packet[2];

	packet[0] = (uchar) (run + 127);
	packet[1] = last;
	_write(out,packet,2);
	return 1;
}

/******************************************************************/
int BitmapFile::flushbyteraw(int out, uchar raster[128],int nraw)
/******************************************************************/
{
	uchar count[1];

	count[0] = (uchar) (nraw -1);
	_write(out,count,1);
	_write(out,raster,nraw);
	return 1;
}

/******************************************************************/
int BitmapFile::addbyteraw(uchar raster [128],int n,uchar index,int rep)
/******************************************************************/
{
	int j;

	for(j=0;j<rep;++j)
		raster[n+j] = index;
	return 1;
} 
