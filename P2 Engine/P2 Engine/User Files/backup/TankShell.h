
#ifndef TANKSHELL_H
#define TANKSHELL_H

#include "P2Engine.h"
using namespace std;

class TankShell:public GameObject{

public:
	
	TankShell(Vect& position,Matrix& rotation);
	TankShell( const TankShell & rhs){ rhs;}
	TankShell& operator = (const TankShell& rhs){ rhs;}
	virtual ~TankShell();
	
	void Initialize(float,float,float);

	virtual void Draw();
	
	
	virtual void Update(); 

	virtual void Collision(Collidable*){}
	virtual void OnTerminate();
	
	Matrix tankShellScale;
	Vect tankShellPos;
	Matrix tankShellRot;
	
	float speed;

};

#endif
