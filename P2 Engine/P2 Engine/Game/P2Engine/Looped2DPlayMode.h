#ifndef LOOPED2DPLAYMODE_H
#define LOOPED2DPLAYMODE_H
#include "PlayModeBase.h"
class Sound;
class Looped2DPlayMode:public PlayModeBase{
	friend class SoundSystem;
	friend class Sound2D;
	
private:
	static Looped2DPlayMode& GetInstance(){
		if(!instance){
			instance = new Looped2DPlayMode();
		}
		return *instance;
	}
	static Looped2DPlayMode* instance;

	Looped2DPlayMode(){}
	virtual ~Looped2DPlayMode(){instance=0;}
	static void DeleteMe(){ delete GetInstance().instance;}
	virtual void Play(Sound2D*);//will tell the system how to play
};


#endif