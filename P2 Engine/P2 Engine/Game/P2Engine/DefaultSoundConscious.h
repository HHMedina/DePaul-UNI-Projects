#ifndef DEFAULTSOUNDCONSCIOUS_H
#define DEFAULTSOUNDCONSCIOUS_H

#include "SoundConscious.h"
#include <assert.h>
class DefaultSoundConscious:public SoundConscious{

	friend class SoundSystem;

public:
	
	virtual const Vect& GetSoundConsciousPosition(){
		
		assert(false && "No listener has been set!");
		return sndConPos;
	}

	virtual const Vect& GetSoundConsciousDirection(){
		
		assert(false && "No listener has been set!");
		return sndConDirection;
	}


protected:
	
	static DefaultSoundConscious& GetInstance(){
		if(!instance){
			instance = new DefaultSoundConscious();
		}
		return *instance;
	}
	DefaultSoundConscious(){
		myState = &NotListeningState::GetInstance();//Default state is not Listening
	}
	DefaultSoundConscious(const DefaultSoundConscious&){}
	DefaultSoundConscious& operator = (const DefaultSoundConscious&){}
	virtual ~DefaultSoundConscious(){instance =0;}

	static DefaultSoundConscious* instance;//this is a singleton

	static void DeleteMe(){
		delete GetInstance().instance;
	}
	
	void SetSndConciousPos(Vect& newPos){
		assert(false && "No listener has been set!");
	}

	void SetToNotListening(){
		//do nothing
	}

	void SetToListening(){
		assert(false && "Can not set default listener as listener.");
		myState = &ListeningState::GetInstance();
	}

	void SetSndConciousDir(Vect& newDir){
		assert(false && "No listener has been set!");
	}

	void UpdatePosAndDir(){
		assert(false && "No listener has been set!");
	}
};
#endif