#include "TutorialLevel1.h"
#include "Bucket.h"
#include "Floor.h"
#include "Bomb.h"
#include "RecyclingBombFactory.h"

void TutorialLevel1::SetUp(){

	floor=new Floor();

	RecyclingBombFactory::CreateBomb(0,0,0);
	bucket=new Bucket(0,0,90);

	SetCollisionPair<Bomb,Bucket>();
}

void TutorialLevel1::OnSceneEnd(){

	//mark gameobject for destroy for explicit memory clean up
	floor->MarkForDestroy();
	bucket->MarkForDestroy();

}