#ifndef WEIRDSHAPE_H
#define WEIRDSHAPE_H

#include "P2Engine.h"
class Ship;
using namespace std;

class WeirdShape:public GameObject{

public:

	WeirdShape();
	WeirdShape( const WeirdShape & rhs){ rhs;}
	WeirdShape& operator = (const WeirdShape& rhs){ rhs;}

	
	virtual void KeyDown(AZUL_KEY key);
	virtual void KeyUp(AZUL_KEY key);

	virtual void Draw();
	virtual void Update();

	//Alarms
	virtual void Alarm0();

	virtual void Collision(Collidable*){cout<<"Weird Shape collision to Collidable"<<endl;}
	virtual void Collision(Ship*){cout<<"Weird Shape collision to Ship"<<endl;}

	virtual void OnTerminate();

};




#endif