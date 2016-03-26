#ifndef COLLISIONSINGLEPROCESSOR_H
#define COLLISIONSINGLEPROCESSOR_H
#include "CollisionProcessorBase.h"
#include "CollidableGroup.h"
#include "Collidable.h"
#include "CollisionVolume.h"
template <class C>
class CollisionSingleProcessor: public CollisionProcessorBase{
	
protected:
	friend class CollisionManager;

	CollisionSingleProcessor()
	{
		CollisionGroup = &CollidableGroup<C>::Instance();
	}
	CollisionSingleProcessor( const CollisionSingleProcessor & ){}
	CollisionSingleProcessor& operator = (const CollisionSingleProcessor& ){ }
	virtual ~CollisionSingleProcessor(){}

	/**
	\brief Checks for collisions between all registered objects of type C against all other C1 types.

	This method checks between collisions between registered objects of type C. If a collision is found,
	then it calls the callback of both objects of type C.
	*/
	virtual void CollisionTest(){

		bool collisionHappened =false;

		for(CollidableGroup<C>::CollidableCollection::iterator it_mine = CollidableGroup<C>::Instance().collideCol.begin();it_mine != CollidableGroup<C>::Instance().collideCol.end();++it_mine){ // for each registered <C> entity

			for(CollidableGroup<C>::CollidableCollection::iterator it_Other = next(it_mine);it_Other != CollidableGroup<C>::Instance().collideCol.end();++it_Other){ // For each registered <C_Other> Entity
				//check cheap sphere versus cheap sphere
				if(P2Math::TestSphereSphereCollision((*it_mine)->cheapSphere,(*it_Other)->cheapSphere)){
					
					//DebugVisualizer::ShowSphere((*it_mine)->cheapSphere->GetCenter(),(*it_mine)->cheapSphere->GetRadius(), Vect(1,1,1));
					//DebugVisualizer::ShowSphere((*it_Other)->cheapSphere->GetCenter(),(*it_Other)->cheapSphere->GetRadius(), Vect(1,1,1));

					collisionHappened = Collidable::TestCollisionPair((*it_mine)->colVolume,(*it_Other)->colVolume);// test for collision

					//Calls callbacks if a collision happened between a <c> class entity and a <C_Other> class entity
					if(collisionHappened){
						(*it_mine)->Collision(*it_Other);
						(*it_Other)->Collision(*it_mine);
					}
				}
			}
		}

	}
	
	/**
	\brief The collision group of type C.
	*/
	CollidableGroupBase* CollisionGroup;
};

#endif