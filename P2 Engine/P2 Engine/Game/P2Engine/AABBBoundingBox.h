#ifndef AABBBOUNDINGBOX_H
#define AABBBOUNDINGBOX_H
#include "CollisionVolume.h"

#ifdef Debug
#include"GraphicsObjectMaker.h"
#endif
class BoundingSphereVolume;
class AABBBoundingBox: public CollisionVolume{
	friend class Collidable;
	
	friend class P2Math;
	friend class BoundingSphereVolume;
	friend class OOBB;
	template<class t>
	friend class CollisionTerrainProcessor;

public:
	virtual const Matrix GetRenderMatrix();

	AABBBoundingBox(Collidable* d){
		myOwner =d;
	}
	AABBBoundingBox(Vect& minPoint,Vect&maxPoint,Matrix& world){
		world;
		minAABBPoint=minPoint;
		maxAABBPoint=maxPoint;

		Vect scale = maxAABBPoint-minAABBPoint;
		Vect pos = (maxAABBPoint+minAABBPoint) *.5f;
		
		Matrix matScale = Matrix(SCALE,scale[x],scale[y],scale[z]);
		Matrix matPos = Matrix (TRANS, pos[x],pos[y],pos[z]);
		Matrix rotM = Matrix();
		rotM.set(ROT_XYZ,0,0,0);
		ownerWorld = matScale *rotM *matPos;

		UpdateCorners();

	}
	~AABBBoundingBox(){}

	virtual void DrawColVolume(Vect& color);

	Vect& GetMinAABBPoint();
	Vect& GetMaxAABBPoint();

protected:
	virtual bool AcceptCollisionTest(CollisionVolume*);

	virtual bool CollisionTest(CollisionVolume*){return false;};
	virtual bool CollisionTest(BoundingSphereVolume*);
	virtual bool CollisionTest(AABBBoundingBox*);
	virtual bool CollisionTest(OOBB*);

	virtual void UpdateValues();
	
	void UpdateCorners();
	Vect minAABBPoint;
	Vect maxAABBPoint;
	Vect Center;

	Matrix ownerWorld;

	Vect cornersOfAABB[8]; 

};
#endif
