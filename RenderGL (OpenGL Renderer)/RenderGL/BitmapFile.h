#ifndef __TARGA_H__
#define __TARGA_H__

// Implementation for 
 
typedef unsigned char uchar;

struct Pixel
{
	uchar b, g, r, alpha;
};

class BitmapFile
{
public:

	// Constructors and Destructors
	BitmapFile(int width = 0, int height = 0, bool mapped = false);
	~BitmapFile();

	void Create(int w, int h);
	void CreateMapped(int w, int h);

	int Read(char *name);
	int Write(char *name);

	// Targa(.tga) File Utilities: See BitmapFile.cpp
	int ReadTGA(char *name);
	int WriteTGA(char *name);
	int WriteTGA24(char *name);
	int WriteTGA10(char *name);

	// The following colormapped versions may be found in TargaMapped.cpp
	int WriteTGAColormapFile(char *name);
	int WriteTGAMappedFile(char *name);
	int WriteTGACompressedMappedFile(char *name);

	// Bitmap(.bmp) File Utilities: See BitmapFile.cpp
	int ReadBMP(char *name);
	int WriteBMP(char *name);

	// Pixel Operations
	Pixel GetPixel(int x, int y);
	void  PutPixel(int x, int y, Pixel pix);
	// Pixel Operations for colormapped bitmaps
	int   GetIndex(int x, int y);
	void  PutIndex(int x, int y, int index);
	unsigned char* ImageData(void) { return image; }

	uchar  Red(int x, int y);
	uchar  Green(int x, int y);
	uchar  Blue(int x, int y);
	uchar  Alpha(int x, int y);
	
	// Bitmap Statistic Utilities
	long Width() { return width; }
	long Height() { return height; }
	int BytesPerPixel() { return bytes; }
	const char *Name() { return name; }

	// The following function should be used when the user needs to 
	// Have RGB data.  The user is then responsible for calling this
	// function again to put the data back into BGR order.
	void ReverseColors(void);
	void Resize(int w, int h);

	// The following functions are for editing the image
	// They allow for some simple image functions
	void Filter(int r, int g, int b, int a = 255);
	void Filter(double r, double g, double b, double a = 1.0);
	void Add(int r, int g, int b, int a = 0);
	void Add(double r, double g, double b, double a = 0.0);

	// These utility functions are for copying pixels.
	static void CopyRGB(Pixel *t, Pixel f);
	static void CopyRGBA(Pixel *t, Pixel f);

private:

	int imloc(int j, int k)  { return (j * width + k) * bytes; }
	int loc24(int j, int k)  { return (j * width + k) * 3; }
	uchar upper(int x)  { return (uchar)((x >> 8) & 0x00ff); }
	uchar lower(int x)  { return (uchar)(x & 0x00ff); }

	int readchar(int in);
	int ReadTGA10File(int in);
	int ReadTGAColormapFile(int in, char header[18]);
	int ReadTGAAsMapped(int in, char header[18]);
	int ReadTGACompressedColormapFile(int in, char header[18]);
	int ReadTGAAsMappedCompressed(int in, char header[18]);

	int ReadBMP1File(int in);
	int ReadBMP4File(int in);
	int ReadBMP8File(int inp, bool OS2, int offset);
	int ReadBMP16File(int in, bool bitmasks, int offset);
	int ReadBMP24File(int inp, int offset);
	int ReadBMP32File(int in, bool bitmasks, int offset);

	static int flushrun(int out, int run, Pixel last);
	static int flushraw(int out, Pixel pix[128], int nraw);
	static int addraw(Pixel pix[128], int n, Pixel apix, int rep);

	static int hash(Pixel pix);
	static int flushbyterun(int out, int run, uchar last);
	static int flushbyteraw(int out, uchar raster[128],int nraw);
	static int addbyteraw(uchar raster [128], int n, uchar index, int rep);

	int  match(Pixel x, Pixel y);
	void fatalerror(const char *msg, int severity);

	char     name[80];           /* pathname of targa file          */

	long     width;              /* image width                     */
	long     height;             /* image height                    */
	int      bytes;              /* bytes per pixel                 */
	int      type;               /* == 1 ->color map; == 2 ->no map */
	uchar   *image;              /* pointer to image data (pixels)  */
	long     szImage;            /* size of image data              */
	long     PixelsPerMeterX;	 /* Physical scale of bitmap		*/
	long     PixelsPerMeterY;    

	uchar   *cmap;               /* pointer to colormap(if present) */
	long     szMap;              /* size of the colormap data       */
	int      mappix;             /* bytes per color map entry       */
	int      mapoffset;          /* first element of colormap       */
	int      maplen;             /* elements in colormap            */

	bool	 rgb;
};

#endif

