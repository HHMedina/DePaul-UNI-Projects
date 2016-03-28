#ifndef __AVIFILE_H__
#define __AVIFILE_H__

#include "windows.h"
#include <vfw.h>
#include "Bitmap.h"

class AVIFile
{
public:

	AVIFile();
	~AVIFile();

	void Open(const char *FileName, Bitmap &format, int FrameRate);
	void AddFrame(Bitmap &frame);
	void Close();

private:

	char			    fileName[MAX_PATH];
	int					curFrame;
	unsigned char	   *imageData;
	PAVIFILE			aviFile;
	PAVISTREAM			aviStream;
	PAVISTREAM			aviCompressedStream;
	AVISTREAMINFO		aviStreamInfo;
	AVICOMPRESSOPTIONS	aviCompressOptions;
};

#endif