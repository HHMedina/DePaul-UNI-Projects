#include "Sound.h"
//#include "AssetManager.h"
#include "SoundSystem.h"
#include "P2Math.h"
#include <string>
#include "Looped2DPlayMode.h"
#include "Burst2DPlayMode.h"


Sound::~Sound(){
	//sound Does not own sound source
}

void Sound::Stop(){
	SoundSystem::StopSound(this);
}

bool Sound::IsPlaying(){
	return SoundSystem::CheckSoundPlaying(this);
}

void Sound::Pause(){
	SoundSystem::PauseSound(this);
}


void Sound::SetVolume(float vol ){
	//set this sounds volume
	volume = P2Math::Clamp(vol,0.0f,1.0f);

	//If there is an active instance of this sound, then also change its volume
	SoundSystem::SetActiveSoundsVolume(this,volume);

	//mySoundSource->setDefaultVolume(P2Math::Clamp(vol,0.0f,1.0f));
}
