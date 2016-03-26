#ifndef BURST3DPLAYMODE_H
#define BURST3DPLAYMODE_H
#include "PlayModeBase.h"

class Sound;
class Burst3DPlayMode:public PlayModeBase{
	friend class SoundSystem;
	friend class Sound3D;
private:
	
	static Burst3DPlayMode& GetInstance(){
		if(!instance){
			instance = new Burst3DPlayMode();
		}
		return *instance;
	}
	static Burst3DPlayMode* instance;
	Burst3DPlayMode(){}
	virtual ~Burst3DPlayMode(){instance=0;}
	static void DeleteMe(){ delete GetInstance().instance;}
	//note: Not static because not needed... will access playing through polymorphism
	virtual void Play(Sound3D*);//will tell the system how to play
};


#endif