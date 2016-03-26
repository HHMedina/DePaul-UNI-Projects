#include "SoundManager.h"
#include "SoundSystem.h"
#include <assert.h>
#include <iostream>
#include "Sound.h"
#include "Sound2D.h"
#include "Sound3D.h"

string SoundManager::pathToAsset = "../Assets/" ;

SoundManager::SoundManager(){
}

SoundManager::~SoundManager(){
	
	//according to http://www.ambiera.com/irrklang/docu/ dropping sound sources is not needed.

	soundSourceDataBase.clear();

	//destroy everything in the recycle stacks
	while(!sound2DRecycleStack.empty()){
		delete sound2DRecycleStack.top();
		sound2DRecycleStack.pop();
	}

	while(!sound3DRecycleStack.empty()){
		delete sound3DRecycleStack.top();
		sound3DRecycleStack.pop();
	}

	//destroy all sounds outside of Factory
	while(!activeSound2DList.empty()){
		delete activeSound2DList.front();
		activeSound2DList.pop_front();
	}

	while(!activeSound3DList.empty()){
		delete activeSound3DList.front();
		activeSound3DList.pop_front();
	}

}
void SoundManager::LoadSound(const char* const fileName, const char* const assetName){
	
	ISoundSource * tempSound = soundSourceDataBase[assetName];

	
	assert(tempSound==NULL && "SoundManager: Sound already Loaded!");
	string temp = pathToAsset + string(fileName);
	ISoundSource* testCreation = SoundSystem::GetSoundEngine()->addSoundSourceFromFile(temp.c_str());
	assert(testCreation!=NULL&& "SoundFile Could not be loaded!");
	soundSourceDataBase[assetName] = testCreation;
	
}


Sound2D* SoundManager::Create2DSound(const char* const assetName){
	
	ISoundSource * tempSound = soundSourceDataBase[assetName];
	assert(tempSound !=NULL && "SoundManager: Sound Asset Not Found!");

	Sound2D* temp;

	if(!sound2DRecycleStack.empty()){
		temp = sound2DRecycleStack.top();
		sound2DRecycleStack.pop();
	}else{
 		temp = new Sound2D(tempSound);
	}
	activeSound2DList.push_front(temp);
	temp->ResetToDefaultState(tempSound);
	return temp;
}

Sound3D* SoundManager::Create3DSound(const char* const assetName){
	
	ISoundSource * tempSound = soundSourceDataBase[assetName];
	assert(tempSound !=NULL && "SoundManager: Sound Asset Not Found!");
	Sound3D* temp;

	if(!sound3DRecycleStack.empty()){
		temp = sound3DRecycleStack.top();
		sound3DRecycleStack.pop();
	}else{
		temp = new Sound3D(tempSound);
	}
	activeSound3DList.push_front(temp);
	temp->ResetToDefaultState(tempSound);
	return temp;
}

void SoundManager::DestroySound(Sound2D* target){
	target->Stop();//stops incase an associated ISound is active
	activeSound2DList.remove(target);
	sound2DRecycleStack.push(target);
}

void SoundManager::DestroySound(Sound3D* target){
	target->Stop();//stops incase an associated ISound is active
	activeSound3DList.remove(target);
	sound3DRecycleStack.push(target);
}