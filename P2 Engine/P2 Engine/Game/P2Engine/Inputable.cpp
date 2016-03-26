#include "Inputable.h"
#include "SceneManager.h"

void Inputable::RegisterForInput(AZUL_KEY key, KeyEvent kEventType){
	SceneManager::RegisterForInput(this,key,kEventType);
}

void Inputable::DeregisterForInput(AZUL_KEY key, KeyEvent kEventType){
	SceneManager::DeregisterForInput(this,key,kEventType);
}

void Inputable::AddRegisteredKeyPair(Inputable::KeyPair pair){

	//check if already in list.
	if( std::find( myRegisteredKeys.begin(),myRegisteredKeys.end(), pair) == myRegisteredKeys.end() ){

		//register this key as having been registered
		myRegisteredKeys.push_front(pair);

	}



}

void Inputable::RemoveRegisteredKeyPair(Inputable::KeyPair pair){
	//check if not in list.
	if( std::find( myRegisteredKeys.begin(),myRegisteredKeys.end(), pair) != myRegisteredKeys.end() ){

		//deregister this key
		myRegisteredKeys.remove(pair);

	}

}

void Inputable::DeregisterAllInputKeys(){
	SceneManager::DeregisterAllInputKeys(this);
}