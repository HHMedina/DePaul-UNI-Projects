#ifndef CAMERA_MAN_H
#define CAMERA_MAN_H

#include "Camera.h"

class CameraMan
{
public:
	static Camera *GetCurrent();
	~CameraMan();

private:
	static CameraMan *privInstance();
	CameraMan();
	

	// Data
	Camera *pCam;
};

#endif