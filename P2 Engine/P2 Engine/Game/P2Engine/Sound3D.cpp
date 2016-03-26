#include "Sound3D.h"
#include "SoundSystem.h"
#include "Burst3DPlayMode.h"
#include "Looped3DPlayMode.h"
#include "SoundManager.h"
#include "DebugVisualizer.h"

Sound3D::Sound3D(ISoundSource* sound){
	myPlayingMode = &Burst3DPlayMode::GetInstance(); // non looped by default
	mySoundSource = sound;
	myID = mySoundSource->getName();
	volume =1;//one by default
}

void Sound3D::Play(){
	myPlayingMode->Play(this);

}
void Sound3D::SetLooped(bool state){

	isLooped = state;

	if(state){
		myPlayingMode = &Looped3DPlayMode::GetInstance();//set sound to have the looping mode
		
	}else{
		//set sound to have the burst mode
		myPlayingMode = &Burst3DPlayMode::GetInstance();
	}
	SoundSystem::SetActiveSoundsLoop(this,state);
}

void Sound3D::SetSoundPosition(Vect& newPos){
	
	//access active sounds and set position;
	SoundSystem::SetSoundPosition(this,newPos);
}

Vect& Sound3D::GetSoundPosition(){
	return audiblePos;
}

void Sound3D::ResetToDefaultState(ISoundSource* sound){
	myPlayingMode = &Burst3DPlayMode::GetInstance(); // non looped by default
	mySoundSource = sound;
	myID = mySoundSource->getName();
	audiblePos = Vect(0,0,0);
	volume =1;
	minSoundDist =1;//default according to irrklang
	maxSoundDist = 1000000000.0f; //default according to irrklang
}

void Sound3D::Release(){
	AssetManager::GetSoundManager()->DestroySound(this);
}

void Sound3D::SetMinSoundDistance(float minDist){
	minSoundDist = minDist;

	//set any active audible sounds from this Sound min dist
	SoundSystem::SetActiveSoundsMinDistance(this,minDist);
}

void Sound3D::Stop(){
	SoundSystem::StopSound(this);
}

void Sound3D::Pause(){
	SoundSystem::PauseSound(this);
}

bool Sound3D::IsPlaying(){
	return SoundSystem::CheckSoundPlaying(this);
}