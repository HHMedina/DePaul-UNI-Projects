#include "Burst2DPlayMode.h"
#include "Sound.h"
#include "SoundSystem.h"
Burst2DPlayMode* Burst2DPlayMode::instance = NULL;
void Burst2DPlayMode::Play(Sound2D* media){
	SoundSystem::Play2D(media);
}