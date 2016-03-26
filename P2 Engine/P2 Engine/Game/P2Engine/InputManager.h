#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H


#include <map>
#include <list>
#include "Inputable.h"
#include "KeyState.h"

using namespace std;


class InputManager{

	//set to allow Scene to only create input managers and manage them.
	//prevents user from creating and touching inputManager directly
	//note: input manager can be accessed through scene only.
	friend class Scene;

private:
	
	/**
	\brief The list used to hold the keystate for each registered key.
	*/
	map<AZUL_KEY,KeyState*> registeredKeyStates;
	map<AZUL_KEY,KeyState*>::iterator iterate;
	
	
	/**
	\brief This method iterated through the registeredKeyStates list and calls their
	Update().
	
	*/
	void Update();
	
	InputManager(){
		registeredKeyStates = map<AZUL_KEY,KeyState*>();
	}
	InputManager( const InputManager & rhs){ rhs;}
	InputManager& operator = (const InputManager& rhs){ rhs; return *this;}
	~InputManager();

public:

	/**\ingroup INPUT
		\brief Method that returns the state of the key in the current frame

		This method allows the user to look at the state of a key at the frame the method is called.
		The method will return True is the key is being pressed at that frame.
		This method can be used for a continuous keydown event if used in the Update() method of an Updatable.

		\param key key is the Azul_Key that should have its state checked.
		\par Example:

		The following code shows a ship's movement controls by using the InputManager's IsKeyPressed() Method.
		\code

		Asumming the following are declared in the header
			float ShipTransSpeed;
			float ShipRotAng;
			Matrix world;
			Matrix ShipScale;
			Vect ShipPos;
			Matrix ShipRot;

		Ship::Ship(){

			SetGraphicsObject( new GraphicsObjectFlatTexture(AssetManager::GetModel("SpaceShip"),AssetManager::GetTexture("ShipTex")));

			//set speeds
			ShipRotAng = .05f;
			ShipTransSpeed = 1;

			//set initial Ship Location
			ShipScale.set( SCALE, 0.5f, 0.5f, 0.5f);
			ShipRot.set( ROT_XYZ, 0, 0, 0);
			ShipPos.set(0, 20, -40);
			world.set(IDENTITY);
			world *= ShipScale * ShipRot * Matrix( TRANS, ShipPos );
		}

		void Ship::Update(){

			//Ship Translation movement
			if (  InputManager::IsKeyPressed(AZUL_KEY::KEY_W))
			{
				ShipPos += Vect(0,0,1) * ShipRot * ShipTransSpeed;
			}
			else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_S) )
			{
				ShipPos += Vect(0,0,1) * ShipRot * - ShipTransSpeed;
			}
	
			// Ship Rotation movement (not using time-based values for simplicity)
			if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_A) )
			{
				ShipRot *= Matrix( ROT_Y, ShipRotAng );
			}
			else if ( InputManager::IsKeyPressed(AZUL_KEY::KEY_D))
			{
				ShipRot *= Matrix( ROT_Y, -ShipRotAng );
			}

			// Spaceship adjust matrix
			world =  ShipScale * ShipRot * Matrix( TRANS, ShipPos );
			SetGameObjectWorld(world);

		}

		void Ship::Draw(){
			RenderGraphics();
		}

		\endcode

	*/
	static bool IsKeyPressed(AZUL_KEY key);


	
	/**
	\brief Register a Inputable for a Key in a certain KeyEvent.

	This method will create a KeyState for the desired key to be considered
	and will put it into the registeredKeyStates list. If they KeyState does not exist,
	it will create one. If it already exists, the Inputable will be added to the
	already existing one's KeyUp or KeyDown list. If inputable is already in that list, do nothing.
	
	\param in in is the inputable to register
	\param key key is the key to be registered for.
	\param keyEvent keyEvent is the KeyEvent to be registered for.

	*/
	void RegisterInput(Inputable* in,AZUL_KEY key,KeyEvent keyEvent);

	
	/**
	\brief Deregister an Inputable for a Key in a certain KeyEvent.

	This method will remove an Inputable from a KeyState with a certain KeyEvent.
	If the KeyState does not exist, do nothing.If the KeyState does exist, then
	remove it from the list of KeyUp or KeyDown Inputables. If the Inputable removal
	makes the list empty, delete that KeyState.

	\param target target is the inputable to deregister
	\param key key is the key to be registered for.
	\param keyEvent keyEvent is the KeyEvent to be registered for.
	
	*/
	void DeregisterInput(Inputable* target, AZUL_KEY key, KeyEvent keyEvent);


};




#endif