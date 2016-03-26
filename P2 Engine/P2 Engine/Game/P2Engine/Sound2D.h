#ifndef SOUND2D_H
#define SOUND2D_H
#include "Sound.h"
using namespace std;
using namespace irrklang;
class Sound2D: public Sound{

	friend class SoundSystem;
	friend class SoundManager;
public:
	
	/** 
	\brief Plays the Sound2D;
	This will plays the sound and will audibly play the designated sound.
	*/
	virtual void Play();

	/** 
	\brief Set the Sound2D to play in a looping fashion, from start to finish.
	*/
	virtual void SetLooped(bool);

	/** 
	\brief This function is used to release the memory of the sound. This is similar to MarkForDestroy().
	*/
	virtual void Release();
protected:
	/** 
	\brief This function is used to initialize the Sound3D for recycling
	*/
	virtual void ResetToDefaultState(ISoundSource* sound);
	
	
	Sound2D(){}
	Sound2D(ISoundSource* sound);
	Sound2D(const Sound2D&){}
	Sound2D& operator = (const Sound2D&){}
	virtual ~Sound2D(){}
};

#endif