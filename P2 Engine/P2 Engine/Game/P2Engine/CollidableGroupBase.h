#ifndef COLLIDABLEGROUPBASE_H
#define COLLIDABLEGROUPBASE_H

class CollidableGroupBase{

	friend class CollisionManager;
	template<class C1,class C2>
	friend class CollisionPairProcessor;

	template<class C1>
	friend class CollisionSingleProcessor;



protected:

	virtual ~CollidableGroupBase(){}
	CollidableGroupBase(){}
	CollidableGroupBase( const CollidableGroupBase & ){}
	CollidableGroupBase& operator = (const CollidableGroupBase& ){ }
	
};
#endif