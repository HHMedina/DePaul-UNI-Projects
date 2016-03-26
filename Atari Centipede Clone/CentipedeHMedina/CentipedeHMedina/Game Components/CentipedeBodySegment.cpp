#include "CentipedeBodySegment.h"
#include "BlasterBullet.h"
#include "Mushroomfield.h"
#include "RecyclingCHeadSegmentFactory.h"
#include "RecyclingCBodySegmentFactory.h"
#include "CentipedeHeadSegment.h"
#include "PlayerManager.h"
#include "CentipedeFactory.h"
#include "Blaster.h"
#include "RecyclingEnemyExplosionFactory.h"
#include "EnemyExplosion.h"

CentipedeBodySegment::CentipedeBodySegment(sf::Vector2f worldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection){

	Speed=speed;

	Field=&FatherField;
	WorldPosition = worldPosition;

	//animateion set up
	Sprite = AnimatedSprite(ResourceManager::GetTexture("CentipedeBody"), 8, 2);
	Sprite.scale(2,2);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(WorldPosition);
	Sprite.SetAnimation(0,7,true,true);

	//physics set up
	bitmap = ResourceManager::GetTextureBitmap("CentipedeBody");
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<CentipedeBodySegment>(*this);
	SetDrawOrder(2000);

	//instantiate with null pointers
	Next=NULL;
	Previous=NULL;
	
	//set direction to initial direction
	Direction = InitialDirection;

	//if the initial diection is positive then face right initially
	if(InitialDirection.x==1){
		Sprite.setRotation(FACINGRIGHT);
	}else if(InitialDirection.x==-1){
		Sprite.setRotation(FACINGLEFT);
	}

	//start all body segments with initial current stack (Initial movement)
	for(int i =0;i<MUSHROOMSEGMENT/Speed;i++){
		CurrentQueue.push(PositionAndRotationOffset(sf::Vector2f(Speed,0),0,Direction,false));
	}
}

void CentipedeBodySegment::Initialize(const sf::Vector2f worldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection){
	Speed = speed;
	Field=&FatherField;
	RegisterCollision<CentipedeBodySegment>(*this);
	Next=NULL;
	Previous=NULL;
	WorldPosition =worldPosition;
	Direction = InitialDirection;

	//if the initial diection is positive then face right initially if negative face left
	if(InitialDirection.x==1){
		Sprite.setRotation(FACINGRIGHT);
	}else if(InitialDirection.x==-1){
		Sprite.setRotation(FACINGLEFT);
	}

	while(!CurrentQueue.empty()){
		CurrentQueue.pop();
	}

	while(!FutureQueue.empty()){
		FutureQueue.pop();
	}

	//start all body segments with initial current stack (Initial movement)
	for(int i =0;i<MUSHROOMSEGMENT/Speed;i++){
		CurrentQueue.push(PositionAndRotationOffset(sf::Vector2f(Speed,0),0,Direction,false));
	}

	Sprite.setPosition(WorldPosition);
}

void CentipedeBodySegment::Update(){
	Sprite.Update();
	

	if(CurrentQueue.empty()){

		
		//if current is empty then work on the future queue.
		CurrentQueue=FutureQueue;
		//empties the future queue
		while(!FutureQueue.empty()){
			FutureQueue.pop();

		}
		
	}



	//if not empty then work on the queue
	if(!CurrentQueue.empty()){

		Direction = CurrentQueue.front().Direction;//set body direction to the previous segments previous direction
		
		//process movement from offset.
		WorldPosition += sf::Vector2f(CurrentQueue.front().PositionOffset.x*CurrentQueue.front().Direction.x,CurrentQueue.front().PositionOffset.y*CurrentQueue.front().Direction.y);
		Sprite.setPosition(WorldPosition);
		Sprite.setRotation(Sprite.getRotation()+CurrentQueue.front().RotationOffset*CurrentQueue.front().Direction.x);

		if(Next!=NULL){//if there is a body after this one push the processed offset to the one after this one
			Next->FutureQueue.push(PositionAndRotationOffset(CurrentQueue.front().PositionOffset,CurrentQueue.front().RotationOffset,Direction,CurrentQueue.front().IsInATurn));
		}
		
		CurrentQueue.pop(); //removes processed offset
		

	}
	
	
}

void CentipedeBodySegment::Draw(){
	WindowManager::MainWindow.draw(Sprite);
}

void CentipedeBodySegment::Destroy(){
	
	CentipedeFactory::RemoveCentipedeSegmentFromAliveList(*this);


	DeregisterCollision<CentipedeBodySegment>(*this);
	
	//empty the queues.
	while(!FutureQueue.empty()){
			FutureQueue.pop();

	}
	while(!CurrentQueue.empty()){
			CurrentQueue.pop();

	}
}


void CentipedeBodySegment::Collision( BlasterBullet *other){
	

	//adds score to current score
	PlayerManager::AddToCurrentScore(ScoreValue);

	//spawns death explosion
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	//creates a mushroom in the area the centipede is in if there is not already
	//a mushroom in that spot.
	if(!Field->CheckGridSpace(Field->WorldPositionToMushroomGrid(WorldPosition+sf::Vector2f(MUSHROOMSEGMENT*Direction.x,0))))
	{
		Field->CreateMushroomAtCoordinate(Field->WorldPositionToMushroomGrid(WorldPosition+sf::Vector2f(MUSHROOMSEGMENT*Direction.x,0)));
	}

	if(Next!=NULL){
	//create centipede at previous location(this worldlocation - one cell length)
		CentipedeHeadSegment* cHead = &RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(Next->WorldPosition,Speed,*Field,Next->Direction );
		CentipedeSegment* it =cHead;
		cHead->Sprite.setRotation(Next->Sprite.getRotation());
		if(!CurrentQueue.empty()&&Next->CurrentQueue.front().IsInATurn){
			cHead->IsTurning =true;
		}
		if(!CurrentQueue.empty()){
			cHead->CurrentQueue = Next->CurrentQueue;
		}
		

		//If there is a second body after the head ( 3 or more body segments present) 
		if(Next->Next!=NULL){
			//set the new centipede's next to the 2nd body behind head (since head replaces the 1st body behind it) 
			Next->Next->Previous = cHead;
	
			//set the new centipedes next to the 2nd body behind the head at collision
			cHead->Next = Next->Next;
		}
		Next->MarkForDestroy();//destroys the 1st body after the head (the one head will replace) Pointers set to NULL (on destroy())


		//this block of code reconfigured the update order of the new centipede collection caused by the split
		int NumberOfSegments=0;
		
		while(it!=NULL){
			++NumberOfSegments;
			it=it->Next;
		}

		it=cHead;
		int UpdateCounter= 0;
		while(it!=NULL){
			it->SetUpdateOrder(NumberOfSegments-UpdateCounter);
			it= it->Next;
			++UpdateCounter;

		}


	}
	
	//set this segment's next and previous to null
	this->Previous->Next = NULL;
	if(Next!=NULL){
		this->Next->Previous =NULL;
	}
	this->Next=NULL;
	this->Previous=NULL;

	this->MarkForDestroy();//destroy the head that was collided with.


}

void CentipedeBodySegment::Collision(Blaster* other){

	//create an Enemy explosion object on bulletCollision
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	this->MarkForDestroy();
}