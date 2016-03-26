#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "TEAL\CommonElements.h"

class SoundSystem{

	friend class SoundManager;

public:

	SoundSystem(){} //prevent constructor
	SoundSystem(const SoundSystem& ){}//prevent Copy Constructor
	SoundSystem& operator = (const SoundSystem& ){}//prevent Assignment
	~SoundSystem(){};

private:

	//functions to call playing a sound
	virtual void PlayBlasterSound()=0;
	virtual void PlayScorpionSound()=0;
	virtual void PlayEnemyDeathSound()=0;
	virtual void PlayPlayerDeathSound()=0;
	virtual void PlayFleaSound()=0;
	virtual void PlaySpiderSound()=0;
	virtual void PlayCentipedeSound()=0;
	
	//Functions to stop playing a sound
	virtual void StopSpiderSound()=0;
	virtual void StopScorpionSound()=0;
	virtual void StopCentipedeSound()=0;
	virtual void StopAllSounds()=0;
};

#endif SOUNDSYSTEM_H