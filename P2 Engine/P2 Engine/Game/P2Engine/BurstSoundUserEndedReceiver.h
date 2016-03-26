#ifndef BURSTSOUNDUSERENDRECEIVER_H
#define BURSTSOUNDUSERENDRECEIVER_H
#include "irrKlang.h"


class SoundSystem;
//This class is in charge of responding to when a sound has stopped.
//In this case, it is used to remove the sound from the multimap of active sounds
class BurstSoundUserEndedReceiver : public irrklang::ISoundStopEventReceiver
 {
	 friend class SoundSystem;
protected:

	 SoundSystem* myOwner;

	 BurstSoundUserEndedReceiver (SoundSystem* owner){
		 myOwner = owner;
	 }

	   //NOTE: the Sound* is passed via the userData.. safe to cast to Sound*
     virtual void OnSoundStopped (irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData);

	
 };

#endif