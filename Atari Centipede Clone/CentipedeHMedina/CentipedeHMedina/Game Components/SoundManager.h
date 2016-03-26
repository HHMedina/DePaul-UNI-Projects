#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <string>
#include "TEAL\CommonElements.h"

class SoundSystem;

class SoundManager{

private:
	
	static SoundManager * instance;//set up Singleton

	SoundManager();//prevent constructor
	SoundManager(const SoundManager& other){}//prevent copy constructor
	SoundManager& operator =(const SoundManager& other){}//prevent assigment
	~SoundManager();

	static SoundManager& Instance(){
		if(!instance){
			instance = new SoundManager();
		}
		return *instance;
	}

	SoundSystem * soundSys; //composition of the kinds of sounds (muted or not muted)

	SoundSystem * AudibleSystem;
	SoundSystem * SilentSystem;

public:

	//sets the composed soundsystem
	static void SetSoundSystem( SoundSystem& ); //using a SoundSystem as parameter avoid having if statements when setting sound system
	static void SetToSilentMode();
	static void SetToAudibleMode();

	//call the sound to play in the composed soundsytem
	static void PlayBlasterSound();
	static void PlayScorpionSound();
	static void PlayEnemyDeathSound();
	static void PlayPlayerDeathSound();
	static void PlayFleaSound();
	static void PlaySpiderSound();
	static void PlayCentipedeSound();

	//calls the sound to stop it from playing (used for looping sounds)
	static void StopSpiderSound();
	static void StopScorpionSound();
	static void StopCentipedeSound();
	static void StopAllSounds();
};

#endif SOUNDMANAGER_H