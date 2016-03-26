
#ifndef TANKSHELL_H
#define TANKSHELL_H

#include "P2Engine.h"
using namespace std;

#define TANKSHELLDEBUG
class TankTop;
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
	virtual void Collision(TankTop*){
		hit=true;

	}
	virtual void Collision(TankShell*){
		hit=true;
	}
	virtual void OnTerminate();
	virtual void Alarm0(){
		MarkForDestroy();
	}
	Matrix tankShellScale;
	Vect tankShellPos;
	Matrix tankShellRot;
	
	float speed;

	Sound3D * tankShot;
	bool hit;

};

#endif
