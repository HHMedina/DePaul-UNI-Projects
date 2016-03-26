#ifndef COLLISONTESTERMOVABLE_H
#define COLLISONTESTERMOVABLE_H
#include "CollisionTester.h"
#include "P2Engine.h"
class CollisionTesterMovable: public GameObject{
	
public:
	CollisionTesterMovable(Vect& position, Vect& Scale, Vect& rotation, float speed =1.0f);
	CollisionTesterMovable( const CollisionTesterMovable & rhs){ rhs;}
	CollisionTesterMovable& operator = (const CollisionTesterMovable& rhs){ rhs;}
	virtual ~CollisionTesterMovable();

	virtual void Draw();
	
	
	virtual void Update(); 

	virtual void Collision(Collidable*){}
	//virtual void Collision(TankTop*){
	//	hit=true;

	//}
	virtual void Collision(CollisionTesterMovable*){
		hit=true;
	}

	virtual void Collision(CollisionTester*){
		hit=true;
	}

	
	virtual void TerrainCollision(){
		hit=true;
	}

	Vect pos;
	Vect rot;
	Vect scale;
	Vect CamOffset;

	float speed;
	float rotSpeed;
	bool hit;
};


#endif