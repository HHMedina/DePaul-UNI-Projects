#ifndef SOUNDCONSCIOUS_H
#define SOUNDCONSCIOUS_H

//#include "Vect.h"
//#include "SoundSystem.h"
//#include "irrKlang.h"
#include "SoundConsciousListeningState.h"
#include "NotListeningState.h"
#include "ListeningState.h"

class SoundConscious{

	friend class SoundSystem;

public:
	
	/** \ingroup SNDCON
	\brief returns the SoundConscious's position.

	*/
	virtual const Vect& GetSoundConsciousPosition(){
		return sndConPos;
	}

	/** \ingroup SNDCON
	\brief returns the SoundConscious's direction.

	*/
	virtual const Vect& GetSoundConsciousDirection(){
		return sndConDirection;
	}

	/** \ingroup SNDCON
	\brief Sets the SoundConsious as the listener
	This method will set this sound conscious object as the world' listener.
	\note a world can only have one listener so the previous listener is demoted.
	*/
	void SetAsListener(){
		SoundSystem::SetListener(this);
	}

protected:
	SoundConscious(){
		myState = &NotListeningState::GetInstance();//Default state is not Listening
	}
	SoundConscious(const SoundConscious&){}
	SoundConscious& operator = (const SoundConscious&){}
	virtual ~SoundConscious(){}
	
	virtual void SetSndConciousPos(Vect& newPos){
		sndConPos = newPos;
	}

	virtual void SetToNotListening(){
		myState = &NotListeningState::GetInstance();
	}

	virtual void SetToListening(){
		myState = &ListeningState::GetInstance();
	}

	virtual void SetSndConciousDir(Vect& newDir){
		sndConDirection = newDir;
	}

	virtual void UpdatePosAndDir(){
		myState->Update(sndConPos,sndConDirection);
	}
	//Data
	Vect sndConPos;
	Vect sndConDirection;

	//every soundConscious Object is either a listener or not
	SoundConsciousListeningState* myState;
};
#endif