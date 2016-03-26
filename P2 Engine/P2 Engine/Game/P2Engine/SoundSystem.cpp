#include "SoundSystem.h"
#include "AssetManager.h"
#include "Sound.h"
#include "Sound3D.h"
#include "Sound2D.h"
#include <map>
#include "P2Math.h"
#include "BurstSoundEndReceiver.h"
#include "BurstSoundUserEndedReceiver.h"
#include "Burst2DPlayMode.h"
#include "Burst3DPlayMode.h"
#include "Looped3DPlayMode.h"
#include "Looped2DPlayMode.h"
#include "SoundConscious.h"
#include "No3DSound.h"
#include "DefaultSoundConscious.h"
SoundSystem* SoundSystem::instance = NULL;
void SoundSystem::StartSoundEngine(){
	
	if(Instance().soundEngine == NULL){//if there isnt already an engine
		Instance().soundEngine = createIrrKlangDevice();
		//Instance().testSoundSource = Instance().soundEngine->play2D("../Assets/TankShot.flac",false,true);

	}
}

SoundSystem::SoundSystem(){
	systemListener = &DefaultSoundConscious::GetInstance();//make default camera the default listener
	sndEndReceiver = new BurstSoundEndReceiver(this);
	sndEndedUsrReceiver = new BurstSoundUserEndedReceiver(this);
	soundEngine = NULL;
}

void SoundSystem::DeleteMe(){
	if(Instance().soundEngine){
		Instance().StopAllSounds();
		Instance().soundEngine->drop();
	}

	DefaultSoundConscious::DeleteMe();

	ListeningState::DeleteMe();
	NotListeningState::DeleteMe();

	No3DSound::DeleteMe();

	Burst2DPlayMode::DeleteMe();
	Looped2DPlayMode::DeleteMe();
	Burst3DPlayMode::DeleteMe();
	Looped3DPlayMode::DeleteMe();
	delete instance;
}

void SoundSystem::Play2D(Sound2D* media){

	
	if(Instance().CheckSoundPlaying(media)){//if playing
		
		ISound* temp = Instance().currPlayingLoopedSounds[media];
		if(!temp->getIsPaused()){
			temp->setPlayPosition(0.0f);//set back to beginning
		}
		temp->setIsLooped(false);
		temp->setSoundStopEventReceiver (Instance().sndEndReceiver,media);
		temp->setVolume(media->volume);
		temp->setIsPaused(false);
	}else{
		//creates a new sound instance
		ISound* temp = Instance().soundEngine->play2D(media->mySoundSource,false,false,true);
		Instance().currPlayingLoopedSounds.insert(std::pair<Sound*,ISound*>(media,temp));
		temp->setSoundStopEventReceiver(Instance().sndEndReceiver,media);
		temp->setVolume(media->volume);
		temp->setIsPaused(false);
	}
	
}

void SoundSystem::Play3D(Sound3D* media){
	if(Instance().CheckSoundPlaying(media)){//if playing
		ISound* temp = Instance().currPlayingLoopedSounds[media];
		if(!temp->getIsPaused()){
			temp->setPlayPosition(0.0f);//set back to beginning
		}
		temp->setIsLooped(false);
		temp->setSoundStopEventReceiver (Instance().sndEndReceiver,media);
		temp->setVolume(media->volume);
		temp->setMinDistance(media->minSoundDist);
		temp->setMaxDistance(FLT_MAX); // max attenuation distance
		temp->setIsPaused(false);
	}else{
		//creates a new sound instance
         ISound* temp = Instance().soundEngine->play3D(media->mySoundSource,vec3df(media->GetSoundPosition()[x],media->GetSoundPosition()[y],media->GetSoundPosition()[z]),false,false,true);
		
		//ISound* temp = Instance().soundEngine->play3D(media->mySoundSource,media->GetPosition(),false,true);
		Instance().currPlayingLoopedSounds.insert(std::pair<Sound*,ISound*>(media,temp));
		temp->setSoundStopEventReceiver(Instance().sndEndReceiver,media);
		temp->setVolume(media->volume);
		temp->setMinDistance(media->minSoundDist);
		temp->setMaxDistance(FLT_MAX); // max attenuation distance
		temp->setIsPaused(false);
		
	}
}

