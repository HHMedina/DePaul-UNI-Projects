#ifndef INPUTABLE_H
#define INPUTABLE_H

#include <Azul.h>
#include <map>
#include <set>
#include <list>
#include "KeyEventEnum.h"
using namespace std;

class SceneManager;
class Inputable{

	friend class KeyState;
	friend class InputManager;
public:

	virtual ~Inputable(){}

	/**\ingroup INPUT
		\brief Method used to register a wanted Input.

		This method is used to register a wanted key with a corresponding KeyEvent

		\param key key is an AZULKEY that will be registered by the user for the Inputable.
		\param kEventType kEventType is parameter for the user to clarify if the key is meant for a key up or key down event.

		\par Example:

		The following code registers a space Key Up event and a G key Down Event for the Ship.
		\code
		Ship::Ship(){
			
			
			//setting up graphical object omitted..
			
			//remember to Register for the desired key with the event!
			this->RegisterForInput(AZUL_KEY::KEY_G,KeyEvent::KeyDown);//registers for the key X with the KeyDown Event
			this->RegisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyUP);//registers for the key X with the KeyDown Event

		}

		void Ship::KeyDown(AZUL_KEY key){

			if(key ==AZUL_KEY::KEY_G ){ //if the key is G
				//do events corresponding to G keydown
			}
		}

		void Ship::KeyUp(AZUL_KEY key){

			if(key ==AZUL_KEY::KEY_SPACE ){ //if the key is Space
				//do events corresponding to Space key up
			}
		}

		//DONT FORGET TO DEREGISTER YOUR INPUTS!

		void Ship::OnTerminate(){
			this->DeregisterForInput(AZUL_KEY::KEY_G,KeyEvent::KeyDown);
			this->DeregisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyUp);
		}
		
		// void Ship::Draw() omitted.

		\endcode

	*/
	void RegisterForInput(AZUL_KEY key, KeyEvent kEventType = KeyEvent::KeyUpAndDown);

	/**\ingroup INPUT
		\brief Method used to deregister a no longer wanted registered Input.

		This method is used to deregister a  no longer wanted registered key with a corresponding KeyEvent

		\param key key is an AZULKEY that will be deregistered by the user for the Inputable.
		\param kEventType kEventType is parameter for the user to clarify if the key to be deregistered is for a key up or key down event.

		\par Example:

		The following code deregisters a G key Down Event for the Ship, when it gets destroyed from colliding into an asteroid.
		\code
		Ship::Ship(){
					
			//setting up graphical object omitted..
			
			//remember to Register for the desired key with the event!
			this->RegisterForInput(AZUL_KEY::KEY_G,KeyEvent::KeyDown);//registers for the key X with the KeyDown Event

		}

		//void Ship::KeyDown(AZUL_KEY key) omitted..
		//void Ship::KeyUp(AZUL_KEY key) omitted..

		//DONT FORGET TO DEREGISTER YOUR INPUTS!

		void Ship::OnTerminate(){
			this->DeregisterForInput(AZUL_KEY::KEY_G,KeyEvent::KeyDown);
		}
		
		// void Ship::Draw() omitted.

		void Ship::Collision(Asteroid* other){

			this->MarkForDestroy(); //will result in OnTerminate being Called
		}

		\endcode

	*/
	void DeregisterForInput(AZUL_KEY key, KeyEvent kEventType = KeyEvent::KeyUpAndDown);
	
	void DeregisterAllInputKeys();

protected:
	
	Inputable(){}
	Inputable( const Inputable & rhs){ rhs;}
	Inputable& operator = (const Inputable& rhs){ rhs;}

	/**\ingroup INPUT
		\brief CallBack when a key is pressed.

		This function is called whenever one of the associated registered Key Down Event keys, for this Inputable , is pressed.

		\param key key is a registered  AZUL_KEY with a KeyDown Event that was pressed during the frame.

		\par Example:

		The following code shows a Bomb exploding with the user presses the x key.
		\code
		Bomb::Bomb(){
			
			
			//setting up graphical object omitted..
			
			//remember to Register for the desired key with the event!
			this->RegisterForInput(AZUL_KEY::KEY_X,KeyEvent::KeyDown);//registers for the key X with the KeyDown Event

		}

		void Bomb::KeyDown(AZUL_KEY key){

			if(key ==AZUL_KEY::KEY_X ){ //if the key is X
				this->Explode();
			}
		}

		void Bomb::Explode(){
			new Explosion(); // spawn explosion
			this->MarkForDestroy(); // destroy self
		}

		// void Bomb::Draw() omitted.

		\endcode

	*/
	virtual void KeyDown(AZUL_KEY key){key;/*left empty in case of no event in inputable*/}

	/**\ingroup INPUT
		\brief CallBack when a key is released.

		This function is called whenever one of the associated registered Key Up Event keys, for this Inputable , is released.

		\param key key is a registered  AZUL_KEY with a KeyUp Event that was released during the frame.

		\par Example:

		The following code shows a ship spawning a bullet when the space key is released
		\code
		Ship::Ship(){

			//setting up graphical object omitted..

			this->RegisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyUp);//registers for the space down event

		}

		void Ship::KeyUp(AZUL_KEY key){

			if(key ==AZUL_KEY::KEY_SPACE ){ //if they key is space
				//Spawn Bullet 
				new Bullet();
			}
		}

		// void Ship::Draw() omitted..

		\endcode

	*/
	virtual void KeyUp(AZUL_KEY key){key;/*left empty in case of no event in inputable*/}

	struct KeyPair
	{
		KeyPair(AZUL_KEY key, KeyEvent kEvent){
			myKey = key;
			myKeyEvent = kEvent;
		}
		AZUL_KEY myKey;
		KeyEvent myKeyEvent;

		bool operator ==(const KeyPair& rhs)const;

	};
	
		
	list<KeyPair> myRegisteredKeys;

	void AddRegisteredKeyPair(Inputable::KeyPair pair);

	void RemoveRegisteredKeyPair(Inputable::KeyPair pair);
};
#endif