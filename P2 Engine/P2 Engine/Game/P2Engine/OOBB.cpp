#include "OOBB.h"
#include "BoundingSphereVolume.h"
#include "AABBBoundingBox.h"
#include "Collidable.h"
#include "P2Math.h"
#include "DebugVisualizer.h"


OOBB::OOBB(Collidable* d){
	myOwner =d;
	maxOOBB = myOwner->collisionModel->maxPointAABB;
	minOOBB = myOwner->collisionModel->minPointAABB;
	ComputeCorners();
	UpdateValues();
		
	Vect scale = (maxOOBB-minOOBB);
	tempPos = ((maxOOBB+minOOBB)*.5f);
	
	translationM.set(TRANS,tempPos);
	scaleM.set(SCALE,scale[x],scale[y],scale[z]);
	rotationM.set(ROT_XYZ,0,0,0);
	
	//update cheap sphere vars
	cheapSphereRadius = (scale).mag()*.5f;
	//DrawColVolume(Vect(1,0,1));
	
	}

bool OOBB::AcceptCollisionTest(CollisionVolume* other){
	return other->CollisionTest(this);
}

bool OOBB::CollisionTest(BoundingSphereVolume* other){
	
	return P2Math::TestSphereOBBCollision(other,this);

}

bool OOBB::CollisionTest(AABBBoundingBox* other){

	
	return P2Math::TestAABBOBBCollision(other,this);
	
}

const Matrix OOBB::GetRenderMatrix(){

	return scaleM*rotationM*translationM*myOwner->colWorld;
}

bool OOBB::CollisionTest(OOBB* other){
	
	return P2Math::TestOBBOBBCollision(this,other);

}

void OOBB::UpdateValues(){

	ComputeCorners();
	//DrawColVolume(Vect(0,0,1));
	
}

void OOBB::ComputeCorners(){

	//back face corners
	cornersOfOOBB[0] = Vect(minOOBB[x],maxOOBB[y],minOOBB[z],1)*myOwner->colWorld;
	cornersOfOOBB[1] = Vect(maxOOBB[x],maxOOBB[y],minOOBB[z],1)*myOwner->colWorld;
	cornersOfOOBB[2] = Vect(minOOBB[x],minOOBB[y],minOOBB[z],1)*myOwner->colWorld;
	cornersOfOOBB[3] = Vect(maxOOBB[x],minOOBB[y],minOOBB[z],1)*myOwner->colWorld;

	//front face corners
	cornersOfOOBB[4] = Vect(minOOBB[x],maxOOBB[y],maxOOBB[z],1)*myOwner->colWorld;
	cornersOfOOBB[5] = Vect(maxOOBB[x],maxOOBB[y],maxOOBB[z],1)*myOwner->colWorld;
	cornersOfOOBB[6] = Vect(minOOBB[x],minOOBB[y],maxOOBB[z],1)*myOwner->colWorld;
	cornersOfOOBB[7] = Vect(maxOOBB[x],minOOBB[y],maxOOBB[z],1)*myOwner->colWorld;

}

void OOBB::DrawColVolume(Vect& color){
	DebugVisualizer::ShowOBB(minOOBB,maxOOBB,myOwner->colWorld ,color);
}