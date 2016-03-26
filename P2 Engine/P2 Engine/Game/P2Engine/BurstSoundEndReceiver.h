#ifndef BURSTSOUNDENDRECEIVER_H
#define BURSTSOUNDENDRECEIVER_H
#include "irrKlang.h"


class SoundSystem;
//This class is in charge of responding to when a sound has stopped.
//In this case, it is used to remove the sound from the multimap of active sounds
class BurstSoundEndReceiver : public irrklang::ISoundStopEventReceiver
 {
	 friend class SoundSystem;
 protected:

	 SoundSystem* myOwner;

	 BurstSoundEndReceiver(SoundSystem* owner){
		 myOwner = owner;
	 }

	   //NOTE: the Sound* is passed via the userData.. safe to cast to Sound*
     virtual void OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData);

	
 };

#endif