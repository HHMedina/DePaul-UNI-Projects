#ifndef COLLIDABLE_H
#define COLLIDABLE_H

#include "Azul.h"
#include "CollidableGroup.h"
#include <assert.h>
#include "CollisionVolume.h"

class Collidable{
	
public:

	template<class T>
	friend class CollidableGroup;
	template<class T>
	friend class CollisionSingleProcessor;
	template<class T>
	friend class CollisionTerrainProcessor;
	template<class T,class T2>
	friend class CollisionPairProcessor;


	friend class CollisionVolume;
	friend class BoundingSphereVolume;
	friend class AABBBoundingBox;
	friend class OOBB;
	friend class P2Math;

	/**
	\ingroup COLLISION
	\brief This method sets the Collidable's collisionModel.
	
	This method sets the Collidable's collisionModel which is used for collision processing. A collision Model is required
	for collision checking, and will crash if you try to consider collisions with an object with no collision model.

	\param collider collider is of Model type, which is the mesh that the collision system will use for calculating occurances of collisions.


	\par Example:

	The following is a typical scenario where a ship's collisonModel is set to resemble the ship.

	\code

	Assuming that the following are included in the Ship header.

		Matrix world;
		Matrix shipScale;
		Vect shipPos;
		Matrix shipRot;

	Ship::Ship(){
		
		//setting up initial properties like visual repsentation omitted..

		//make the collision Model the shape of the SpaceShipModel
		SetCollider(AssetManager::GetModel("SpaceShipModel"));

		//set up the location, rotation, and scale of the ship
		shipScale.set( SCALE, 0.5f, 0.5f, 0.5f);
		shipRot.set( ROT_XYZ, 0, 0, 0);
		shipPos.set(0, 20, -40);
		world.set(IDENTITY);
		world *= shipScale * shipRot * Matrix( TRANS, shipPos );
		
		//commit the graphicsObject and the collisionModel to have the transforms
		//indicated by world
		SetGameObjectWorld(world);

		RegisterCollision(this); // register this ship for collision consideration
	}
		
	void Ship::OnTerminate(){
		
		DeregisterCollision(this); // Remember to deregister on termination!
	}
	
	//void Ship::Draw() omitted because collisionModels are not draw anyway..

	\endcode

	*/
	void SetCollider(Model * collider,CollisionVolumeType type = CollisionVolumeType::SphereVolume);


	
	template <typename C>
	/**
	\ingroup COLLISION
	\brief This method registers a Collidable for collision.
	
	This method registers a Collidable to the collision detection system in order for the
	Collidable to be considered for collision. If it is not registered, the object will not
	have its collision callbacks called.

	
	\param cPtr cPtr is the Collidable that is to be registered.


	\par Example:

	The following is an example of Asteroid registering itself to the collision system for collision consideration.

	\code

	Asteroid::Asteroid(){
		
		//setting up initial properties like visual repsentation omitted..

		//make the collision Model the shape of the Asteroid Model
		SetCollider(AssetManager::GetModel("AsteroidModel"));

		RegisterCollision(this); // register to collision system for consideration

	}

	
	void Asteroid::Collision(Ship* other){
		this->MarkForDestroy 
	}

	void Asteroid::OnTerminate(){

		 DeregisterCollision(this); //dont forget to Deregister for collision!
	}

	//void Asteroid::Draw() omitted..

	\endcode

	*/
	void RegisterCollision(C* cPtr){
		CollidableGroup<C>::Instance().Register(cPtr);
	}


	
	template <typename C>
	/**
	\ingroup COLLISION
	\brief This method Deregisters a Collidable from collision.
	
	This method deregisters a Collidable from collision, effectively disabling the collision callbacks, as well
	as the collisions themselves.

	\param cPtr cPtr is the Collidable that is to be deregistered.

	\note If you have registered for a collision, always deregister in OnTerminate() as a safeguard.
	 For an example on how to deregister, please see the example in RegisterCollision() .

	*/
	void DeregisterCollision(C* cPtr){
		CollidableGroup<C>::Instance().Deregister(cPtr);
	}

