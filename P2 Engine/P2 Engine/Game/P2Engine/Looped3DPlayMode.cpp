#include "Looped3DPlayMode.h"
#include "Sound.h"
#include "SoundSystem.h"
Looped3DPlayMode* Looped3DPlayMode::instance = NULL;
void Looped3DPlayMode::Play(Sound3D* media){
	SoundSystem::Play3DLooped(media);
}