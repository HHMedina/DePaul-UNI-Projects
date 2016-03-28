
#include "AVIFile.h"
#include "jmisc.h"

AVIFile::AVIFile()
{
	CoInitialize(NULL);  // Get COM up and running

	strcpy(fileName, "\0");
	curFrame = 0;
	imageData = NULL;
	aviFile = NULL;
	aviStream = NULL;
	aviCompressedStream = NULL;

	AVIFileInit();  // Start up the AVIFileEngine in Windows
}

AVIFile::~AVIFile()
{
	if (aviFile != NULL)
		Close();

	CoUninitialize();  // Tell COM that we are finished
}

void AVIFile::Open(const char *FileName, Bitmap &format, int frameRate)
{
	if (aviFile != NULL)
		Close();

	strcpy(fileName, FileName);
	if (AVIFileOpen(&aviFile, fileName, OF_CREATE | OF_WRITE, NULL) != S_OK)
		complain("Unable to create the avi file\n");

	memset(&aviStreamInfo, 0, sizeof(AVISTREAMINFO));
	aviStreamInfo.fccType = streamtypeVIDEO;
	aviStreamInfo.fccHandler = mmioFOURCC('M','P','4','2');
	aviStreamInfo.dwScale = 1;
	aviStreamInfo.dwRate = frameRate;
	aviStreamInfo.dwQuality = -1;
	aviStreamInfo.dwSuggestedBufferSize = format.Width() * format.Height() * 4;
	SetRect(&aviStreamInfo.rcFrame, 0, 0, format.Width(), format.Height());
	strcpy(aviStreamInfo.szName, "Video");

	if (AVIFileCreateStream(aviFile, &aviStream, &aviStreamInfo) != S_OK)
		complain("Unable to create stream\n");

	memset(&aviCompressOptions, 0, sizeof(AVICOMPRESSOPTIONS));
	aviCompressOptions.fccType = streamtypeVIDEO;
	aviCompressOptions.fccHandler = aviStreamInfo.fccHandler;
	aviCompressOptions.dwFlags = AVICOMPRESSF_VALID;

	if (AVIMakeCompressedStream(&aviCompressedStream, aviStream, &aviCompressOptions, NULL) != S_OK)
		complain("Unable to create compressed stream\n");

	BITMAPINFO bitInfo = format.GetBitmapInfo();
	if (AVIStreamSetFormat(aviCompressedStream, 0, &bitInfo, bitInfo.bmiHeader.biSize) != S_OK)
		complain("Unable to set avi format\n");

	imageData = new unsigned char[format.Width() * format.Height() * 4];

	curFrame = 0;
}

void AVIFile::AddFrame(Bitmap &frame)
{
	BITMAPINFO bitInfo = frame.GetBitmapInfo();
	frame.GetBitmapData(imageData);
	if (AVIStreamWrite(aviCompressedStream, curFrame, 1, imageData, bitInfo.bmiHeader.biSizeImage, 0, NULL, NULL) != S_OK)
		complain("Couldn't write frame %d to avi file\n", curFrame);
	curFrame++;
}

void AVIFile::Close(void)
{
	if (aviFile)
	{
		AVIStreamRelease(aviCompressedStream);
		aviCompressedStream = NULL;

		AVIStreamRelease(aviStream);
		aviStream = NULL;

		AVIFileRelease(aviFile);
		aviFile = NULL;

		AVIFileExit();

		delete [] imageData;
	}
}