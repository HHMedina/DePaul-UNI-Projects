#ifndef CAMERA_MAN_H
#define CAMERA_MAN_H

#include "Camera.h"

/**

Note: Modded Original Azul CamMan


*/

class CameraMan
{
public:
	static Camera *GetCurrent();
	~CameraMan();

	/*
	Added these function to allow P2 Engine's Camera Manager more
	Functionality.
	*/

	static void SetCurrentCamera(Camera* );
	static Camera*  CameraMan::GetDefaultCamera();

	//end of additions
private:
	static CameraMan *privInstance();
	CameraMan();
	

	// Data
	Camera *pCam;//pointer to default camera

	Camera* currentCamera;
};

#endif