#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include "P2Engine.h"
#include <iostream>


using namespace std;
class Ship;
class BattleShip:public GameObject{

public:

	BattleShip();
	BattleShip( const BattleShip & rhs){ rhs;}
	BattleShip& operator = (const BattleShip& rhs){ rhs;}

	
	virtual void KeyDown(AZUL_KEY ){}
	virtual void KeyUp(AZUL_KEY){}

	virtual void Draw(){}
	virtual void Update(){}

	//Alarms
	virtual void Alarm0(){}

	virtual void Collision(Collidable*){cout<<"BattleShip Collision with Collidable"<<endl; }
	virtual void Collision(Ship*){cout<<"BattleShip Collision with Ship"<<endl;}
};




#endif