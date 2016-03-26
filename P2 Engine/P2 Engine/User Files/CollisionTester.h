#ifndef COLLISONTESTER_H
#define COLLISONTESTER_H

#include "P2Engine.h"
class CollisionTester: public GameObject{

public:
	CollisionTester(Vect& position, Vect& Scale, Vect& rotation, float speed =1.0f);
	CollisionTester( const CollisionTester & rhs){ rhs;}
	CollisionTester& operator = (const CollisionTester& rhs){ rhs;}
	virtual ~CollisionTester();

	virtual void Draw();
	
	
	virtual void Update(); 

	virtual void Collision(Collidable*){}
	//virtual void Collision(TankTop*){
	//	hit=true;

	//}
	virtual void Collision(CollisionTester*){
		hit=true;
	}
	Vect pos;
	Vect rot;
	Vect scale;

	float speed;

	bool hit;
};


#endif