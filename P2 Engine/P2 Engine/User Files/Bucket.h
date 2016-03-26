#ifndef BUCKET_H
#define BUCKET_H

#include "P2Engine.h"
using namespace std;

class Bomb;
class Bucket:public GameObject{

public:
	
	Bucket(float,float,float);
	Bucket( const Bucket & rhs){ rhs;}
	Bucket& operator = (const Bucket& rhs){ rhs;}
	virtual ~Bucket();
	
	virtual void Draw();
	virtual void Update();
	virtual void KeyDown(AZUL_KEY);

	virtual void Collision(Collidable*){}
	virtual void TerrainCollision();
	virtual void Collision(Bomb*);
	virtual void OnTerminate();
	
	Matrix bucketScale;
	Vect bucketPos;
	Matrix bucketRot;
	static const float bucketMovementSpeed;
	static const float bucketTurningSpeed;
	

};




#endif