	/**
	\ingroup COLLISION
	\brief This method sets the transform of the CollisionModel
	
	\param world world is the matrix that the collidable's collision model will be set to.
		
	*/

	CollisionVolume& GetCollisionVolume();

	Vect& GetCheapSphereCenter();

	float GetCheapSphereRadius();

protected:

	
	void SetColWorld(Matrix& world);

	Collidable();
	Collidable( const Collidable & rhs){ rhs;}
	Collidable& operator = (const Collidable& rhs){ rhs;}
	
	virtual ~Collidable();

	/**
	\ingroup COLLISION
	\brief This is the callBack Method that will be called when one collidable hits another.
	
	This method is the callback Method that will be caled when one collisable hits another.
	This method is required to be present in the .h file of Collidables. This method should do nothing,
	but the other overloaded collisions will be called.

	\par Below is an example of setting up a correct Collision Callback.

	First the .h file.
	\code

	#include "P2Engine.h" (remove --... doxygen error)
	using namespace std;

	class Bucket;
	class Bomb:public GameObject{

	public:
	
		Bomb(float,float,float);
		Bomb( const Bomb & rhs){ rhs;}
		Bomb& operator = (const Bomb& rhs){ rhs;}
		virtual ~Bomb();
	
		void Initialize(float,float,float);

		virtual void Draw();
	
		//Alarms
		virtual void Alarm0();

		virtual void Update(){} 

		virtual void Collision(Collidable*){} // will not be called
		virtual void Collision(Bucket*);//will be called
		virtual void OnTerminate();
	
		Matrix bombScale;
		Vect bombPos;
		Matrix bombRot;
	

	};

	\endcone

	and the .cpp..

	\code
	//note:All other methods have been removed since they are irrelevant to collisions.

	Bucket::Bucket(float xPos , float yPos, float zPos){

		SetGraphicAndCollisionModel(GraphicsObjectMaker::CreateGraphicsObjectFlatTexture(AssetManager::GetModel("BucketModel"),AssetManager::GetTexture("BucketTexture")   ));
		RegisterForInput(AZUL_KEY::KEY_SPACE,KeyEvent::KeyDown);
		Matrix temp = Matrix();
		bucketScale.set( SCALE, .25f, .25f, .25f);
		bucketRot.set( ROT_XYZ, 0, 0, 0);
		bucketPos = Vect(xPos,yPos,zPos);
		temp = bucketScale * bucketRot * Matrix( TRANS, bucketPos );
		SetGameObjectWorld(temp);
		RegisterCollision(this);
	}

	void Bucket::Collision(Bomb*){

		GameController::IncrementBombsDiffused();
		GameController::SpawnRandomBomb();
	}

	void Bucket::OnTerminate(){
	
		DeregisterCollision(this);//dont forget to deregister

	}

	\endcode
	*/
	virtual void Collision(Collidable*){}
	
	virtual void TerrainCollision(){}

	static bool TestCollisionPair(CollisionVolume*,CollisionVolume*);
	/**
	\ingroup IMPORTANTCOLLISIONMEMBERS
	\brief This is the Model which is the representation of the collision mesh.

	This model will be used to calculate if another collidable has collided with this collidable
	*/
	Model* collisionModel;

	CollisionVolume* colVolume;

	/**
	\ingroup IMPORTANTCOLLISIONMEMBERS
	\brief This is the world Matrix of the collisonModel which holds its position, scale, and rotation.

	This Matrix will be used to set the rotation,location, and scale of the Collision Model.
	*/
	Matrix colWorld;

	
	BoundingSphereVolume* cheapSphere;
	AABBBoundingBox* cheapAABB;
};
#endif COLLIDABLE_H