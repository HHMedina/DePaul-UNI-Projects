#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include "AssetManager.h"
using namespace irrklang;
#include <string>
#include <iostream>
#include <list>

#pragma comment(lib, "irrklang.lib")
class Sound;
class SoundConscious;
class SoundSystem{

	friend class Game;

	friend class SoundManager;

	friend class Burst3DPlayMode;
	friend class Burst2DPlayMode;
	friend class Looped3DPlayMode;
	friend class Looped2DPlayMode;
	friend class BurstSoundEndReceiver;
	friend class BurstSoundUserEndedReceiver;

	friend class SoundConscious;
	friend class Sound;
	friend class Sound2D;
	friend class Sound3D;
	friend class ListeningState;
	static struct comparer
	{
		public:
		bool operator()(const std::string x, const std::string y)
		{
			return x.compare(y)>0;
		}
	};
	

public:

	/** \ingroup SOUNDSYS
	\brief Stops all currently Playing Sounds

	The following line of code should be called when trying to stop all sounds
	/code
	SoundSystem::StopAllSounds();
	/endcode
	*/
	static void StopAllSounds();

	

private:

	static void SetListener(SoundConscious*);
	static void SetActiveSoundsMinDistance(Sound* target,float minDist);
	static void SetActiveSoundsMaxDistance(Sound* target,float maxDist);
	static void StopSound(Sound*);
	static void Play2D(Sound2D* media);
	static void Play2DLooped(Sound2D* media);
	static void Play3D(Sound3D* media);
	static void Play3DLooped(Sound3D* media);
	static void PauseSound(Sound* media);
	static bool CheckSoundPlaying(Sound* media);
	static void SetMainVolume(float mVol);
	static ISoundEngine* GetSoundEngine();
	static void SetActiveSoundsLoop(Sound*,bool);
	static void SetActiveSoundsVolume(Sound* media,float vol);
	static void ProcessSoundDrops();
	static void SetSoundPosition(Sound* media, Vect& newPos);
	static void RemoveFromActive(Sound* target);
	static void AddToDropList(Sound* target);
	SoundSystem();
	~SoundSystem();

	ISoundEngine* soundEngine;

	static SoundSystem& Instance(){
		if(!instance){
			instance = new SoundSystem();

		}
		return *instance;
	}

	static SoundSystem* instance;//this is a singleton
	
	static void StartSoundEngine();
	static void DeleteMe();
	
	
	//holds all of the currently playing Sounds
	//Note: each Sound can only have one active audible sound
	//at a time.
	map<Sound*,ISound*> currPlayingLoopedSounds;

	ISound* testSoundSource;

	BurstSoundEndReceiver* sndEndReceiver;
	BurstSoundUserEndedReceiver* sndEndedUsrReceiver;

	//Sound system only has one "listener"
	SoundConscious* systemListener;

	list<Sound*> soundsToBeDropped;
};



#endif