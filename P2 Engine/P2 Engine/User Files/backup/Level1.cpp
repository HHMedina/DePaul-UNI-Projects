#include "Level1.h"
#include "GameObject.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include "Ship.h"
#include "WeirdShape.h"
#include "BattleShip.h"

//#include "CollidableGroup.h"

void Level1::SetUp(){

	//Ship* test= new Ship();
	//test;

	Ship* test2= new Ship(0);
	test2;
	WeirdShape* test3 =new WeirdShape();
	test3;

	//BattleShip* test4 =new BattleShip();
	//test4;


	//delete test4;
	
	SetCollisionPair<Ship,WeirdShape>();
	//SceneManager::SetCollisionPair<Ship,BattleShip>();
}

void Level1::OnSceneEnd(){

	///delete test2;
	//delete test3;
}
