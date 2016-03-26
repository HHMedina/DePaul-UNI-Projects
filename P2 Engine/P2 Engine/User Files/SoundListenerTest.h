#ifndef SOUNDLISTENERTEST_H
#define SOUNDLISTENERTEST_H

#include "P2Engine.h"

class SoundLisenerTest:public GameObject{
public:
	SoundLisenerTest(){
		
		SetAsListener();
		RegisterForInput(AZUL_KEY::KEY_W);
		RegisterForInput(AZUL_KEY::KEY_A);
		RegisterForInput(AZUL_KEY::KEY_S);
		RegisterForInput(AZUL_KEY::KEY_D);
		RegisterForInput(AZUL_KEY::KEY_Q);
		RegisterForInput(AZUL_KEY::KEY_E);
		RegisterForInput(AZUL_KEY::KEY_C);
		RegisterForInput(AZUL_KEY::KEY_Z);
		myPos = Vect(0,0,0);
		myRot = Vect(P2Math::DegToRad(0),P2Math::DegToRad(0),P2Math::DegToRad(0));
		SetGraphicsObject(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("BombTexture")));
		SetGameObjectWorld(myPos,Vect(.2,.2,.2),myRot);
	}

	void Draw(){
		RenderGraphics();
	}

	void Update(){
		CameraMan::GetCurrent()->setOrientAndPosition(Vect(0,1,0),Vect(0,0,0), Vect(0,20,40));
		CameraMan::GetCurrent()->updateCamera();

		
		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_A)){
			myPos += Vect(-7*TimeManager::GetFrameTime(),0,0);

		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_D)){
			myPos += Vect(7*TimeManager::GetFrameTime(),0,0);

		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_W)){

			myPos += Vect(0,0,7*TimeManager::GetFrameTime());
		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_S)){

			myPos += Vect(0,0,-7*TimeManager::GetFrameTime());
		}
		
		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_Q)){

			myRot += Vect(0,-7*TimeManager::GetFrameTime(),0);
		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_E)){

			myRot += Vect(0,7*TimeManager::GetFrameTime(),0);
		}


		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_Z)){

			myRot += Vect(-7*TimeManager::GetFrameTime(),0,0);
		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_C)){

			myRot += Vect(7*TimeManager::GetFrameTime(),0,0);
		}
				
		SetGameObjectWorld(myPos,Vect(0.2f,0.2f,0.2f),myRot);
	}

	void KeyDown(AZUL_KEY key){


	}


	Vect myPos;
	Vect myRot;
};
#endif