#ifndef NO3DSOUND_H
#define NO3DSOUND_H
#include "Sound3D.h"
#include <assert.h>
using namespace std;
using namespace irrklang;
class No3DSound: public Sound3D{

	friend class SoundSystem;
	friend class SoundManager;
	friend class Audible;
public:
	//assertion error due to user calling functions on audible with no sound set.
	virtual void Play(){assert(false&&"No3DSound: Cannot play Audible's sound. No Sound Attached.");}
	virtual void SetLooped(bool){assert(false&&"No3DSound: Cannot set audible Sound's loop status. No Sound Attached.");}
	virtual Vect& GetSoundPosition(){assert(false&&"No3DSound: Cannot get audible Sound's position. No Sound Attached.");return zeroVect;}
	virtual void Release(){/* used by engine...no assertion */}
	virtual void SetMinSoundDistance(float minDist){assert(false&&"No3DSound: Cannot get audible Sound's minimum distance. No Sound Attached.");}
	virtual void Stop()override{assert(false&&"No3DSound: Cannot stop audible's Sound. No Sound Attached.");}
	virtual void Pause(){assert(false&&"No3DSound: Cannot pause audible's Sound. No Sound Attached.");}
	virtual bool IsPlaying(){assert(false&&"No3DSound: Cannot check audible's Sound playing status . No Sound Attached.");return false;}
private:
	
	
	static No3DSound& GetInstance(){
		if(!instance){
			instance = new No3DSound();
		}
		return *instance;
	}
	static No3DSound* instance;

	static void DeleteMe(){
		delete GetInstance().instance;
	}

	No3DSound(){}
	No3DSound(const No3DSound&){}
	No3DSound& operator = (const No3DSound&){}
	~No3DSound(){instance=NULL;}

	
	virtual void SetSoundPosition(Vect&){/* no asset since set position called by engine */}

	Vect zeroVect;
};
#endif