#ifndef NOSOUNDSYSTEM_H
#define NOSOUNDSYSTEM_H
#include "SoundSystem.h"

#include "TEAL\CommonElements.h"

class NoSoundSystem: public SoundSystem{

public:

	NoSoundSystem(){} //prevent constructor
	NoSoundSystem(const NoSoundSystem& ){}//prevent Copy Constructor
	NoSoundSystem& operator = (const NoSoundSystem& ){}//prevent Assignment
	~NoSoundSystem();

private:

	//Functions that play a sound
	virtual void PlayBlasterSound(){}
	virtual void PlayScorpionSound(){}
	virtual void PlayEnemyDeathSound(){}
	virtual void PlayPlayerDeathSound(){}
	virtual void PlayFleaSound(){}
	virtual void PlaySpiderSound(){}
	virtual void PlayCentipedeSound(){}

	//functions that stop a sound
	virtual void StopSpiderSound(){}
	virtual void StopScorpionSound(){}
	virtual void StopCentipedeSound(){}
	virtual void StopAllSounds(){}

};

#endif NOSOUNDSYSTEM_H