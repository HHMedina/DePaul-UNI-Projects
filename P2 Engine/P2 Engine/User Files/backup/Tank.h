#ifndef TANK_H
#define TANK_H

#include "P2Engine.h"
using namespace std;

class TankBottom;
class TankTop;

class Tank:public GameObject{

public:
	
	Tank(Vect& position,Vect& scale,Vect& rotation,int mode);
	Tank( const Tank & rhs){ rhs;}
	Tank& operator = (const Tank& rhs){ rhs;}
	virtual ~Tank();
	
	void Initialize(float,float,float);
	
	virtual void KeyDown(AZUL_KEY key);
	virtual void Update(); 

	virtual void Collision(Collidable*){}
	virtual void OnTerminate();
	
	//lower tank
	Matrix tankScale;
	Vect tankPos;
	Matrix tankRot;
	
	float bottomSpeed;
	float bottomTurningSpeed;

	//upper tank rotation -scale is same as main tank
	Matrix tankTopRot;
	
	float topTurningSpeed;

	Vect CamOffset;
	void AdjustHP(int amount);
private:
	Vect topVerticalOffset;
	float shellSpawnOffsetFront;
	Vect topTankPartAdjustedPosition;
	Vect shellSpawnPosition;
	Matrix topTankPartAdjustedRotation;

	TankTop* top;
	TankBottom* bottom;

	void TopMovement();
	void BottomMovement();

	int hp;

	static const int maxHP =3;

	void DestroyParts();

};

#endif