/*
#ifndef SOUNDTEST1_H
#define SOUNDTEST1_H
#include "P2Engine.h"
#include "SoundSystem.h"


class MySoundEndReceiver : public irrklang::ISoundStopEventReceiver
 {
   public:
     virtual void OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData)
     {
        // called when the sound has ended playing
		 cout<<sound->setPlayPosition(0)<<" ";
        printf("sound has ended");
		cout<<endl;
     }
 };

class SoundTest1:public GameObject{
public:

	SoundTest1(){
		mySoundReciever = new MySoundEndReceiver();
		RegisterForInput(AZUL_KEY::KEY_1);
		RegisterForInput(AZUL_KEY::KEY_2);
		RegisterForInput(AZUL_KEY::KEY_3);
		RegisterForInput(AZUL_KEY::KEY_4);
		mySoundSource = SoundSystem::GetSoundEngine()->addSoundSourceFromFile("../Assets/TankShot.flac");
	}

	virtual void KeyDown(AZUL_KEY key){
		if(key == AZUL_KEY::KEY_1){
			sound1 = SoundSystem::GetSoundEngine()->play2D(mySoundSource,false,false,true);
			sound1->setIsPaused(false);
			sound1->setSoundStopEventReceiver(mySoundReciever);
		}

		if(key == AZUL_KEY::KEY_2){

			sound2 = SoundSystem::GetSoundEngine()->play2D(mySoundSource,false,false,true);
			sound2->setIsPaused(false);
		}

		if(key == AZUL_KEY::KEY_3){
			bool success = sound1->setPlayPosition(0.0001);
			if(success){
				cout<<"success replay";
			}
		}

		if(key == AZUL_KEY::KEY_4){
			sound1->drop();
			sound2->drop();
		}
	}

	ISoundSource* mySoundSource;
	ISound * sound1;
	ISound * sound2;

	MySoundEndReceiver* mySoundReciever;
};



#endif 
*/