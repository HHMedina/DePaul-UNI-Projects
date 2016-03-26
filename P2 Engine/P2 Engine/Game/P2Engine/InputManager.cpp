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
	
	if(keyEvent == KeyUpAndDown){//if keyevent is both then register for both
		//attempt to register
		registeredKeyStates[key]->Register(in,KeyDown);
		registeredKeyStates[key]->Register(in,KeyUp);

		//adjust Inputable's Registered Keys list (Already checks for doubles)
		in->AddRegisteredKeyPair(Inputable::KeyPair(key,KeyDown));
		in->AddRegisteredKeyPair(Inputable::KeyPair(key,KeyUp));

	}else{//else do other they specified 
		registeredKeyStates[key]->Register(in,keyEvent);
		in->AddRegisteredKeyPair(Inputable::KeyPair(key,keyEvent));
	}
	
}

void InputManager::DeregisterInput(Inputable * target, AZUL_KEY key, KeyEvent keyEvent){
	iterate = registeredKeyStates.find(key);
	if(iterate!=registeredKeyStates.end()){
		
		//attempt deregister
		if(keyEvent==KeyUpAndDown){
			//check Keystate to see if gameobject is not there. If so register it
			registeredKeyStates[key]->Deregister(target,KeyDown);
			registeredKeyStates[key]->Deregister(target,KeyUp);

			//adjust Inputable's Registered Keys list (Already checks for doubles)
			target->RemoveRegisteredKeyPair(Inputable::KeyPair(key,KeyDown));
			target->RemoveRegisteredKeyPair(Inputable::KeyPair(key,KeyUp));

		}else{
			registeredKeyStates[key]->Deregister(target,keyEvent);
			target->RemoveRegisteredKeyPair(Inputable::KeyPair(key,keyEvent));
		}

		//check if keystate is empty
		if(registeredKeyStates[key]->IsKeyStateEmpty()){//check if empty
			delete iterate->second;
			registeredKeyStates.erase(iterate);
			
		}
		
	}else{
		//cout<<"InputManager: GameObject " <<target<<" not Found in Register with \""<<char(key)<<" and KeyEvent \""<<KeyState::GetCStringOfEnum(keyEvent)<<"\"."<<endl;
	}


}

void InputManager::DeregisterAllInputKeys(Inputable * in){

	while(!in->myRegisteredKeys.empty()){
		DeregisterInput(in,in->myRegisteredKeys.front().myKey,in->myRegisteredKeys.front().myKeyEvent);
		//in->myRegisteredKeys.pop_front();//deregister also removes from list
	}
}

bool Inputable::KeyPair::operator ==(const KeyPair& rhs)const{
	return myKey==rhs.myKey&&myKeyEvent==rhs.myKeyEvent;
}