void SoundSystem::Play2DLooped(Sound2D* media){

	if(Instance().CheckSoundPlaying(media)){//if currently playing
		ISound* temp =Instance().currPlayingLoopedSounds[media]; 
		if(!temp->getIsPaused()){//if not paused... start from beginning
			temp->setPlayPosition(0.0f);//set back to beginning
		}
		temp->setIsLooped(true);
		temp->setSoundStopEventReceiver (Instance().sndEndedUsrReceiver,media);
		temp->setVolume(media->volume);
		temp->setIsPaused(false);
	}else{
		//creates a new sound instance
		ISound* temp = Instance().soundEngine->play2D(media->mySoundSource,true,false,true);
		Instance().currPlayingLoopedSounds.insert(std::pair<Sound*,ISound*>(media,temp));
		temp->setSoundStopEventReceiver(Instance().sndEndedUsrReceiver,media);
		temp->setVolume(media->volume);
		temp->setIsPaused(false);
	}

}


void SoundSystem::Play3DLooped(Sound3D* media){
	if(Instance().CheckSoundPlaying(media)){//if playing
		ISound* temp = Instance().currPlayingLoopedSounds[media];
		if(!temp->getIsPaused()){
			temp->setPlayPosition(0.0f);//set back to beginning
		}
		temp->setIsLooped(true);
		temp->setSoundStopEventReceiver (Instance().sndEndedUsrReceiver,media);
		temp->setVolume(media->volume);
		temp->setMinDistance(media->minSoundDist);
		
		temp->setMaxDistance(FLT_MAX); // max attenuation distance
		temp->setIsPaused(false);
	}else{
		//creates a new sound instance
		ISound* temp = Instance().soundEngine->play3D(media->mySoundSource,vec3df(media->GetSoundPosition()[x],media->GetSoundPosition()[y],media->GetSoundPosition()[z]),true,false,true);
		//ISound* temp = Instance().soundEngine->play3D(media->mySoundSource,media->GetPosition(),false,true);
		Instance().currPlayingLoopedSounds.insert(std::pair<Sound*,ISound*>(media,temp));
		temp->setSoundStopEventReceiver(Instance().sndEndedUsrReceiver,media);
		temp->setVolume(media->volume);
		temp->setMinDistance(media->minSoundDist);
		temp->setMaxDistance(FLT_MAX); // max attenuation distance
		temp->setIsPaused(false);
	}

}

bool SoundSystem::CheckSoundPlaying(Sound* media){
	return Instance().currPlayingLoopedSounds.find(media)!= Instance().currPlayingLoopedSounds.end();
}



void SoundSystem::PauseSound(Sound* media){
	if( Instance().currPlayingLoopedSounds.find(media)!=Instance().currPlayingLoopedSounds.end()){//if sound exists
		Instance().currPlayingLoopedSounds[media]->setIsPaused(true);
	}
}

void SoundSystem::SetMainVolume(float mVol){
	
	Instance().soundEngine->setSoundVolume(P2Math::Clamp(mVol,0.0f,1.0f));
}

void SoundSystem::SetActiveSoundsLoop(Sound* target,bool state){
	map<Sound*,ISound*>::iterator it;
	it = Instance().currPlayingLoopedSounds.find(target);
	if(it!=Instance().currPlayingLoopedSounds.end()){
		it->second->setIsLooped(state);
		if(state==true){
			it->second->setSoundStopEventReceiver(Instance().sndEndedUsrReceiver,target);
		}else{
			it->second->setSoundStopEventReceiver(Instance().sndEndReceiver,target);
		}
	}
}

