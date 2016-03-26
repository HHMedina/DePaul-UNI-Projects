#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <map>
#include <iostream>

class SceneManager;

using namespace std;

class Updatable{

	//Update Manager should have access to all Updateable functions
	friend class UpdatableManager;
public:

	/**\ingroup UPDATING
	\brief Registers Updatable For Updating.
	

	This function registers an Updatable for Updating

	\note GameObjects are automatically registered for updating upon creation. This example is to show how you use it, to reactivate an unregistered Updatable.

	\par Example:
	The following code shows setting up a bullet for Updating;
	\code

	Assumming the following has been defined in the  header of Bullet
	
		Matrix world;
		Matrix bulletScale;
		Vect bulletPos;
		Matrix bulletRot;


	Bullet::Bullet(){
		
		this->RegisterForUpdate(); //registers updatable and will call its Update() at every frame from now on, or until deregistered.

		//set's the gameObject's graphicsObject
		SetGraphicAndCollisionModel( new GraphicsObjectFlatTexture(AssetManager::GetModel("BulletModel"),AssetManager::GetTexture("BulletTexture")));
	
		//sets up initial rotation,scale and position in the world
		bulletScale.set( SCALE, 0.5f, 0.5f, 0.5f);
		bulletRot.set( ROT_XYZ, 0, 0, 0);
		bulletPos.set(0, 20, -40);
		world.set(IDENTITY);
		world *= bulletScale * bulletRot * Matrix( TRANS, bulletPos );

		SetGameObjectWorld(world); // set GameObject's (bullet) graphic model and collision model's world transform to world;
	}
	\endcode
	
	*/
	void RegisterForUpdate();


	/**\ingroup UPDATING
	\brief Deregisters Updatable From Updating.
	

	This function Deregisters an Updatable from Updating

	\note GameObjects are automatically registered for updating upon creation.

	\par Example:
	The following code shows deregistering a bullet from updating (essentially stopping it from moving) when it collides with a wall.
	This stopping may be unconventional but shows the deregistering in action. Another useful use of deregistering from update and input could be
	to stop a player from moving curing a cutscene.
	\code

	Assumming the following has been defined in the  header of Bullet
	
		Matrix world;
		Matrix bulletScale;
		Vect bulletPos;
		Matrix bulletRot;


	Bullet::Bullet(){
		
		// note: GameObjects are automatically registered for update.

		//set's the gameObject's graphicsObject
		SetGraphicAndCollisionModel( new GraphicsObjectFlatTexture(AssetManager::GetModel("BulletModel"),AssetManager::GetTexture("BulletTexture")));
	
		//sets up initial rotation,scale and position in the world
		bulletScale.set( SCALE, 0.5f, 0.5f, 0.5f);
		bulletRot.set( ROT_XYZ, 0, 0, 0);
		bulletPos.set(0, 20, -40);
		world.set(IDENTITY);
		world *= bulletScale * bulletRot * Matrix( TRANS, bulletPos );

		SetGameObjectWorld(world); // set GameObject's (bullet) graphic model and collision model's world transform to world;

		RegisterCollision(this);
	}


	Bullet::Update(){
		bulletPos + = Vector(0,0,1); //move bullet forward 1 unit each frame
		SetGameObjectWorld(world); //set changes for collision mesh and graphical model
	}

	Bullet::Collision(Wall* other){
		
		this->DeregisterForUpdate(); //causes update to no longer continue, stopping the bullet form updating.

	}

	\endcode
	
	*/
	void DeregisterForUpdate();


protected:

	Updatable(){
		RegisterForUpdate();
	}
	Updatable( const Updatable & rhs){ rhs;}
	Updatable& operator = (const Updatable& rhs){ rhs;}
	virtual ~Updatable(){}

	/**
	\ingroup DEFAULTEVENTS
	\brief This method is run at every frame.

	This method should be reimplemented by a derived class if they wish to have an update capability, in which, this method is called at every frame.

	\par Example:

	The following code shows the Update() method of a rocket, which moves upwards at one unit per frame.

	\code

	Assumming the following has been defined in the  header of Rocket
	
		Matrix world;
		Matrix rocketScale;
		Vect rocketPos;
		Matrix rocketRot;


	Rocket::Rocket(){
	
	
	//set up gameobject here
	SetGraphicsObject( new GraphicsObjectFlatTexture(AssetManager::GetModel("RocketModel"),AssetManager::GetTexture("RocketTexture")));
		

		// Initial Rocket scale, rotation, and position
		rocketScale.set( SCALE, 1f, 1f, 1f);
		rocketRot.set( ROT_XYZ, 0, 0, 0);
		rocketPos.set(0, 20, 0);
		world.set(IDENTITY);
		world *= rocketScale * rocketRot * Matrix( TRANS, rocketPos );
		
		// Rocket adjust matrix
		world =  rocketScale * rocketRot * Matrix( TRANS, rocketPos );
		SetGameObjectWorld(world);//set changes


	}

	void Rocket::Update(){
		
		rocketPos += Vect(0,1,0) * rocketSpeed;

		// rocketPos adjust matrix
		world =  rocketScale * rocketRot * Matrix( TRANS, rocketPos );
		SetGameObjectWorld(world);//set changes
	}

	\endcode

	*/
	virtual void Update(){}
	
		

};
#endif