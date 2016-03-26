#ifndef OOBB_H
#define OOBB_H
#include "CollisionVolume.h"

#ifdef Debug
#include"GraphicsObjectMaker.h"
#endif
class BoundingSphereVolume;

class OOBB: public CollisionVolume{
	//friend class Collidable;
	
	friend class P2Math;
public:
	virtual const Matrix GetRenderMatrix();
	//const Vect& GetMinOOBB(){}

	OOBB(Collidable* d);
	~OOBB(){}

	virtual void DrawColVolume(Vect& color);
protected:
	virtual bool AcceptCollisionTest(CollisionVolume*);

	virtual bool CollisionTest(CollisionVolume*){return false;};
	virtual bool CollisionTest(BoundingSphereVolume*);
	virtual bool CollisionTest(AABBBoundingBox*);
	virtual bool CollisionTest(OOBB*);
	
	virtual void UpdateValues();

	void ComputeCorners();
	void UpdateWorldSpaceCorners();

	Vect cornersOfOOBB[8];
	Vect minOOBB;
	Vect maxOOBB;
		
	Vect tempPos;

};
#endif
