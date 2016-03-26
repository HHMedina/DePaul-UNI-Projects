#ifndef SHIP_H
#define SHIP_H

#include "P2Engine.h"
using namespace std;
class WeirdShape; 
class BattleShip;
class Ship:public GameObject{

public:

	Ship();

	Ship(int);
	Ship( const Ship & rhs){ rhs;}
	Ship& operator = (const Ship& rhs){ rhs;}
	virtual ~Ship();
	
	virtual void KeyDown(AZUL_KEY key);
	virtual void KeyUp(AZUL_KEY key);

	virtual void Draw();
	virtual void Update();
	
	//Alarms
	virtual void Alarm0();
	virtual void Alarm1();

	virtual void Collision(Collidable*){cout<<"Ship collision with Collidable"<<endl;}
	virtual void Collision(WeirdShape*);
	virtual void Collision(BattleShip*){cout<<"Ship Collision with BattleShip"<<endl;}
	virtual void Collision(Ship*);

	virtual void OnTerminate();

	GraphicsObjectWireFrame* s;

	Matrix world;
	Matrix ShipScale;
	Vect ShipPos;
	Matrix ShipRot;
	

};




#endif