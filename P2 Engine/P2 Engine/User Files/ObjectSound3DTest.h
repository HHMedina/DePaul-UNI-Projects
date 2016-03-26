#ifndef OBJECTSOUND3DTEST_H
#define OBJECTSOUND3DTEST_H
#include "P2Engine.h"

class ObjectSound3DTest : public GameObject{
public:
	ObjectSound3DTest(){
		SetAudibleSound("GetOutBGM");
		PlayAudibleSound();
		mySound =  AssetManager::GetSound3DInstance("GetOutBGM"); //new Sound2D(AssetManager::GetSoundSource("TankShotSound"));
		mySound->SetVolume(1);
		//mySound->SetLooped(true);
		mySound2 =  AssetManager::GetSound3DInstance("GetOutBGM");
		
		mySound2->SetVolume(1.0f);
		//mySound2->Play();
		//mySound->Play();
		//mySound2->SetLooped(true);
		pos = Vect(0,0,0);
		//SoundSystem::GetSoundEngine()->play3D("../assets/tankshot.flac",vec3df(0,0,0),false,false,true);
		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture( AssetManager::GetModel("TankShellModel"),AssetManager::GetTexture("BombTexture")));
		SetGameObjectWorld(pos,Vect(1,1,1),Vect(0,0,0));
		SetAlarm(AlarmID_0,5);
		
	}

	void Alarm0(){
		MarkForDestroy();
	}

	void OnTerminate(){
		mySound->Release();
		mySound2->Release();
	}

	void Update(){

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_1)){
		//	mySound->Play();
		}
		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_2)){
		//	mySound2->Play();
		}
		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_3)){
		//	mySound->SetVolume(1.0f);
		}
		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_4)){
		//	mySound->SetVolume(0.1f);
		}
		pos[x] = pos[x] +.10f;
		SetGameObjectWorld(pos,Vect(1,1,1),Vect(0,0,0));
		mySound->SetSoundPosition(pos);//This is what is causing the problem!!!!!!!! 
		/*
		Note:: maybe it is accessing a dead sound. 
		*/
	}

	void Draw(){
		RenderGraphics();
	}
	Sound3D* mySound;
	Sound3D* mySound2;
	Vect pos;
};


#endif