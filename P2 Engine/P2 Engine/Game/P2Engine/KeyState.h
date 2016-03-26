#ifndef KEYSTATE_H
#define KEYSTATE_H

#include <Azul.h>
#include <list>
#include "Inputable.h"
#include "KeyEventEnum.h"
using namespace std;
/*
enum KeyEvent{
		KeyDown, //0
		KeyUp, //1

	};
*/
class KeyState{

	//Only Allow private member access to InputManager
	//no one else should be manipulating the keyStates
	friend class InputManager;

private:

	//used to convert enums
	static char* GetCStringOfEnum(KeyEvent key);

	KeyState(AZUL_KEY key);
	KeyState(KeyState& rhs){ rhs;}
	KeyState& operator = (KeyState& rhs){rhs;}
	~KeyState();
		

	
	/**
	\brief Registers an Inputable into the corresponding KeyEvent list.

	This method will register an Inputable into the specified KeyEvent list.
	If the Inputable is already in the specified list, do nothing. Returns
	true if succesful or false if it failed.
	
	\param target target is the inputable to register.
	\param kE kE is the KeyEvent to register it for

	*/
	bool Register(Inputable* target,KeyEvent kE);

	/**
	\brief Deregisters an Inputable from the corresponding KeyEvent list.

	This method will deregister an Inputable from the specified KeyEvent list.
	If the Inputable is not in the list, do nothing. Returns true if successful,
	or false if it failed.
	
	\param target target is the inputable to deregister.
	\param kE kE is the KeyEvent to register it for

	*/
	bool Deregister(Inputable*,KeyEvent);

	/**
	\brief Updates the state of the key.

	This method checks if the Key for this KeyState Object has been pressed by using CheckKeyDown()
	, checks if the key has been released by using CheckKeyUp(), and then records the current state of the
	key as the previous state.
	
	*/
	void Update();

	/**
	\brief Checks if the KeyState is empty.

	This function checks if the keystate is empty by checking if the keyUpList and
	keyDownList are both empty. The function returns true if both lists are empty. If the lists
	are not both empty, then it will return false.
	
	*/
	bool IsKeyStateEmpty();
	

	/**
	\brief The Key assigned to this KeyState object.
	
	*/	
	AZUL_KEY myKey;

	/**
	\brief Bool used to hold if the previous state of pressed (true) or not pressed (false)
	
	*/
	bool previousState;

	/**
	\brief List of Inputables Interested in the KeyUp Event for the key myKey.
	
	This list holds the inputables that want their callback KeyUp() called when the key myKey
	is released. 
	*/
	list<Inputable*> keyUpList;

	/**
	\brief List of Inputables Interested in the KeyDown Event for the key myKey.
	
	This list holds the inputables that want their callback KeyDown() called when the key myKey
	is pressed. 
	*/
	list<Inputable*> keyDownList;
	list<Inputable*>::iterator iterate;
	
	/**
	\brief Checks if key myKey has been released by comparing it to previousState. 
	*/
	void CheckKeyUp();

	/**
	\brief Checks if key myKey has been pressed by comparing it to previousState. 
	*/
	void CheckKeyDown();

};


#endif 