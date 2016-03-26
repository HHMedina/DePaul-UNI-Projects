#include "Looped2DPlayMode.h"
#include "Sound.h"
#include "SoundSystem.h"
Looped2DPlayMode* Looped2DPlayMode::instance = NULL;
void Looped2DPlayMode::Play(Sound2D* media){
	SoundSystem::Play2DLooped(media);
}