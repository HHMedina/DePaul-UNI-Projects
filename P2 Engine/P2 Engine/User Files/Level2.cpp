#include "Level2.h"
#include "GameObject.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "Ship.h"
#include "WeirdShape.h"
#include "BattleShip.h"

//#include "CollidableGroup.h"

void Level2::SetUp(){

	Ship* test= new Ship();
	test;

	Ship* test2= new Ship(0);
	test2;

	WeirdShape* test3 =new WeirdShape();
	test3;
	//BattleShip* test4 =new BattleShip();
	//test4;


	//delete test4;
	
	//SceneManager::SetCollisionPair<Ship,WeirdShape>();
	//SceneManager::SetCollisionPair<Ship,BattleShip>();
	
	SetCollisionSelf<Ship>();
}

