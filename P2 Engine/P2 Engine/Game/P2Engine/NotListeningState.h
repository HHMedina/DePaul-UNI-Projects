#ifndef NOTLISTENINGSTATE_H
#define NOTLISTENINGSTATE_H
#include "SoundConsciousListeningState.h"

class NotListeningState: public SoundConsciousListeningState{
	friend class SoundConscious;
	friend class SoundSystem;
	friend class DefaultSoundConscious;
protected:

	
	static NotListeningState& GetInstance(){
		if(!instance){
			instance = new NotListeningState();
		}
		return *instance;
	}

	static NotListeningState* instance;//this is a singleton

	NotListeningState(){}
	NotListeningState(const NotListeningState&){}
	const NotListeningState& operator = (const NotListeningState&){}
	virtual ~NotListeningState(){instance =0;}

	static void DeleteMe(){
		delete GetInstance().instance;
	}

	virtual void Update(Vect& ,Vect& ){}//OnlyListenerUpdatesPosition
};

#endif