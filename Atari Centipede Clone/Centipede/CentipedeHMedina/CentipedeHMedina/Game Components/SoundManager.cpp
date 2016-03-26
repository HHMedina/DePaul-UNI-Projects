#include "SoundManager.h"
#include "AudibleSoundSystem.h"
#include "SoundSystem.h"
#include "NoSoundSystem.h"

SoundManager * SoundManager::instance =NULL;

SoundManager::SoundManager(){
	AudibleSystem = new AudibleSoundSystem();
	SilentSystem = new NoSoundSystem();

	soundSys= AudibleSystem; // Set SoundSys to Audible By Default


}

SoundManager::~SoundManager(){ 

	//clean up dynamically allocated memory
	delete AudibleSystem;
	delete SilentSystem;
}

void SoundManager::SetSoundSystem( SoundSystem& newSystem){

	Instance().soundSys->StopAllSounds(); //stop all sounds

	
	Instance().soundSys = &newSystem; //points the sound system to the new system of sound

}

void SoundManager::SetToSilentMode(){


	Instance().soundSys->StopAllSounds(); //stop all sounds

	Instance().soundSys = Instance().SilentSystem; //points the sound system to the new system of sound

}

void SoundManager::SetToAudibleMode(){


	Instance().soundSys->StopAllSounds(); //stop all sounds

	Instance().soundSys = Instance().AudibleSystem; //points the sound system to the new system of sound

}

void SoundManager::PlayBlasterSound(){
	Instance().soundSys->PlayBlasterSound();
}

void SoundManager::PlayScorpionSound(){
	Instance().soundSys->PlayScorpionSound();
}

void SoundManager::PlayEnemyDeathSound(){
	Instance().soundSys->PlayEnemyDeathSound();
}

void SoundManager::PlayFleaSound(){
	Instance().soundSys->PlayFleaSound();
}

void SoundManager::PlayPlayerDeathSound(){
	Instance().soundSys->PlayPlayerDeathSound();
}

void SoundManager::PlaySpiderSound(){
	Instance().soundSys->PlaySpiderSound();
}

void SoundManager::PlayCentipedeSound(){
	Instance().soundSys->PlayCentipedeSound();
}

void SoundManager::StopSpiderSound(){
	Instance().soundSys->StopSpiderSound();
}

void SoundManager::StopScorpionSound(){
	Instance().soundSys->StopScorpionSound();
}

void SoundManager::StopCentipedeSound(){
	Instance().soundSys->StopCentipedeSound();
}

void SoundManager::StopAllSounds(){
	Instance().soundSys->StopAllSounds();
}