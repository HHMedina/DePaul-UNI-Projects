#ifndef BURSTPLAY2DMODE_H
#define BURSTPLAY2DMODE_H
#include "PlayModeBase.h"

class Sound;
class Burst2DPlayMode:public PlayModeBase{
	friend class SoundSystem;
	friend class Sound2D;

private:
	static Burst2DPlayMode& GetInstance(){
		if(!instance){
			instance = new Burst2DPlayMode();
		}
		return *instance;
	}
	static Burst2DPlayMode* instance;
	Burst2DPlayMode(){}
	virtual ~Burst2DPlayMode(){instance=0;}
	static void DeleteMe(){ delete GetInstance().instance;}
	//note: Not static because not needed... will access playing through polymorphism
	virtual void Play(Sound2D*);//will tell the system how to play
};


#endif