#include "BoundingSphereVolume.h"
#include "AABBBoundingBox.h"
#include "P2Math.h"
#include "OOBB.h"
#include "DebugVisualizer.h"


BoundingSphereVolume::BoundingSphereVolume(Collidable* d){
	myOwner =d;
	centerPosition = Vect(0,0,0,0);
	radius =0.0f;
	//UpdateValues();

}
bool BoundingSphereVolume::AcceptCollisionTest(CollisionVolume* other){
	return other->CollisionTest(this);
}

bool BoundingSphereVolume::CollisionTest(BoundingSphereVolume* other){

	return P2Math::TestSphereSphereCollision(this,other);

}

bool BoundingSphereVolume::CollisionTest(AABBBoundingBox* other){

	return P2Math::TestAABBSphereCollision(other,this);

}

bool BoundingSphereVolume::CollisionTest(OOBB* other){

	return P2Math::TestSphereOBBCollision(this,other);

}

void BoundingSphereVolume::UpdateValues(){

	//initial values
	Vect initialVector(0,0,1,0);//arbitrary unit vector to find scaling Factor
	float initialRadius = myOwner->collisionModel->radius;
	Vect initialCenter = myOwner->collisionModel->center;

	//new values after applying world
	centerPosition = initialCenter * myOwner->colWorld;
	cheapCenter = centerPosition;
	Vect transFormedVector = initialVector * myOwner->colWorld;
	float scaleFactor = (transFormedVector).mag(); // vector length is not affected by translation or rotation
	radius = initialRadius*scaleFactor;

	cheapSphereRadius = radius;

	translationM.set(TRANS,centerPosition);
	scaleM.set(SCALE,radius,radius,radius);
	rotationM.set(ROT_XYZ,80,80,0);	

}

const Matrix BoundingSphereVolume::GetRenderMatrix(){

	return scaleM*rotationM*translationM;
}

void BoundingSphereVolume::DrawColVolume(Vect& color){
	DebugVisualizer::ShowSphere(centerPosition,radius,color);
}