#include "DeathMatchLevel.h"
#include "DesertFloor.h"
#include "TankShell.h"
#include "TankBottom.h"
#include "TankTop.h"
#include "Tank.h"

void DeathMatchLevel::SetUp(){

	desertFloor=new DesertFloor();
	//new TankShell(Vect(0,0,0),Vect(0,-1.57079633,0) );
	new Tank(Vect(0,0,0),Vect(1,1,1),Vect(0,0,0),0);
	new Tank(Vect(100,0,0),Vect(1,1,1),Vect(0,90,0),1);
	//new TankBottom(Vect(0,0,0),Vect(0,0,0));
	//new TankTop(Vect(0,24,0),Vect(0,0,0));
	//SetCollisionPair<TankBottom,TankShell>();
	//SetCollisionPair<TankTop,TankShell>();
	SetCollisionSelf<TankShell>();
}

void DeathMatchLevel::OnSceneEnd(){

	//mark gameobject for destroy for explicit memory clean up
	desertFloor->MarkForDestroy();

}