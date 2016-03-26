#include "CameraMan.h"



Camera * CameraMan::GetCurrent()
{
	return CameraMan::privInstance()->pCam;
}

CameraMan *CameraMan::privInstance()
{
	static CameraMan camMan;
	return &camMan;
}

CameraMan::CameraMan()
{
	this->pCam = new Camera();
}

CameraMan::~CameraMan()
{
	delete this->pCam;
}