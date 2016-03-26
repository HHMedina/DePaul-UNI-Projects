#include "P2Math.h"

#include "Collidable.h"
#include "CollisionVolume.h"
#include "AABBBoundingBox.h"
#include "BoundingSphereVolume.h"
#include "OOBB.h"
#include "Azul.h"

float P2Math::Min(float a,float b){
	if(a<b){
		return a;
	}
	return b;
}

int P2Math::ArrayIndexFrom2DTo1D(int row, int column, int numberOfCellsInRow){
	return (row *numberOfCellsInRow)+ column;
}

float P2Math::Clamp(float value,float minVal, float maxVal){
	if(value<minVal){
		return minVal;
	}else if(value>maxVal){
		return maxVal;
	}else{
		return value;
	}
}

float P2Math::Max(float a,float b){
	if(a>b){
		return a;
	}
	return b;
}

float P2Math::DegToRad(float deg){
	return (MATH_PI/180) *deg;
}

Vect P2Math::MaxVect(Vect&a , Vect&b){
	return Vect(Max(a[x],b[x]),Max(a[y],b[y]),Max(a[z],b[z]));
}

Vect P2Math::MinVect(Vect&a , Vect&b){
	return Vect(Min(a[x],b[x]),Min(a[y],b[y]),Min(a[z],b[z]));
}

bool P2Math::IsWithinInterval(float i1min, float i1max, float i2min , float i2max){

	bool intersection = false;
	//check case 1
	if(i1min<=i2min&&i1max>=i2min){
		intersection = true;
	}

		
	//check case 2
	if(i1min>=i2min&&i1max<=i2max){
		intersection = true;
	}

	//check case 3
	if(i1min<=i2max&&i1max>=i2max){
		intersection = true;
	}

	//check case4
	if(i1min<=i2min&&i1max>=i2max){
		intersection = true;
	}
	return intersection;
}

bool P2Math::TestAABBAABBCollision(AABBBoundingBox * a, AABBBoundingBox* b){

	return P2Math::IsWithinInterval(b->minAABBPoint[x],b->maxAABBPoint[x],a->minAABBPoint[x],a->maxAABBPoint[x])
		&&P2Math::IsWithinInterval(b->minAABBPoint[y],b->maxAABBPoint[y],a->minAABBPoint[y],a->maxAABBPoint[y])
		&&P2Math::IsWithinInterval(b->minAABBPoint[z],b->maxAABBPoint[z],a->minAABBPoint[z],a->maxAABBPoint[z]);
				
}

bool P2Math::TestAABBAABBCollision(Vect& minA,Vect& maxA,Vect& minB,Vect& maxB){
	return P2Math::IsWithinInterval(minB[x],maxB[x],minA[x],maxA[x])
		&&P2Math::IsWithinInterval(minB[y],maxB[y],minA[y],maxA[y])
		&&P2Math::IsWithinInterval(minB[z],maxB[z],minA[z],maxA[z]);
}

