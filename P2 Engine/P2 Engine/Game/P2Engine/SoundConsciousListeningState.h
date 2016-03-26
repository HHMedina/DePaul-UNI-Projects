#ifndef SOUNDCONSCIOUSLISTENINGSTATE_H
#define SOUNDCONSCIOUSLISTENINGSTATE_H
#include "irrKlang.h"
#include "Vect.h"

class SoundConsciousListeningState{
	friend class SoundConscious;
	friend class DefaultSoundConscious;
protected:
	SoundConsciousListeningState(){}
	SoundConsciousListeningState(const SoundConsciousListeningState&){}
	const SoundConsciousListeningState& operator = (const SoundConsciousListeningState&){}
	virtual ~SoundConsciousListeningState(){}

	virtual void Update(Vect&,Vect&)=0;
};

#endif