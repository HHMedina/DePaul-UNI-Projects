
#include "KeyState.h"


KeyState::KeyState(AZUL_KEY key){
	myKey = key;
	previousState = false;
	
}

KeyState::~KeyState(){

	keyUpList.clear();
	keyDownList.clear();

}

void KeyState::Update(){
	
	CheckKeyDown();
	CheckKeyUp();

	//store previous state
	previousState = InputMan::GetKeyboard()->GetKeyState(myKey);
	
}

/*
	Registers the object into the corresponding list.
	The function will return true is successful or false if the inputable
	is already registered.
*/
bool KeyState::Register(Inputable* target,KeyEvent kE){

	if(kE==KeyEvent::KeyDown){

		iterate = std::find(keyDownList.begin(),keyDownList.end(),target);//see if object is registered
		if(iterate==keyDownList.end()){ //find() will return end if not found (not already registered)
			keyDownList.push_back(target);
			return true;
		}
		
	}else if (kE==KeyEvent::KeyUp){

		iterate = std::find(keyUpList.begin(),keyUpList.end(),target);//see if object is registered
		if(iterate==keyUpList.end()){ //find() will return end if not found (not already registered)
			keyUpList.push_back(target);
			return true;
		}
	}
	
	return false;//Gameobject Already Registered
}


bool KeyState::Deregister(Inputable* target,KeyEvent kE){
	target;
	if(kE==KeyEvent::KeyDown){
		iterate = std::find(keyDownList.begin(),keyDownList.end(),target);//see if object is registered
		if(iterate!=keyDownList.end()){ //find() will return end if not found (not already registered)
			iterate=keyDownList.erase(iterate);
			return true;
		}
	}else if (kE==KeyEvent::KeyUp) {
		iterate = std::find(keyUpList.begin(),keyUpList.end(),target);//see if object is registered
		if(iterate!=keyUpList.end()){ //find() will return end if not found (not already registered)
			iterate=keyUpList.erase(iterate);
			return true;
		}
	}
	
	return false;
}

void KeyState::CheckKeyDown(){
	
	if(!previousState&&InputMan::GetKeyboard()->GetKeyState(myKey)){

		//call all callback functions in registered inputables for key down event
		for(iterate= keyDownList.begin();iterate!=keyDownList.end();){
			(*iterate)->KeyDown(myKey);
			++iterate;
			
		}
	
	}
}

void KeyState::CheckKeyUp(){
	
	if(previousState&&!InputMan::GetKeyboard()->GetKeyState(myKey)){

		//call all callback functions in registered inputables for key up event
		for(iterate= keyUpList.begin();iterate!=keyUpList.end();){
			(*iterate)->KeyUp(myKey);

			++iterate;

		}
	
	}


}

char* KeyState::GetCStringOfEnum(KeyEvent keyEvent){

	if(keyEvent==KeyEvent::KeyDown){
		return "KeyDown";

	}else if(keyEvent==KeyEvent::KeyUp){
		return "KeyUp";
	}

	return "~~Default- No corresponding Key~~";
}

bool KeyState::IsKeyStateEmpty(){
	if(keyUpList.empty()&&keyDownList.empty()){
		return true;
	}
	return false;
}