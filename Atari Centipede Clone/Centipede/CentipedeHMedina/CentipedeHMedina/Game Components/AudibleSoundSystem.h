#ifndef AUDIBLESOUNDSYSTEM_H
#define AUDIBLESOUNDSYSTEM_H
#include "SoundSystem.h"

#include "TEAL\CommonElements.h"

class AudibleSoundSystem: public SoundSystem{

public:

	AudibleSoundSystem(); //prevent constructor
	AudibleSoundSystem(const AudibleSoundSystem& ){}//prevent Copy Constructor
	AudibleSoundSystem& operator = (const AudibleSoundSystem& ){}//prevent Assignment
	~AudibleSoundSystem();

private:

	//Functions that play a sound
	virtual void PlayBlasterSound();
	virtual void PlayScorpionSound();
	virtual void PlayEnemyDeathSound();
	virtual void PlayFleaSound();
	virtual void PlayPlayerDeathSound();
	virtual void PlaySpiderSound();
	virtual void PlayCentipedeSound();

	//functions that stop a sound
	virtual void StopSpiderSound();
	virtual void StopScorpionSound();
	virtual void StopCentipedeSound();
	virtual void StopAllSounds();

	//All of the sounds in this Sound System
	sf::Sound FleaSound;
	sf::Sound ScorpionSound;
	sf::Sound BlasterSound;
	sf::Sound EnemyDeathSound;
	sf::Sound PlayerDeathSound;
	sf::Sound CentipedeBeatSound;
	sf::Sound SpiderSound;
	sf::Sound CentipedeSound;
};

#endif AUDIBLESOUNDSYSTEM_H