#ifndef SOUNDTESTER_H
#define SOUNDTESTER_H
#include "AssetManager.h"
#include "Sound2D.h"
#include "Sound3D.h"
#include "TimeManager.h"
#include "ObjectSound3DTest.h"


class SoundTester:public GameObject{
public:
	SoundTester(int mode){
		if(mode ==0){
			SetAudibleSound("GetOutBGM");
		}
		if(mode ==1){
			//SetAudibleSound("TankShotSound");
			//DeregisterForUpdate();
			//RegisterToDraw();
		}
		//ISAudibleSoundPlaying();
		//PauseAudibleSound();
		//StopAudibleSound();
		SetAudibleLooped(true);
		//SetAudibleSoundMinDistance(.5f);
		PlayAudibleSound();
		//SoundSystem::GetSoundEngine()->setListenerPosition(vec3df(0,0,0), vec3df(0,0,1));
		//AssetManager::GetSoundInstance("GetOutBGM")->Play();
		blaster = AssetManager::GetSound3DInstance("TankShotSound");
		BGM = AssetManager::GetSound3DInstance("GetOutBGM");
		//BGM->SetVolume(0.5f);
		//blaster->SetVolume(0.5f);
		RegisterForInput(AZUL_KEY::KEY_1);
		RegisterForInput(AZUL_KEY::KEY_2);
		RegisterForInput(AZUL_KEY::KEY_3);
		RegisterForInput(AZUL_KEY::KEY_4);
		RegisterForInput(AZUL_KEY::KEY_5);
		RegisterForInput(AZUL_KEY::KEY_6);
		RegisterForInput(AZUL_KEY::KEY_7);
		RegisterForInput(AZUL_KEY::KEY_8);
		RegisterForInput(AZUL_KEY::KEY_9);
		RegisterForInput(AZUL_KEY::KEY_0);
		RegisterForInput(AZUL_KEY::KEY_MINUS);
		
		RegisterForInput(AZUL_KEY::KEY_O);
		RegisterForInput(AZUL_KEY::KEY_P);
		RegisterForInput(AZUL_KEY::KEY_K);
		RegisterForInput(AZUL_KEY::KEY_L);

		RegisterForInput(AZUL_KEY::KEY_LEFT);
		RegisterForInput(AZUL_KEY::KEY_UP);
		RegisterForInput(AZUL_KEY::KEY_DOWN);
		RegisterForInput(AZUL_KEY::KEY_RIGHT);
		RegisterForInput(AZUL_KEY::KEY_KP_8);//up direction
		RegisterForInput(AZUL_KEY::KEY_KP_2);//down direction

		RegisterForInput(AZUL_KEY::KEY_SPACE);

		SetGraphicsObject(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("BombModel"),AssetManager::GetTexture("BombTexture")));

		SetGameObjectWorld(Vect(0,0,0),Vect(.2f,.2f,.2f),Vect(0,0,0));

		
		CameraMan::GetCurrent()->setOrientAndPosition(Vect(0,1,0),Vect(0,0,0), Vect(0,20,-40));
		CameraMan::GetCurrent()->updateCamera();

	}

	void Draw(){
		RenderGraphics();
	}

	void Update(){
		//blaster->SetSoundPosition(blasterPos);
		blasterPos+= Vect(7*TimeManager::GetFrameTime(),0,0);

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_LEFT)){
			myPos += Vect(-7*TimeManager::GetFrameTime(),0,0);
			//blaster = AssetManager::GetSound3DInstance("TankShotSound");
			//blaster->Play();
			//blaster->Play();
			//blaster->Play();
			//blaster->Play();
			//SoundSystem::GetSoundEngine()->play3D(AssetManager::GetSoundSource("TankShotSound"),vec3df(0.0f,0.0f,0.0f),false,false);

		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_RIGHT)){
			myPos += Vect(7*TimeManager::GetFrameTime(),0,0);

		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_UP)){

			myPos += Vect(0,0,7*TimeManager::GetFrameTime());
		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_DOWN)){

			myPos += Vect(0,0,-7*TimeManager::GetFrameTime());
		}
		
		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_KP_8)){

			myPos += Vect(0,7*TimeManager::GetFrameTime(),0);
		}

		if(InputManager::IsKeyPressed(AZUL_KEY::KEY_KP_2)){

			myPos += Vect(0,-7*TimeManager::GetFrameTime(),0);
		}
				
		SetGameObjectWorld(myPos,Vect(.2f,.2f,.2f),Vect(0,0,0));
		BGM->SetSoundPosition(myPos);
		blaster->SetSoundPosition(myPos);
	}

	void KeyDown(AZUL_KEY key){

		if(key == AZUL_KEY::KEY_0){
			SetAudibleSoundMinDistance(4);
		}

		if(key == AZUL_KEY::KEY_1){
			blaster->Play();
			blasterPos = Vect(0,0,0);
		}

		if(key == AZUL_KEY::KEY_2){
			blaster->Pause();
		}


		if(key == AZUL_KEY::KEY_3){
			blaster->Stop();
		}

		if(key == AZUL_KEY::KEY_4){

			BGM->Play();
		}

		if(key == AZUL_KEY::KEY_5){
			BGM->Pause();
		}

		if(key == AZUL_KEY::KEY_6){
			BGM->Stop();
		}

		if(key == AZUL_KEY::KEY_SPACE){
			//new TankShell(Vect(0,0,0),Matrix(ROT_XYZ,0,0,1));
			//Sound3D* temp = new Sound3D(AssetManager::GetSoundSource("TankShotSound"));
			//temp->Play
			new ObjectSound3DTest();
		}

		if(key == AZUL_KEY::KEY_MINUS){
			SoundSystem::StopAllSounds();
		}
		if(key == AZUL_KEY::KEY_O){
			blaster->SetLooped(true);
		}
		if(key == AZUL_KEY::KEY_P){
			blaster->SetLooped(false);
		}

		if(key == AZUL_KEY::KEY_K){
			BGM->SetLooped(true);
		}
		if(key == AZUL_KEY::KEY_L){
			BGM->SetLooped(false);
		}

	}

	Sound3D* blaster;
	Sound3D* BGM;

	Vect myPos;
	Vect blasterPos;
};
#endif