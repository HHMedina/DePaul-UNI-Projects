#ifndef COLLISIONPROCESSORBASE_H
#define COLLISIONPROCESSORBASE_H

class CollisionProcessorBase{

	friend class CollisionManager;
	
protected:

	CollisionProcessorBase(){}
	virtual ~CollisionProcessorBase(){}

	virtual void CollisionTest() = 0;
	
};
#endif