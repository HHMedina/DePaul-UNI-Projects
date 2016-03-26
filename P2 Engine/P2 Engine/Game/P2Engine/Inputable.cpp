#include "Inputable.h"
#include "SceneManager.h"

void Inputable::RegisterForInput(AZUL_KEY key, KeyEvent kEventType){
	SceneManager::RegisterForInput(this,key,kEventType);
}

void Inputable::DeregisterForInput(AZUL_KEY key, KeyEvent kEventType){
	SceneManager::DeregisterForInput(this,key,kEventType);
}