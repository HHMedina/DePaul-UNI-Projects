#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <set>
#include "CollidableGroupBase.h"
#include "CollidableGroup.h"
#include "CollisionProcessorBase.h"
#include "CollisionSingleProcessor.h"
#include "CollisionPairProcessor.h"

class CollisionManager{

	friend class Scene; // used to allow scene to create CollisionManagers and restrict user creation
	
public:
		
	/**
	\brief This method sets a pair for the collision system to consider.

	This method send a pair of Class C1 and C2 into the CollisionManager which will then
	check for collisions between registered C1 types and registered C2 types.

	For example, SceneManager::SetCollisionPair<Ship,Asteroid>(); , allows Ships to collide
	with Asteroids.
	*/
	template<class C1, class C2>
	void SetCollisionPair(){
		colProcessorCol.push_front(new CollisionPairProcessor<C1,C2>() );
	}

	/**
	\brief This method sets Class C to be able to collide with itself.

	This method send creates a new CollisionSingleProcessor with a class of C, and
	places it into the collision processor groups for collision consideration.

	For example, SceneManager::SetCollisionSelf<Ship>(); , allows Ships to collide
	with one another.
	*/
	template<class C>
	void SetCollisionSelf(){
		colProcessorCol.push_front(new CollisionSingleProcessor<C>());
		
	}

	
	
private:

	
	typedef  std::list<CollisionProcessorBase*> CollisionProcessorCollection;

	CollisionManager(){
		colProcessorCol = std::list<CollisionProcessorBase*>();
	} //dissallow User to create CollisionManagers

	CollisionManager( const CollisionManager & rhs){ rhs;} //dissallow User to copy 
	CollisionManager& operator = (const CollisionManager& rhs){//dissallow User to assign
		rhs;
		return *this;
	}

	/**
	\brief Cleans up the CollidableGroupBase and all of the CollisionProcessorBase.

	*/
	~CollisionManager(){
	
		//delete collision processor groups
		std::list< CollisionProcessorBase*>::iterator it2 = colProcessorCol.begin();
		while(it2!=colProcessorCol.end()){
			delete (*it2);//(*it)->DeleteMe();
			it2 = colProcessorCol.erase(it2);
		}

	}


	/**
	\brief This method is called by a Scene at every frame to check for collisions.

	This method calls CollisionTest() through all of the CollisionProcessors, whether single or paired, and checks if
	any collisions have happened.

	*/
	void ProcessCollisions(){
		std::list< CollisionProcessorBase*>::iterator it2 = colProcessorCol.begin();
		while(it2!=colProcessorCol.end()){
			(*it2)->CollisionTest();
			++it2;
		}
	}

	
	/**
	\brief This is the list of pairs and self collisions considered for collisions.
	*/
	CollisionProcessorCollection colProcessorCol;
};
#endif