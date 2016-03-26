#include "Burst3DPlayMode.h"
#include "Sound.h"
#include "SoundSystem.h"
Burst3DPlayMode* Burst3DPlayMode::instance = NULL;
void Burst3DPlayMode::Play(Sound3D* media){
	SoundSystem::Play3D(media);
}