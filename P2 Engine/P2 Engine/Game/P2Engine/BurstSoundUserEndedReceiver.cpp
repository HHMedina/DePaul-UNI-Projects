#include "BurstSoundUserEndedReceiver.h"
#include "SoundSystem.h"

/*
	This method is called when a sound is stopped by the user.
	When a user stopped the sound, it will be deregistered from the list
	of the active sounds (in the sound system) already so only the memory needs to be freed.
*/
void BurstSoundUserEndedReceiver::OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData)
{
	reason;
	userData;
	cout<<"SOUND STOPPED BY USER"<<endl;
		
	
	sound->drop();//release sound from irrklang Engine
}