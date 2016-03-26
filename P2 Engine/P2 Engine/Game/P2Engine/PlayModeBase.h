#ifndef PLAYMODEBASE_H
#define PLAYMODEBASE_H

class PlayModeBase{
	friend class Sound;
	friend class Sound2D;
	friend class Sound3D;
protected:
	PlayModeBase(){}
	virtual ~PlayModeBase(){}
	virtual void Play(Sound*){}//will tell the system how to play
	virtual void Play(Sound2D*){}
	virtual void Play(Sound3D*){}
};


#endif