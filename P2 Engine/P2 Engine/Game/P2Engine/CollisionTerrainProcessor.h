#ifndef COLLISIONTERRAINPAIR_H
#define COLLISIONTERRAINPAIR_H

#include "Terrain.h"
#include "DebugVisualizer.h"
#include "AABBBoundingBox.h"
#include "Azul.h"

template <class C1>
class CollisionTerrainProcessor: public CollisionProcessorBase{
	
protected:
	friend class CollisionManager;

	CollisionTerrainProcessor()
	{
		CollisionGroup1 = &CollidableGroup<C1>::Instance();
		Matrix tempMatrix;
		Vect zeroVect;
		fakeAABB = new AABBBoundingBox(zeroVect,zeroVect,tempMatrix);
	}
	CollisionTerrainProcessor( const CollisionTerrainProcessor & ){}
	CollisionTerrainProcessor& operator = (const CollisionTerrainProcessor& ){ }
	virtual ~CollisionTerrainProcessor(){delete fakeAABB;}

	/**
	\brief Checks for collisions between all registered objects of type C1 against all registered types of C2.

	This method checks between collisions between registered objects of type C1 and C2. If a collision is found,
	then it calls the callback of both objects.
	*/
	virtual void CollisionTest(){
		

		//check if group 1 has nothing
		if(CollisionGroup1->GetGroupSize()<=0){
			return;//one has nothing so cant have collisions
		}

		currentTerrain = SceneManager::GetCurrentTerrain();
		
		for(CollidableGroup<C1>::CollidableCollection::iterator itC1 = CollidableGroup<C1>::Instance().collideCol.begin();itC1 != CollidableGroup<C1>::Instance().collideCol.end();++itC1){ // for each registered <C> entity
			
			bool collisionHappened =false;

			Vect position = (*itC1)->GetCheapSphereCenter();
			float sphereRadius =(*itC1)->GetCheapSphereRadius();

			//DebugVisualizer::ShowSphere(position,sphereRadius);
			//retrieve likely candidate cells for Collidable
			//list<Terrain::MaxAndMinPoints> candidates = currentTerrain->GetLikelyCandidates(*itC1);

			//test if collidable is colliding with any of the likely candidates
			list<Terrain::MaxAndMinPoints>::iterator it;
			list<Terrain::MaxAndMinPoints>::iterator candEnd;

			currentTerrain->GetLikelyCandidates((*itC1),it,candEnd);

			while(it != candEnd&&!collisionHappened){
				if(P2Math::TestAABBSphereCollision((*it).MinPoint,(*it).MaxPoint,position,sphereRadius)){//test against sphere
					
					Matrix  id(MatrixSpecialType::IDENTITY);
					//id.set(MatrixSpecialType::IDENTITY);
					//create temp ABB for testing purposes.....for now....
					//AABBBoundingBox temp((*it).MinPoint,(*it).MaxPoint,id);
					fakeAABB->minAABBPoint = (*it).MinPoint;
					fakeAABB->maxAABBPoint = (*it).MaxPoint;
					fakeAABB->UpdateCorners();
					if(Collidable::TestCollisionPair((*itC1)->colVolume,fakeAABB)){//check collision with real ColVol
						collisionHappened =true;
						//test agianst terrain
						
						DebugVisualizer::ShowAABB((*it).MinPoint,(*it).MaxPoint,Vect(1,0,1));
						(*itC1)->TerrainCollision();
					}
					//else{
					//	//DebugVisualizer::ShowAABB((*it).MinPoint,(*it).MaxPoint,Vect(0,1,0));
					//}
				}else{
					
					//DebugVisualizer::ShowAABB((*it).MinPoint,(*it).MaxPoint,Vect(0,0,1));
				}
				++it;
			}
		}	
		
	}
	
	/**
	\brief The first of the collision groups in this pair wise collision Processor
	*/
	CollidableGroupBase* CollisionGroup1;

	Terrain*currentTerrain;

	AABBBoundingBox* fakeAABB;

};
#endif 