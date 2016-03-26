#ifndef SOUND_H
#define SOUND_H
#include "ik_ISoundSource.h"
#include "ik_ISound.h"
#include <string>
#include  "Azul.h"
class PlayModeBase;

using namespace std;
using namespace irrklang;
class Sound{

	friend class SoundSystem;

public:

	/** \ingroup BOTHSOUNDS
	\brief This method will begin the playback of the Sound.
	*/
	virtual void Play() =0;
	/** \ingroup BOTHSOUNDS
	\brief This method will set if a sound is looped or not.
	\param state true = looped false = not looped
	*/
	virtual void SetLooped(bool state) =0;

	/** \ingroup BOTHSOUNDS
	\brief This method will free up the Sound when no longer needed.
	*/
	virtual void Release()=0;
	/** \ingroup BOTHSOUNDS
	\brief This method will pause the Sound.
	*/
	void Pause();

	/** \ingroup BOTHSOUNDS
	\brief This method Stop a sound.
	\note To unpause a sound call Play() on it.
	*/
	void Stop();

	/** \ingroup BOTHSOUNDS
	\brief This method sets the volume of the sound.
	\note A Sound's volume can only be set to a value of [0.0f - 1.0f]
	*/
	void SetVolume(float vol);

	/** \ingroup BOTHSOUNDS
	\brief This function returns true if the sound is playing.
	*/
	bool IsPlaying();
	

protected:

	
	Sound(){}
	Sound(ISoundSource* sound);
	Sound(const Sound&){}
	Sound& operator = (const Sound&){}
	virtual ~Sound();

	virtual void ResetToDefaultState(ISoundSource* sound)=0;

	ISoundSource* mySoundSource;

	PlayModeBase* myPlayingMode;
	string myID;

	bool isLooped;

	float volume;
};

#endif