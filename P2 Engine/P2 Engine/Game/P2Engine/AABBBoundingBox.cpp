#include "AABBBoundingBox.h"
#include "Collidable.h"
#include "P2Math.h"
#include "BoundingSphereVolume.h"
#include "OOBB.h"
#include "DebugVisualizer.h"

bool AABBBoundingBox::AcceptCollisionTest(CollisionVolume* other){
	return other->CollisionTest(this);
}


bool AABBBoundingBox::CollisionTest(BoundingSphereVolume* other){
	
	return P2Math::TestAABBSphereCollision(this,other);

	
}

bool AABBBoundingBox::CollisionTest(AABBBoundingBox* other){

	return P2Math::TestAABBAABBCollision(this,other);

}

bool AABBBoundingBox::CollisionTest(OOBB* other){

	
	return P2Math::TestAABBOBBCollision(this,other);
	
}

void AABBBoundingBox::DrawColVolume(Vect& color){
	DebugVisualizer::ShowAABB(minAABBPoint,maxAABBPoint,color);
}

void AABBBoundingBox::UpdateValues(){
		
	ownerWorld =  myOwner->colWorld;
	UpdateCorners();

	Vect currentVert = myOwner->collisionModel->getVectList()[0] *ownerWorld;

	//Place-holder for min and max values
	float minX = currentVert[x];
	float minY = currentVert[y];
	float minZ = currentVert[z];

	float maxX = currentVert[x];
	float maxY = currentVert[y];
	float maxZ = currentVert[z];

	//get vector list.... for each... transform with world matrix, check for min and max for each axis
	for(int i = 0; i<myOwner->collisionModel->numVerts ;++i){
		currentVert = myOwner->collisionModel->getVectList()[i]* ownerWorld;
		
		//check for min and max X value
		minX = P2Math::Min(minX,currentVert[x]);
		maxX = P2Math::Max(maxX,currentVert[x]);

		//check for min and max Y Value
		minY = P2Math::Min(minY,currentVert[y]);
		maxY = P2Math::Max(maxY,currentVert[y]);

		//check for min and max on Z axis
		minZ = P2Math::Min(minZ,currentVert[z]);
		maxZ = P2Math::Max(maxZ,currentVert[z]);

	}

	//update new AABB points
	minAABBPoint[x] = minX;
	minAABBPoint[y] = minY;
	minAABBPoint[z] = minZ;

	maxAABBPoint[x] = maxX;
	maxAABBPoint[y] = maxY;
	maxAABBPoint[z] = maxZ;

	Vect scale = maxAABBPoint-minAABBPoint;
	Center = (minAABBPoint+maxAABBPoint)*.5f;
	translationM.set(TRANS,Center);
	scaleM.set(SCALE,scale[x],scale[y],scale[z]);
	rotationM.set(ROT_XYZ,0,0,0);
	
	//DebugVisualizer::ShowAABB(minAABBPoint,maxAABBPoint);
}

const Matrix AABBBoundingBox::GetRenderMatrix(){
	return scaleM*rotationM*translationM;
}

void AABBBoundingBox::UpdateCorners(){

	//treated as OOBB
	Vect min = minAABBPoint;
	Vect max = maxAABBPoint;

	//back face corners
	cornersOfAABB[0] = Vect(min[x],max[y],min[z],1);
	cornersOfAABB[1] = Vect(max[x],max[y],min[z],1);
	cornersOfAABB[2] = Vect(min[x],min[y],min[z],1);
	cornersOfAABB[3] = Vect(max[x],min[y],min[z],1);

	//front face corners
	cornersOfAABB[4] = Vect(min[x],max[y],max[z],1);
	cornersOfAABB[5] = Vect(max[x],max[y],max[z],1);
	cornersOfAABB[6] = Vect(min[x],min[y],max[z],1);
	cornersOfAABB[7] = Vect(max[x],min[y],max[z],1);




}

Vect& AABBBoundingBox::GetMinAABBPoint(){
	return minAABBPoint;
}

Vect& AABBBoundingBox::GetMaxAABBPoint(){
	return maxAABBPoint;
}