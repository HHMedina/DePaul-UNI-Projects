#ifndef LOOPED3DPLAYMODE_H
#define LOOPED3DPLAYMODE_H
#include "PlayModeBase.h"

class Sound;
class Looped3DPlayMode:public PlayModeBase{
	friend class SoundSystem;
	friend class Sound3D;
private:
	
	static Looped3DPlayMode& GetInstance(){
		if(!instance){
			instance = new Looped3DPlayMode();
		}
		return *instance;
	}
	static Looped3DPlayMode* instance;
	Looped3DPlayMode(){}
	virtual ~Looped3DPlayMode(){instance=0;}
	static void DeleteMe(){ delete GetInstance().instance;}
	//note: Not static because not needed... will access playing through polymorphism
	virtual void Play(Sound3D*);//will tell the system how to play
};


#endif