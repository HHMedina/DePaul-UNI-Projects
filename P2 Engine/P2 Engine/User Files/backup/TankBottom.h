#ifndef TANKBOTTOM_H
#define TANKBOTTOM_H

#include "P2Engine.h"
using namespace std;

class Tank;
class TankBottom:public GameObject{

public:
	
	TankBottom(Vect& position,Vect& scale,Vect& rotation,Tank* owner);
	TankBottom( const TankBottom & rhs){ rhs;}
	TankBottom& operator = (const TankBottom& rhs){ rhs;}
	virtual ~TankBottom();
	
	void Initialize(float,float,float);

	virtual void Draw();
	
	virtual void Collision(Collidable*){}
	virtual void OnTerminate();
	
	Matrix tankBottomScale;
	Vect tankBottomPos;
	Matrix tankBottomRot;
	
	float speed;
	float turningSpeed;

	Vect CamOffset;

	Tank* myOwner;
};

#endif
