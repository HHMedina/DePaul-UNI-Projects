#ifndef BOUNDINGSPHEREVOLUME_H
#define BOUNDINGSPHEREVOLUME_H
#include "CollisionVolume.h"
#include "Collidable.h"


#ifdef Debug
#include "GraphicsObjectMaker.h"
#endif

class BoundingSphereVolume;
class AABBBoundingBox;
class CollisionVisitor;

class BoundingSphereVolume : public CollisionVolume{

	friend class Collidable;
		
	friend class P2Math;
public:

	virtual const Matrix GetRenderMatrix();

	BoundingSphereVolume(Collidable* d);
	~BoundingSphereVolume(){}

	virtual void DrawColVolume(Vect& color);
	float GetRadius(){
		return radius;
	}

	Vect GetCenter(){
		return centerPosition;
	}
protected:
	virtual bool AcceptCollisionTest(CollisionVolume*);

	//virtual bool CollisionTest(CollisionVolume* visitor){return false;};
	virtual bool CollisionTest(BoundingSphereVolume*);
	virtual bool CollisionTest(AABBBoundingBox*);
	virtual bool CollisionTest(OOBB*);

	virtual void UpdateValues();

	Vect centerPosition;
	float radius;
};




#endif