#ifndef AUDIBLE_H
#define AUDIBLE_H

#include "irrKlang.h"
#include "Vect.h"
#include "Sound3D.h"
#include "AssetManager.h"

class Audible{

public:

	
	/** \ingroup AUDIBLE
	\brief Sets the audible's sound

	This function will set what the sound will play based on one of the loaded sounds.

	\param assetName The name of the loaded Sound asset this audible sound sounds like.
	*/
	void SetAudibleSound(const char* const assetName){
		
		my3DSound->Release();//release previous sound memory
		my3DSound = AssetManager::GetSound3DInstance(assetName);//new sound

	}

	/** \ingroup AUDIBLE
	\brief Sets the audible's looping status.

	Sets whether the sound is looping or not.

	\param state true = looped, flase = not looped.
	*/
	void SetAudibleLooped(bool state){
		my3DSound->SetLooped(state);
	}

	/** \ingroup AUDIBLE
	\brief Plays Audible Sound.

	This functions plays the attached AudibleSound.
	\note If no sound is present, nothing will play. Ensure an audibleSound is present if playing an audible sound!
	*/
	void PlayAudibleSound(){
		my3DSound->Play();
	}

	/** \ingroup AUDIBLE
	\brief Stops an audible's playing sound.

	This functions Stopsthe attached AudibleSound.
	\note If sound is not active, nothing will happen.
	*/
	void StopAudibleSound(){
		my3DSound->Stop();
	}

	/** \ingroup AUDIBLE
	\brief pauses an audible's playing sound.

	This functions pauses the attached AudibleSound.
	\note If sound is not active, nothing will happen. Also, a sound is unpaused by calling play on it.
	*/
	void PauseAudibleSound(){
		my3DSound->Pause();
	}

	/** \ingroup AUDIBLE
	\brief Set an audible's volume.

	This functions sets the volume of the audible sound.
	\param vol The volume level to set the audible sound to.

	\note volume goes from [0...1]. The functions clamps between this.
	*/
	void SetAudibleVolume(float vol){
		my3DSound->SetVolume(vol);
	}

	/** \ingroup AUDIBLE
	\brief Checks the playing status of the audible's sound.

	This functions returns true if the audible sound is playing and false if it is not.

	\note volume goes from [0...1]. The functions clamps between this.
	*/
	bool ISAudibleSoundPlaying(){
		return my3DSound->IsPlaying();
	}

	/** \ingroup AUDIBLE
	\brief sets distance away from Audible before its sound starts attenuating.

	This method sets the audible sounds distance away from its position in which the volume will start attenuating.

	\param minDist The distance away from the object before volume begins attenuating.
	*/
	void SetAudibleSoundMinDistance(float minDist){
		my3DSound->SetMinSoundDistance(minDist);
	}


protected:

	
	Audible();
	Audible(const Audible&){}
	virtual ~Audible(){
		my3DSound->Release();
	}

	void SetAudiblePosition(Vect& newPos){
		my3DSound->SetSoundPosition(newPos);
	}

	Sound3D* my3DSound;
};
#endif AUDIBLE_H