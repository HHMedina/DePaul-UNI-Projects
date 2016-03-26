#ifndef SOUND3D_H
#define SOUND3D_H
#include "Sound.h"
using namespace std;
using namespace irrklang;
class Sound3D: public Sound{

	friend class SoundSystem;
	friend class SoundManager;
public:

	/** 
	\brief this function will play the Sound3D.
	*/
	virtual void Play();

	/** 
	\brief This method sets the sound to play in a looped fashion.
	*/
	virtual void SetLooped(bool);

	/** \ingroup SOUND3D
	\brief This method sets the position of the Sound3D in 3D space.
	*/
	virtual void SetSoundPosition(Vect& newPos);

	/** \ingroup SOUND3D
	\brief This method returns the position of the Sound3D in 3D space.
	*/
	virtual Vect& GetSoundPosition();
	
	virtual void SetMinSoundDistance(float minDist);

	/** 
	\brief This method is used to release the memory of the Sound3D.
	\note This should be called or memory leaks will occur!
	*/
	virtual void Release();

	virtual void Stop();

	virtual void Pause();

	virtual bool IsPlaying();
protected:

	
	Sound3D(){}
	Sound3D(ISoundSource* sound);
	Sound3D(const Sound3D&){}
	Sound3D& operator = (const Sound3D&){}
	virtual ~Sound3D(){}

	virtual void ResetToDefaultState(ISoundSource* sound);
	Vect audiblePos;

	float maxSoundDist;
	float minSoundDist;
	
};

#endif