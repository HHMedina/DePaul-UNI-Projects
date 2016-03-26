#ifndef COLLISIONPAIRPROCESSOR_H
#define COLLISIONPAIRPROCESSOR_H
#include "CollisionProcessorBase.h"
#include "CollidableGroup.h"
#include "CollisionVolume.h"
#include "BoundingSphereVolume.h"

#define COLPAIRDEBUG

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

		//check if group 1 has nothing
		if(CollisionGroup1->GetGroupSize()<=0){
			return;//one has nothing so cant have collisions
		}

		//check if group 2 has nothing
		if(CollisionGroup2->GetGroupSize()<=0){
			return;
		}

		
		if(!P2Math::TestAABBAABBCollision(CollisionGroup1->groupAABBMin,CollisionGroup1->groupAABBMax,CollisionGroup2->groupAABBMin,CollisionGroup2->groupAABBMax)){//if collision groups do not collide	
			return; //no need to continue
		}
#ifdef COLPAIRDEBUG
		else{
			//debug only
			//DebugVisualizer::ShowAABB(CollisionGroup1->groupAABBMin,CollisionGroup1->groupAABBMax,Vect(1,1,0));
			//DebugVisualizer::ShowAABB(CollisionGroup2->groupAABBMin,CollisionGroup2->groupAABBMax, Vect(1,1,0));
			//end debug
		}
#endif
		for(CollidableGroup<C1>::CollidableCollection::iterator itC1 = CollidableGroup<C1>::Instance().collideCol.begin();itC1 != CollidableGroup<C1>::Instance().collideCol.end();++itC1){ // for each registered <C> entity
			
			//check individual against group
			if(P2Math::TestAABBAABBCollision((*itC1)->cheapAABB->GetMinAABBPoint(),(*itC1)->cheapAABB->GetMaxAABBPoint(),CollidableGroup<C2>::Instance().groupAABBMin,CollidableGroup<C2>::Instance().groupAABBMax)){

				//DebugVisualizer::ShowAABB((*itC1)->cheapAABB->GetMinAABBPoint(),(*itC1)->cheapAABB->GetMaxAABBPoint(), Vect(1,0,1));		

				for(CollidableGroup<C2>::CollidableCollection::iterator it_C_Other = CollidableGroup<C2>::Instance().collideCol.begin();it_C_Other != CollidableGroup<C2>::Instance().collideCol.end() ;++it_C_Other){ // For each registered <C_Other> Entity

					//test cheap sphere versus cheap sphere
					if(P2Math::TestSphereSphereCollision( (*itC1)->cheapSphere,(*it_C_Other)->cheapSphere ) ){
						
						//DebugVisualizer::ShowSphere((*itC1)->cheapSphere->GetCenter(),(*itC1)->cheapSphere->GetRadius(), Vect(1,1,1));
						//DebugVisualizer::ShowSphere((*it_C_Other)->cheapSphere->GetCenter(),(*it_C_Other)->cheapSphere->GetRadius(), Vect(1,1,1));
						collisionHappened = Collidable::TestCollisionPair((*itC1)->colVolume,(*it_C_Other)->colVolume);// test for collision

						//Calls callbacks if a collision happened between a <c> class entity and a <C_Other> class entity
						if(collisionHappened){
					
							(*itC1)->Collision(*it_C_Other);
							(*it_C_Other)->Collision(*itC1);
						}
					}
				}
				
			}
		}

		//debug only
		//DebugVisualizer::ShowAABB(CollisionGroup1->groupAABBMin,CollisionGroup1->groupAABBMax);
		//DebugVisualizer::ShowAABB(CollisionGroup2->groupAABBMin,CollisionGroup2->groupAABBMax);
		//end debug

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