bool P2Math::TestAABBOBBCollision(AABBBoundingBox * a ,OOBB* o){
		//face normals for AABB
		Vect a1 = o->myOwner->colWorld.get(MatrixRowType::ROW_0);//a1
		Vect a2 = o->myOwner->colWorld.get(ROW_1);//a2
		Vect a3 = o->myOwner->colWorld.get(ROW_2);//a3

		a->maxAABBPoint;
		a->minAABBPoint;

		//face normals for OOBB (other)

		Vect b1 = a->ownerWorld.get(MatrixRowType::ROW_0); //b1
		Vect b2 = a->ownerWorld.get(MatrixRowType::ROW_1);//b2
		Vect b3 = a->ownerWorld.get(MatrixRowType::ROW_2);//b3
		
		//Vect b3 = a->myOwner->colWorld.get(MatrixRowType::ROW_2);//b3
		return P2Math::IsWithinSeperatedAxisInterval(a1,a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2,a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a3,a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(b1,a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(b2,a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(b3,a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a1.cross(b1),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a1.cross(b2),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a1.cross(b3),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2.cross(b1),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2.cross(b2),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2.cross(b3),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a3.cross(b1),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a3.cross(b2),a->cornersOfAABB,8,o->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a3.cross(b3),a->cornersOfAABB,8,o->cornersOfOOBB,8);
	}

bool P2Math::TestOBBOBBCollision(OOBB* a, OOBB* b){
		
		//face normals for AABB
		Vect a1 = a->myOwner->colWorld.get(MatrixRowType::ROW_0);//a1
		Vect a2 = a->myOwner->colWorld.get(ROW_1);//a2
		Vect a3 = a->myOwner->colWorld.get(ROW_2);//a3

		//face normals for OOBB (other)
		Vect b1 = b->myOwner->colWorld.get(MatrixRowType::ROW_0); //b1
		Vect b2 = b->myOwner->colWorld.get(MatrixRowType::ROW_1);//b2
		Vect b3 = b->myOwner->colWorld.get(MatrixRowType::ROW_2);//b3

		return P2Math::IsWithinSeperatedAxisInterval(a1,b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2,b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&& P2Math::IsWithinSeperatedAxisInterval(a3,b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(b1,b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(b2,b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(b3,b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a1.cross(b1),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a1.cross(b2),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a1.cross(b3),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2.cross(b1),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2.cross(b2),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a2.cross(b3),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a3.cross(b1),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a3.cross(b2),b->cornersOfOOBB,8,a->cornersOfOOBB,8)
			&&P2Math::IsWithinSeperatedAxisInterval(a3.cross(b3),b->cornersOfOOBB,8,a->cornersOfOOBB,8);
	}

bool P2Math::TestSphereOBBCollision(BoundingSphereVolume* s,OOBB* o ){

		Vect sphereCenterOOBBLoc = s->centerPosition * o->myOwner->colWorld.getInv();

		Vect sphereClampedOOBBLoc;
	
		sphereClampedOOBBLoc[x] = P2Math::Clamp(sphereCenterOOBBLoc[x],o->minOOBB[x],o->maxOOBB[x]);
		sphereClampedOOBBLoc[y] = P2Math::Clamp(sphereCenterOOBBLoc[y],o->minOOBB[y],o->maxOOBB[y]);
		sphereClampedOOBBLoc[z] = P2Math::Clamp(sphereCenterOOBBLoc[z],o->minOOBB[z],o->maxOOBB[z]);

		//move clamped to oobb world coordinate
		sphereClampedOOBBLoc *= o->myOwner->colWorld; 
	
		//test if q2 is withing sphere
		if((sphereClampedOOBBLoc-s->centerPosition).magSqr() < s->radius* s->radius){
			return true;

		}
		return false;
	}

bool P2Math::TestSphereSphereCollision(BoundingSphereVolume* a, BoundingSphereVolume* b){
		//Get Distance between transformed centers
		float dist = (a->centerPosition  - b->centerPosition).mag();
		
		float radiusAdded =a->radius + b->radius;
		//check if distance between is more than the radius added
 		if(dist < radiusAdded ){
			return true; //collision
		}
		return false; //no collision

	}

bool P2Math::TestSphereSphereCollision(Vect& centerA, float radiusA, Vect& centerB, float radiusB){
	//Get Distance between transformed centers
	float dist = (centerA - centerB).mag();
		
	float radiusAdded =radiusA + radiusB;
	//check if distance between is more than the radius added
 	if(dist < radiusAdded ){
		return true; //collision
	}
	return false; //no collision

}


bool P2Math::TestAABBSphereCollision(AABBBoundingBox* a, BoundingSphereVolume* s){
		Vect clampedSphere;

		clampedSphere[x] = P2Math::Clamp(s->centerPosition[x],a->minAABBPoint[x],a->maxAABBPoint[x]);
		clampedSphere[y] = P2Math::Clamp(s->centerPosition[y],a->minAABBPoint[y],a->maxAABBPoint[y]);
		clampedSphere[z] = P2Math::Clamp(s->centerPosition[z],a->minAABBPoint[z],a->maxAABBPoint[z]);

		//check if sphere is inside
		if(clampedSphere[x] > a->minAABBPoint[x]
		&& clampedSphere[x] < a->maxAABBPoint[x]
		&& clampedSphere[y] > a->minAABBPoint[y]
		&& clampedSphere[y] < a->maxAABBPoint[y]
		&& clampedSphere[z] > a->minAABBPoint[z]
		&& clampedSphere[z] < a->maxAABBPoint[z]){
			return true;
		}else if ((s->centerPosition-clampedSphere).magSqr() < s->radius*s->radius){//check if distance is less than radius
  			return true;
		}
			return false;
	}

bool P2Math::TestAABBSphereCollision(Vect& minAABB,Vect& maxAABB, Vect& sphereCenter,float sphereRadius){
		Vect clampedSphere;

		clampedSphere[x] = P2Math::Clamp(sphereCenter[x],minAABB[x],maxAABB[x]);
		clampedSphere[y] = P2Math::Clamp(sphereCenter[y],minAABB[y],maxAABB[y]);
		clampedSphere[z] = P2Math::Clamp(sphereCenter[z],minAABB[z],maxAABB[z]);

		//check if sphere is inside
		if(clampedSphere[x] > minAABB[x]
		&& clampedSphere[x] < maxAABB[x]
		&& clampedSphere[y] > minAABB[y]
		&& clampedSphere[y] < maxAABB[y]
		&& clampedSphere[z] > minAABB[z]
		&& clampedSphere[z] < maxAABB[z]){
			return true;
		}else if ((sphereCenter-clampedSphere).magSqr() < sphereRadius*sphereRadius){//check if distance is less than radius
  			return true;
		}
			return false;
	}

bool P2Math::IsWithinSeperatedAxisInterval(const Vect& axis,const Vect aPoints[],const int aPointsSize,const Vect bPoints[],const int bPointsSize){
		
		float aMinValue;
		float aMaxValue;
		float bMinValue;
		float bMaxValue;

		//set initial values
		aMinValue = aPoints[0].dot(axis);
		aMaxValue = axis.dot(aPoints[0]);
		bMinValue = axis.dot(bPoints[0]);
		bMaxValue = axis.dot(bPoints[0]);

		//find min and max for A's set of points
		for(int i =1; i<aPointsSize;++i){
			//project point
			float projectedValue = aPoints[i].dot(axis);

			//check if less than current a min
			aMinValue = Min(aMinValue,projectedValue);
			//check is greater than current a max
			aMaxValue = Max(aMaxValue, projectedValue);

		}

		//find min and max for B's set of points
		for(int i =1; i<bPointsSize;++i){
			//project point
			float projectedValue = bPoints[i].dot(axis);

			//check if less than current a min
			bMinValue = Min(bMinValue,projectedValue);
			//check is greater than current a max
			bMaxValue = Max(bMaxValue, projectedValue);

		}
		return IsWithinInterval(aMinValue,aMaxValue,bMinValue,bMaxValue);

	}

Matrix P2Math::LeftHandRotX(float amount){
	Matrix lhrx;

	float cosAmount = cos(amount);
	float sinAmount = sin(amount);
	lhrx.set(ROW_0, Vect(1,0,0,0));
	lhrx.set(ROW_1, Vect(0,cosAmount,-sinAmount,0));
	lhrx.set(ROW_2, Vect(0,sinAmount,cosAmount,0));
	lhrx.set(ROW_1, Vect(0,0,0,1));
	return lhrx;
}

Matrix P2Math::LeftHandRotY(float amount){
	Matrix lhrx;
	float cosAmount = cos(amount);
	float sinAmount = sin(amount);

	lhrx.set(ROW_0, Vect(cosAmount,0,sinAmount,0));
	lhrx.set(ROW_1, Vect(0,1,0,0));
	lhrx.set(ROW_2, Vect(-sinAmount,0,cosAmount,0));
	lhrx.set(ROW_1, Vect(0,0,0,1));
	return lhrx;
}

Matrix P2Math::LeftHandRotZ(float amount){
	Matrix lhrx;
	
	float cosAmount = cos(amount);
	float sinAmount = sin(amount);

	lhrx.set(ROW_0, Vect(cosAmount,-sinAmount,0,0));
	lhrx.set(ROW_1, Vect(sinAmount,cosAmount,0,0));
	lhrx.set(ROW_2, Vect(0,0,1,0));
	lhrx.set(ROW_1, Vect(0,0,0,1));
	return lhrx;
}