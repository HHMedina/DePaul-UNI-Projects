#include "BurstSoundEndReceiver.h"
#include "SoundSystem.h"

/*
Note: Took about 7 hours of debugging.... but this callback is called during any frame 
...hence it is not synced with the gameloop. In order to fix this, AddToDropList() registers
the Isound for deletion during a designated part of the frame, fixing crashes caused by accessing
an irrklang 3d sound that was already stopped.
*/

void BurstSoundEndReceiver::OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData)
{

	Sound* soundsOwner = (Sound*) userData;
	//cout<<"SOUND STOPPED"<<endl;
	/*	
	SoundSystem::RemoveFromActive(soundsOwner);
	//end removal of burst sound from list
	sound->drop();//release sound from irrklang Engine
	*/
	SoundSystem::AddToDropList(soundsOwner);//WORKS YESSS!!!! FINALLLY
	//new attempt.. add to sounds to be dropped.

}