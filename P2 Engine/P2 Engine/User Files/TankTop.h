#ifndef TANKTOP_H
#define TANKTOP_H

//#define TANKTOPDEBUG

#include "P2Engine.h"
using namespace std;

class Tank;
class TankShell;
class TankTop:public GameObject{

public:
	
	TankTop(Vect& position,Vect& scale,Vect& rotation,Tank* owner);
	TankTop( const TankTop & rhs){ rhs;}
	TankTop& operator = (const TankTop& rhs){ rhs;}
	virtual ~TankTop();
	
	void Initialize(float,float,float);

	virtual void Update();
	virtual void Draw();
	

	virtual void Collision(Collidable*){}
	virtual void Collision(TankShell*);
	virtual void OnTerminate();
	
	Matrix tankTopScale;
	Vect tankTopPos;
	Matrix tankTopRot;
	
	Tank* myOwner;

	bool hit;
#ifdef TANKTOPDEBUG
	GraphicsObjectWireFrame* debugWireFrame;
#endif TANKTOPDEBUG
};

#endif