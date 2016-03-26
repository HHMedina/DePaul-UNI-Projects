#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <map>
#include "irrKlang.h"
#include <string>
#include <stack>
#include <list>

using namespace irrklang;
using namespace std;

class Sound;
class Sound2D;
class Sound3D;

class SoundManager{
	friend class AssetManager;
	friend class Sound;
	friend class Sound2D;
	friend class Sound3D;
	static struct comparer
	{
		public:
		bool operator()(const std::string x, const std::string y)
		{
			return x.compare(y)>0;
		}
	};

public:

	/** \ingroup ASSETMANAGER
	\brief loads a sound asset

	This method allows a user to import a sound file into the game engine.
	\param fileName the name of the file in the Assets folder to import
	\param assetName The name to assign to the imported sound file.

	An example of loading a sound is seen below.

	\code

	void Game::LoadGameContent(){
	
		AssetManager::LoadSound("TankShot.flac","TankShotSound");
		AssetManager::LoadSound("getout.ogg","GetOutBGM");
		
		//other assets have been omitted...

}

	\endcode

	*/
	void LoadSound( const char* const fileName,const char* const assetName);

	/** \ingroup ASSETMANAGER
	\brief Creates and returns a Sound2D

	This method creates and returns a Sound2D with the audible sound specified.

	\param assetName The name of the sound asset to use as audio.
	\note See Sound for an example of how to use Create2DSound
	*/
	Sound2D* Create2DSound(const char* const assetName);

	/** \ingroup ASSETMANAGER
	\brief Creates and returns a Sound3D

	This method creates and returns a Sound3D with the audible sound specified.

	\param assetName The name of the sound asset to use as audio.
	\note See Sound for an example of how to use Create3DSound
	*/
	Sound3D* Create3DSound(const char* const assetName);

	

private:

	SoundManager();
	SoundManager(const SoundManager&);
	SoundManager& operator = (const SoundManager&);
	~SoundManager();

	ISoundSource* GetSoundSource(const char* const assetName){

		return soundSourceDataBase[assetName];
	}

	
	void DestroySound(Sound*){}
	void DestroySound(Sound2D*);
	void DestroySound(Sound3D*);

	map<string, ISoundSource*,comparer> soundSourceDataBase;// sound sources are "copied" so a master copy is needed (Sound class contain a copy of the soundsource)
	
	static string pathToAsset;

	//recycling stacks
	stack<Sound2D*> sound2DRecycleStack;
	stack<Sound3D*> sound3DRecycleStack;

	//active list (used for clean up)
	list<Sound2D*> activeSound2DList;
	list<Sound3D*> activeSound3DList;
};



#endif