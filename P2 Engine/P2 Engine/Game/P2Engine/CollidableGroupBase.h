#ifndef COLLIDABLEGROUPBASE_H
#define COLLIDABLEGROUPBASE_H

class CollidableGroupBase{

	friend class CollisionManager;
	template<class C1,class C2>
	friend class CollisionPairProcessor;

	template<class C1>
	friend class CollisionSingleProcessor;
	template <class T>
	friend class CollisionTerrainProcessor;

	virtual int GetGroupSize()=0;

protected:

	virtual ~CollidableGroupBase(){}
	CollidableGroupBase(){}
	CollidableGroupBase( const CollidableGroupBase & ){}
	CollidableGroupBase& operator = (const CollidableGroupBase& ){ }
	
	
	virtual void UpdateGroupAABB() = 0;
	Vect groupAABBMin;
	Vect groupAABBMax;
	
};
#endif