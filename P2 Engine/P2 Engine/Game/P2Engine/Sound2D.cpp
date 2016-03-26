#include "Sound2D.h"
#include "SoundSystem.h"
#include "Burst2DPlayMode.h"
#include "Looped2DPlayMode.h"
#include "SoundManager.h"

Sound2D::Sound2D(ISoundSource* sound){
	myPlayingMode = &Burst2DPlayMode::GetInstance(); // non looped by default
	mySoundSource = sound;
	myID = mySoundSource->getName();
	volume=1;//volume is one by default
}

void Sound2D::Play(){
	myPlayingMode->Play(this);

}
void Sound2D::SetLooped(bool state){

	isLooped = state;

	if(state){
		myPlayingMode = &Looped2DPlayMode::GetInstance();//set sound to have the looping mode
		
	}else{
		//set sound to have the burst mode
		myPlayingMode = &Burst2DPlayMode::GetInstance();
	}
	SoundSystem::SetActiveSoundsLoop(this,state);
}

void Sound2D::ResetToDefaultState(ISoundSource* sound){

	myPlayingMode = &Burst2DPlayMode::GetInstance(); // non looped by default
	mySoundSource = sound;
	myID = mySoundSource->getName();
	
	//todo: add volume default reset here
	volume =1;
}

void Sound2D::Release(){
	AssetManager::GetSoundManager()->DestroySound(this);
}