#include "CameraMan.h"


/*
modded to get current Camera
*/
Camera * CameraMan::GetCurrent()
{
	return CameraMan::privInstance()->currentCamera;
}

/*
added... method to get default Camera

*/
Camera*  CameraMan::GetDefaultCamera(){
	return CameraMan::privInstance()->pCam;
}

void CameraMan::SetCurrentCamera(Camera* toSet){
	CameraMan::privInstance()->currentCamera= toSet;
}

CameraMan *CameraMan::privInstance()
{
	static CameraMan camMan;
	return &camMan;
}

CameraMan::CameraMan()
{
	this->pCam = new Camera();//default camera
	currentCamera = this->pCam;//set current to default
}

CameraMan::~CameraMan()
{
	delete this->pCam;//delete the default camera
}