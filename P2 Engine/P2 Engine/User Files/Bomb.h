#ifndef BOMB_H
#define BOMB_H

#include "P2Engine.h"
using namespace std;

class Bucket;
class Bomb:public GameObject{

public:
	
	Bomb(float,float,float);
	Bomb( const Bomb & rhs){ rhs;}
	Bomb& operator = (const Bomb& rhs){ rhs;}
	virtual ~Bomb();
	
	void Initialize(float,float,float);

	virtual void Draw();
	
	//Alarms
	virtual void Alarm0();

	virtual void Update(); 

	virtual void Collision(Collidable*){}
	virtual void Collision(Bucket*);
	virtual void OnTerminate();
	
	Matrix bombScale;
	Vect bombPos;
	Matrix bombRot;
	

};




#endif