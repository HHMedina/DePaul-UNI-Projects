#include <Collidable.h>
#include <iostream>
#include "CollisionVolume.h"
#include "BoundingSphereVolume.h"
#include "AABBBoundingBox.h"
#include "OOBB.h"

using namespace std;

Collidable::Collidable(){
	colVolume=NULL;
	collisionModel =NULL;
	colWorld.set(IDENTITY);
	cheapSphere = new BoundingSphereVolume(this);
	cheapAABB = new AABBBoundingBox(this);
	
}

Collidable::~Collidable(){
	if(colVolume!=NULL){
		delete colVolume;
	}
	delete cheapAABB;
	delete cheapSphere;

}

Vect& Collidable::GetCheapSphereCenter(){
	return cheapSphere->centerPosition;
}

float Collidable::GetCheapSphereRadius(){
	return cheapSphere->radius;
}


void Collidable::SetCollider(Model* collider,CollisionVolumeType type){
	collisionModel = collider;
	if(type==CollisionVolumeType::SphereVolume){
		colVolume = new BoundingSphereVolume(this);
	}else if(type==CollisionVolumeType::AABBBoxVolume){
		colVolume = new AABBBoundingBox(this);
	}else if(type == OOBBVolume){
		colVolume = new OOBB(this);
	}

	//for debugging
	colVolume->UpdateValues();
	cheapSphere->UpdateValues();
	cheapAABB->UpdateValues();

}

bool Collidable::TestCollisionPair(CollisionVolume* a, CollisionVolume* b){
	return a->AcceptCollisionTest(b);
}

void Collidable::SetColWorld(Matrix& world){
	colWorld = world;
	if(this->collisionModel!=NULL){
		colVolume->UpdateValues();
		cheapAABB->UpdateValues();
		cheapSphere->UpdateValues();
	}
	
}
CollisionVolume& Collidable::GetCollisionVolume(){
	return *colVolume;
}