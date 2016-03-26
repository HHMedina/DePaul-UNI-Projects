#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include <set>
#include "CollidableGroupBase.h"
#include "CollidableGroup.h"
#include "CollisionProcessorBase.h"
#include "CollisionSingleProcessor.h"
#include "CollisionPairProcessor.h"
#include "CollisionTerrainProcessor.h"

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

		//add group base to list
		colGroupCol.insert(&CollidableGroup<C1>::Instance());
		colGroupCol.insert(&CollidableGroup<C2>::Instance());
	}

	template <class C>
	void SetCollisionWithTerrain(){

		colProcessorCol.push_front(new  CollisionTerrainProcessor<C>() );
		//add group base to list
		colGroupCol.insert(&CollidableGroup<C>::Instance());
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
		colGroupCol.insert(&CollidableGroup<C>::Instance());
	}

	
	
private:

	
	typedef  std::list<CollisionProcessorBase*> CollisionProcessorCollection;
	typedef  std::set<CollidableGroupBase*> ColGroupCollection;

	CollisionManager(){
		colGroupCol = set<CollidableGroupBase*>();
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

		colGroupCol.clear();

	}

	void UpdateGroupAABS(){
		std::set< CollidableGroupBase*>::iterator it2 = colGroupCol.begin();
		while(it2!=colGroupCol.end()){
			(*it2)->UpdateGroupAABB();
			++it2;
		}

	}

	/**
	\brief This method is called by a Scene at every frame to check for collisions.

	This method calls CollisionTest() through all of the CollisionProcessors, whether single or paired, and checks if
	any collisions have happened.

	*/
	void ProcessCollisions(){
		UpdateGroupAABS();

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

	//set of all of the collision groups
	ColGroupCollection colGroupCol;
	
};
#endif