#ifndef COLLISIONPAIRPROCESSOR_H
#define COLLISIONPAIRPROCESSOR_H
#include "CollisionProcessorBase.h"
#include "CollidableGroup.h"

template <class C1, class C2>
class CollisionPairProcessor: public CollisionProcessorBase{
	
protected:
	friend class CollisionManager;

	CollisionPairProcessor()
	{
		CollisionGroup1 = &CollidableGroup<C1>::Instance();
		CollisionGroup2 = &CollidableGroup<C2>::Instance();
	}
	CollisionPairProcessor( const CollisionPairProcessor & ){}
	CollisionPairProcessor& operator = (const CollisionPairProcessor& ){ }
	virtual ~CollisionPairProcessor(){}

	/**
	\brief Checks for collisions between all registered objects of type C1 against all registered types of C2.

	This method checks between collisions between registered objects of type C1 and C2. If a collision is found,
	then it calls the callback of both objects.
	*/
	virtual void CollisionTest(){

		bool collisionHappened =false;

		for(CollidableGroup<C1>::CollidableCollection::iterator itC1 = CollidableGroup<C1>::Instance().collideCol.begin();itC1 != CollidableGroup<C1>::Instance().collideCol.end();++itC1){ // for each registered <C> entity

			for(CollidableGroup<C2>::CollidableCollection::iterator it_C_Other = CollidableGroup<C2>::Instance().collideCol.begin();it_C_Other != CollidableGroup<C2>::Instance().collideCol.end() ;++it_C_Other){ // For each registered <C_Other> Entity

				collisionHappened = Collidable::TestCollisionPair(*itC1,*it_C_Other);// test for collision

				//Calls callbacks if a collision happened between a <c> class entity and a <C_Other> class entity
				if(collisionHappened){
					(*itC1)->Collision(*it_C_Other);
					(*it_C_Other)->Collision(*itC1);
				}
			}
		}

	}
	
	/**
	\brief The first of the collision groups in this pair wise collision Processor
	*/
	CollidableGroupBase* CollisionGroup1;

	/**
	\brief The second of the collision groups in this pair wise collision Processor
	*/
	CollidableGroupBase* CollisionGroup2;
};

#endif