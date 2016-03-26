#include "Updatable.h"
#include "SceneManager.h"

void Updatable::RegisterForUpdate(){
	SceneManager::RegisterForUpdating(this);
}

void Updatable::DeregisterForUpdate(){
	SceneManager::DeregisterForUpdating(this);
}