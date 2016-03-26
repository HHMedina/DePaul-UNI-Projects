#include "DeathMatchLevel.h"
#include "DesertFloor.h"
//#include "TankShell.h"
//#include "TankBottom.h"
//#include "TankTop.h"
#include "Tank.h"
//#include "BGM.h"
//#include "Bucket.h"
#include "CollisionTester.h"

#include "CollisionTesterMovable.h"
void DeathMatchLevel::SetUp(){

	//new CameraObjectFollow();


	new CollisionTester(Vect(-10,0,0),Vect(1,1,1),Vect(0,P2Math::DegToRad(90),0),0.1f);
	new CollisionTester(Vect(10,0,0),Vect(1,1,1),Vect(0,P2Math::DegToRad(-90),0),0.1f);
	new CollisionTesterMovable(Vect(0,1.0f,0),Vect(.05f,.05f,.05f),Vect(0,0,0),.3f);
	//new CollisionTesterMovable(Vect(4,.50f,4),Vect(.05f,.05f,.05f),Vect(0,0,0),.3f);

	SetCollisionPair<CollisionTester,CollisionTesterMovable>();
	SetCollisionSelf<CollisionTester>();
	//collision tester stuff

	//new Bucket(0,0,0);
	//new Bucket(60,0,60);
	//Tank* test = new Tank(Vect(0,0,0),Vect(1,1,1),Vect(0,0,0),0);
	//new TerrainModel("TerTest6.tga",250,20,0,"BombTexture",1,1,new Bucket(0,0,0));
	//desertFloor=new DesertFloor();
	//new TankShell(Vect(0,0,0),Vect(0,-1.57079633,0) );
	//new BackGroundMusicObject();
	//new Tank(Vect(0,0,-10),Vect(.01f,.01f,.01f),Vect(0,0,0),0);

	//new TankBottom(Vect(0,0,0),Vect(.2f,.2f,.2f),Vect(0,0,0));
	//new TankBottom(Vect(-125,0,0),Vect(.2f,.2f,.2f),Vect(0,0,0));
	//new TankBottom(Vect(125,0,0),Vect(.2f,.2f,.2f),Vect(0,0,0));
	//new TankBottom(Vect(0,0,-125),Vect(.2f,.2f,.2f),Vect(0,0,0));
	//new TankBottom(Vect(0,0,125),Vect(.2f,.2f,.2f),Vect(0,0,0));
	//new TankTop(Vect(0,0,0),Vect(0,0,0));
	//SetCollisionPair<TankBottom,TankShell>();
	//SetCollisionPair<TankTop,TankShell>();
	//SetCollisionSelf<TankShell>();
	//AssetManager::LoadTerrain("TerTest6.tga","TerTestTerrain","BombTexture",250,20,0,1,1);
	//SetCollisionSelf<Bucket>();
	//SceneManager::SetCollisionWithTerrain<Bucket>();

	SetCollisionWithTerrain<CollisionTesterMovable>();
	SetCurrentTerrain("TerTestTerrain");
}

void DeathMatchLevel::OnSceneEnd(){

	//mark gameobject for destroy for explicit memory clean up
	//desertFloor->MarkForDestroy();

}