void SoundSystem::SetActiveSoundsMinDistance(Sound* target,float minDist){
	map<Sound*,ISound*>::iterator it;
	it = Instance().currPlayingLoopedSounds.find(target);
	if(it!=Instance().currPlayingLoopedSounds.end()){
		it->second->setMinDistance(minDist);
	}
}


void SoundSystem::SetActiveSoundsVolume(Sound* media, float vol){
	map<Sound*,ISound*>::iterator it;
	it = Instance().currPlayingLoopedSounds.find(media);
	if(it!=Instance().currPlayingLoopedSounds.end()){
		it->second->setVolume(vol);
	}
}

SoundSystem::~SoundSystem(){
	currPlayingLoopedSounds.clear();
	delete sndEndReceiver;
	sndEndReceiver=NULL;

	delete sndEndedUsrReceiver;
	sndEndedUsrReceiver =NULL;
	
	soundEngine =NULL;
	instance =NULL;
}

void SoundSystem::StopAllSounds(){
	//remove all Loop Sounds
	cout<<Instance().currPlayingLoopedSounds.size();
	std::multimap<Sound*,ISound*>::iterator toErase; 
	std::multimap<Sound*,ISound*>::iterator it = Instance().currPlayingLoopedSounds.begin();
	while( it !=Instance().currPlayingLoopedSounds.end()){
		it->second->setSoundStopEventReceiver(Instance().sndEndedUsrReceiver);
		it->second->stop();
		toErase = it;
		it++;
		Instance().currPlayingLoopedSounds.erase(toErase);
	}
	

}
void SoundSystem::RemoveFromActive(Sound* target){
	map<Sound*,ISound*>::iterator it;
	it = Instance().currPlayingLoopedSounds.find(target);
	if(it!=Instance().currPlayingLoopedSounds.end()){
		Instance().currPlayingLoopedSounds.erase(it); //remove

	}
}

void SoundSystem::StopSound(Sound* soundToStop){
	map<Sound*,ISound*>::iterator it;
	it = Instance().currPlayingLoopedSounds.find(soundToStop);
	if(it!=Instance().currPlayingLoopedSounds.end()){
		Instance().currPlayingLoopedSounds[soundToStop]->setSoundStopEventReceiver(Instance().sndEndedUsrReceiver,soundToStop);//sets the stop event reciever to be a user ended one.
		
		Instance().currPlayingLoopedSounds[soundToStop]->stop();//stop sound
		//it->second->drop();//will be dropped at end reciever
		
		Instance().currPlayingLoopedSounds.erase(it);

	}
}

ISoundEngine* SoundSystem::GetSoundEngine(){
	return Instance().soundEngine;
}

void SoundSystem::SetSoundPosition(Sound* media, Vect& newPos ){
	map<Sound*,ISound*>::iterator it;
	it = Instance().currPlayingLoopedSounds.find(media);
	if(it!=Instance().currPlayingLoopedSounds.end()){//if found
		DebugVisualizer::ShowPoint(Vect(newPos[x], newPos[y],-newPos[z]));
		Instance().currPlayingLoopedSounds[media]->setPosition( vec3df(newPos[x], newPos[y],-newPos[z]) ); //Azul (right hand) to irrklang (left hand)
	
	}
}

void SoundSystem::SetListener(SoundConscious* newListener){

	//Set Previous Listener To Not Listener
	Instance().systemListener->SetToNotListening();
	
	Instance().systemListener = newListener;

	newListener->UpdatePosAndDir();//update new listener
	
	//SetNewListener to listening state
	newListener->SetToListening();
}

void SoundSystem::AddToDropList(Sound* target){
	Instance().soundsToBeDropped.push_front(target);

}

void SoundSystem::ProcessSoundDrops(){
	
	map<Sound*,ISound*>::iterator it;
	while(!Instance().soundsToBeDropped.empty()){
  		it = Instance().currPlayingLoopedSounds.find(Instance().soundsToBeDropped.front());
		
		//drop and remove
		it->second->drop();
		Instance().currPlayingLoopedSounds.erase(it);

		Instance().soundsToBeDropped.pop_front();

	}

}