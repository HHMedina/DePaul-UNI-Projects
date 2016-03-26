#ifndef CAMERAOBJECTFOLLOW_H
#define CAMERAOBJECTFOLLOW_H
#include "P2Engine.h"
class CameraObjectFollow: public GameObject{
public:

	CameraObjectFollow(Vect& pos,Vect& lookAt,float spd = 1.0f){
		speed = spd;
		position = pos;
		//CameraMan::GetCurrent()->set
		CameraMan::GetCurrent()->setOrientAndPosition( 
													Vect(0,1,0),
													Vect(0,0,0) - position, 
													Vect(0,4,-10));
		CameraMan::GetCurrent()->updateCamera();
	}

	void Update(){
		
		Matrix tempR = Matrix();
		tempR.set(ROT_XYZ,rotation[x],rotation[y],rotation[z]);

		if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_W) )
		{
			position += (Vect(0,0,1) *tempR).getNorm() *speed;
		}
		if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_S) )
		{
			position -= (Vect(0,0,1) *tempR).getNorm() *speed;
		}
	}

	Vect position;
	Vect rotation;
	float speed;
};

#endif 