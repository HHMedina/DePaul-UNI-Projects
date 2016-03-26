#ifndef COLLISIONVOLUME_H
#define COLLISIONVOLUME_H
#include "Azul.h"
#include <assert.h>

//#define Debug

class Collidable;
class BoundingSphereVolume;
class AABBBoundingBox;
class OOBB;

enum CollisionVolumeType{
	
	SphereVolume,
	AABBBoxVolume,
	OOBBVolume


};

class CollisionVolume //: CollisionVisitor
{
	friend class Collidable;
	friend class GameObject;

	friend class OOBB;
	friend class BoundingSphereVolume;
	friend class AABBBoundingBox;

public:
	CollisionVolume(){}
	const Matrix& GetScaleMatrix(){
		return scaleM;
	}
	const Matrix& GetRotationMatrix(){
		return rotationM;
	}
	const Matrix& GetTranslationMatrix(){
		return translationM;
	}
	virtual const Matrix GetRenderMatrix(){
		
		Matrix temp;
		temp.set(IDENTITY);
		return temp;
	}

	Vect& GetCheapMaxAABBPoint(){
		return cheapMaxAABBPoint;
	}

	Vect& GetCheapMinAABBPoint(){
		return cheapMinAABBPoint;
	}

	float& GetCheapSphereRadius(){
		return cheapSphereRadius;
	}

	Vect& GetCheapCenter(){
		return cheapCenter;
	}

	virtual void DrawColVolume(Vect& color)=0; 
protected:
	virtual bool AcceptCollisionTest(CollisionVolume*)=0;

	virtual bool CollisionTest(CollisionVolume*){ assert(false&&"CollisionVolume:Visit(CollisionVolume) Called!"); return false;};
	virtual bool CollisionTest(BoundingSphereVolume*)=0;
	virtual bool CollisionTest(AABBBoundingBox*)=0;
	virtual bool CollisionTest(OOBB*) =0;

	virtual ~CollisionVolume(){}
	virtual void UpdateValues()=0;
	
	Collidable* myOwner;

	Matrix scaleM;
	Matrix rotationM;
	Matrix translationM;


	//used for cheapTesting
	Vect cheapCenter;

	//sphere used for generating cheap AABB and cheap testing
	float cheapSphereRadius;

	//AABB used for cheaptesting
	Vect cheapMinAABBPoint;
	Vect cheapMaxAABBPoint;
};
#endif