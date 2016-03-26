#include "InputManager.h"
#include <iostream>

InputManager::~InputManager(){

	//clean up active key states
	map< AZUL_KEY,KeyState*>::iterator it = registeredKeyStates.begin();
	while(it!=registeredKeyStates.end()){
		delete (it->second);
		it = registeredKeyStates.erase(it);
	}
	registeredKeyStates.clear();
}

bool InputManager::IsKeyPressed(AZUL_KEY key){
	return InputMan::GetKeyboard()->GetKeyState(key);
}

void InputManager::Update(){
	
	//updates all keys (they call the callbacks)
	for(iterate=registeredKeyStates.begin();iterate!=registeredKeyStates.end();++iterate){
				
		iterate->second->Update();
		
		


	}
}

void InputManager::RegisterInput(Inputable * in, AZUL_KEY key, KeyEvent keyEvent){

	//if there isnt a key State for that particular key yet
	if( registeredKeyStates[key] == NULL){
		registeredKeyStates[key] = new KeyState(key);
	}
	
	//attempt to register
	registeredKeyStates[key]->Register(in,keyEvent);
		
	
}

void InputManager::DeregisterInput(Inputable * target, AZUL_KEY key, KeyEvent keyEvent){
	iterate = registeredKeyStates.find(key);
	if(iterate!=registeredKeyStates.end()){
		//check Keystate to see if gameobject is not there. If so register it
		if(!registeredKeyStates[key]->Deregister(target,keyEvent)){//not found
			//cout<<"InputManager: GameObject " <<target<<" not Found in Register with \""<<char(key)<<" and KeyEvent \""<<KeyState::GetCStringOfEnum(keyEvent)<<"\"."<<endl;
		}else{//is found and check if no Inputables are Registered to key and remove key
			if(registeredKeyStates[key]->IsKeyStateEmpty()){//check if empty
				delete iterate->second;
				registeredKeyStates.erase(iterate);
			
			}
		}
	}else{
		//cout<<"InputManager: GameObject " <<target<<" not Found in Register with \""<<char(key)<<" and KeyEvent \""<<KeyState::GetCStringOfEnum(keyEvent)<<"\"."<<endl;
	}


}

