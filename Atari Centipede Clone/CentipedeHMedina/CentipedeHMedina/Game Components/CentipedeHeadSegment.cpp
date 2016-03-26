#include "CentipedeHeadSegment.h"
#include "Mushroomfield.h"
#include "RecyclingCBodySegmentFactory.h"
#include "RecyclingCHeadSegmentFactory.h"
#include "Blaster.h"
#include "BlasterBullet.h"
#include "WaveManager.h"
#include "RecyclingEnemyExplosionFactory.h"
#include "EnemyExplosion.h"
#include "SoundManager.h"
#include "CentipedeFactory.h"
#include "PlayerManager.h"
#include "Player.h"
#include "Player1.h"
#include "Player2.h"

int CentipedeHeadSegment::NumberOfInstances=0;

CentipedeHeadSegment::CentipedeHeadSegment(const sf::Vector2f worldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection ){
	
	Speed=speed;

	//this->SetUpdateOrder(9000);
	TagNumber =NumberOfInstances;
	//blasterBorder Definition
	TopBlasterBorder= WindowManager::MainWindow.getView().getSize().y-((7*MUSHROOMSEGMENT)) + MUSHROOMSEGMENT/2;
	BottomBlasterBorder= WindowManager::MainWindow.getView().getSize().y - (MUSHROOMSEGMENT/2)-MUSHROOMSEGMENT;
	//initial bool setup
	HasTouchedBottom =false;
	IsPoisonedState =false;
	//initial world position
	WorldPosition=worldPosition;
	
	//sprite setup 
	Sprite = AnimatedSprite(ResourceManager::GetTexture("CentipedeHead"), 8, 2);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(WorldPosition);
	Sprite.SetAnimation(0,7,true,true);
	Sprite.scale(2,2);
	SetDrawOrder(1000);
	
	//set up the collider
	bitmap = ResourceManager::GetTextureBitmap("CentipedeHead");
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<CentipedeHeadSegment>(*this);

	//on creation set Pointer to point to NULL
	Next = NULL;
	Previous=NULL;

	//set up inital field
	Field=&FatherField;

	//set up direction
	Direction = InitialDirection;
	
	//if the direction
	if(InitialDirection.x==1){
		Sprite.setRotation((float)FACINGRIGHT);
	}
}

void CentipedeHeadSegment::Initialize(const sf::Vector2f worldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection){
	
	Speed=speed;
	
	while(!CurrentQueue.empty()){
		CurrentQueue.pop();
	}

	while(!HorizontalOffsetQueue.empty()){
		HorizontalOffsetQueue.pop();
	}
	while(!TurnOffsetQueue.empty()){
		TurnOffsetQueue.pop();
	}

	if(NumberOfInstances==0){//if this is the only centipede created on a centipede-less screen
		SoundManager::PlayCentipedeSound();//begin the CentipedeLoop.
	}
	++NumberOfInstances;//increment instances of CentipedeHead

	WorldPosition = worldPosition;
	Field = &FatherField;
	RegisterCollision<CentipedeHeadSegment>(*this);
	Next=NULL;
	Previous=NULL;
	Direction = InitialDirection;
	Sprite.setPosition(WorldPosition);
	//initial bool setup
	HasTouchedBottom =false;
	IsPoisonedState =false;
	IsTurning =false;
	

	if(InitialDirection.x==1){
		Sprite.setRotation(FACINGRIGHT);
	}else{
		Sprite.setRotation(FACINGLEFT);
	}


	if((MUSHROOMSEGMENT/Speed)==1){

		TurnOffsetQueue.push(PositionAndRotationOffset(sf::Vector2f(0,(MUSHROOMSEGMENT/(MUSHROOMSEGMENT/Speed))),180.0f/(MUSHROOMSEGMENT/Speed),Direction,true));

	}else{

		for(int i =0;i<MUSHROOMSEGMENT/Speed/2;i++){
			TurnOffsetQueue.push(PositionAndRotationOffset(sf::Vector2f(1,(MUSHROOMSEGMENT/(MUSHROOMSEGMENT/Speed))),180.0f/(MUSHROOMSEGMENT/Speed),Direction,true));
		}

		for(int i =MUSHROOMSEGMENT/Speed/2;i<MUSHROOMSEGMENT/Speed;i++){
			TurnOffsetQueue.push(PositionAndRotationOffset(sf::Vector2f(-1,(MUSHROOMSEGMENT/(MUSHROOMSEGMENT/Speed))),180.0f/(MUSHROOMSEGMENT/Speed),Direction,true));
		}
	}
	
	//created the offsets that define a move horizontal is (Absolute values.)
	for(int i =0;i<MUSHROOMSEGMENT/Speed;i++){
		HorizontalOffsetQueue.push(PositionAndRotationOffset(sf::Vector2f(Speed,0),0,Direction,false));
	}

}

void CentipedeHeadSegment::Update(){
	
	Sprite.Update();
	if(CurrentQueue.empty()){
		//ConsoleMsg::WriteLine(Tools::ToString(TagNumber)+" Is MAKING A DECISION!");
		
		//if the head is at the lowest row, then it has touched the bottom and each the veritcal target direction
		if(WorldPosition.y>=BottomBlasterBorder ){
			if(IsPoisonedState==true){
				//Direction.x=-Direction.x;
			}
			Direction.y=-1;
			HasTouchedBottom=true;
			WaveManager::ActivateHeadSpawning(); //begin spawning heads
			IsPoisonedState=false;
			
		}


		if(IsTurning){IsTurning=false; Direction.x=-Direction.x;}// if the centipede was turning before then switch direction

		

		//If the centipede is at the top blaster border and has touched the bottom
		//row then flip the direction.
		if(WorldPosition.y <= TopBlasterBorder && HasTouchedBottom==true){
			Direction.y=1;
		}
		

		if(IsPoisonedState){
   			CurrentQueue =TurnOffsetQueue; //as long as centipede is poisoned it will set current to a turn(untill it hits bottom)
			IsTurning=true;
		}else if(WorldPosition.x<= ( ( MUSHROOMSEGMENT/2 )) &&Direction.x==-1||WorldPosition.x>=WindowManager::MainWindow.getView().getSize().x-( ( MUSHROOMSEGMENT/2 ))&&Direction.x==1){// if on the sides
			//set current workload to a turn manuever
			CurrentQueue =TurnOffsetQueue;
			IsTurning =true;
				
		}else if (   Field->CheckMushroomForPoison(Field->WorldPositionToMushroomGrid(WorldPosition+sf::Vector2f(Direction.x*MUSHROOMSEGMENT,0)  ) )   ){
			
			IsPoisonedState =true; // set the centipede to being poisoned
			CurrentQueue = TurnOffsetQueue;//next move will be a turn
			Direction.y=1;//centipede always goes down on a poison mushroom
			IsTurning =true;
		}
		else if(Field->CheckGridSpace(Field->WorldPositionToMushroomGrid(WorldPosition +sf::Vector2f(Direction.x*MUSHROOMSEGMENT,0)    )) ){
				
 			if(WorldPosition.y <= TopBlasterBorder && HasTouchedBottom==true){
				Direction.y=1;
			}
			//set current work load to a turn
			CurrentQueue =TurnOffsetQueue;
			IsTurning =true;

				
		}else{
				
			//set current workload to a HorizontalCommand	
			CurrentQueue = HorizontalOffsetQueue;				
				
		}
			
			
	}
			
	//regular check... if the current stack is not empty then work on the stack
	if(!CurrentQueue.empty()){

		//process the movement from the first offset
		WorldPosition+=sf::Vector2f(CurrentQueue.front().PositionOffset.x*Direction.x,CurrentQueue.front().PositionOffset.y*Direction.y);
		Sprite.setPosition(WorldPosition);
		Sprite.setRotation(Sprite.getRotation()+CurrentQueue.front().RotationOffset*Direction.x*Direction.y);
			
		if(Next!=NULL){
			Next->FutureQueue.push(PositionAndRotationOffset(CurrentQueue.front().PositionOffset,CurrentQueue.front().RotationOffset,Direction,IsTurning));
		}
		CurrentQueue.pop();
			
	}
	
}

void CentipedeHeadSegment::Draw(){
	WindowManager::MainWindow.draw(Sprite);
}

void CentipedeHeadSegment::Destroy(){
	CentipedeFactory::RemoveCentipedeSegmentFromAliveList(*this);
	if(NumberOfInstances==0){//if this is the last centipede on the screen
		SoundManager::StopCentipedeSound();//stop the CentipedeLoop.
				
	}


	DeregisterCollision<CentipedeHeadSegment>(*this); //deregister collision

	//empty out the stacks of information (delete not needed due to being queues of objects not pointers of objects
	while(!FutureQueue.empty()){
			FutureQueue.pop();

	}
	while(!CurrentQueue.empty()){
			CurrentQueue.pop();

	}
}






void CentipedeHeadSegment::Collision( BlasterBullet *other){

	--NumberOfInstances;
	//ConsoleMsg::WriteLine(Tools::ToString(CentipedeFactory::GetNumberOfAliveSegments()));
	if(CentipedeFactory::GetNumberOfAliveSegments()==1){//if all centipede heads after this collision will be gone
		PlayerManager::GetPlayer()->IncrementPlayerCurrentWave();//player has won round
		WaveManager::StopSpawningCentipedeHeads();//stop spawning heads from last round due to centipede hitting the bottom
		WaveManager::LoadPlayerWave();//load the next wave
	}
	//adds score to current score
	PlayerManager::AddToCurrentScore(ScoreValue);

	//spawns death
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	//creates a mushroom in the area in front of the centipede if there is not already
	//a mushroom in that spot.
	if(!Field->CheckGridSpace(Field->WorldPositionToMushroomGrid(WorldPosition+sf::Vector2f(MUSHROOMSEGMENT*Direction.x,0))))
	{
		Field->CreateMushroomAtCoordinate(Field->WorldPositionToMushroomGrid(WorldPosition+sf::Vector2f(MUSHROOMSEGMENT*Direction.x,0)));
	}
	

	
	if(Next!=NULL){
	//create centipede at previous location(this worldlocation - one cell length)
		
		CentipedeHeadSegment* cHead = &RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(Next->WorldPosition,Speed,*Field,Next->Direction);
		CentipedeSegment* it =cHead;
		if(!Next->CurrentQueue.empty()&&Next->CurrentQueue.front().IsInATurn){
			cHead->IsTurning =true;
		}

		cHead->Sprite.setRotation(Next->Sprite.getRotation());
	
		if(!Next->CurrentQueue.empty()){
			cHead->CurrentQueue = Next->CurrentQueue;
		}
		//cHead->Direction = Next->Direction;
	
		//If there is a second body after the head ( 3 or more body segments present) 
		if(Next->Next!=NULL){
			//set the new centipede's next to the 2nd body behind head (since head replaces the 1st body behind it) 
			Next->Next->Previous = cHead;
			//set the new centipedes next to the 2nd body behind the head at collision
			cHead->Next = Next->Next;
		}
		
		Next->MarkForDestroy();//destroys the 1st body after the head (the one head will replace) Pointers set to NULL (on destroy())

		
		int NumberOfSegments=0;

		//gets the number of segments after collision behind new head
		while(it!=NULL){
			++NumberOfSegments;
			it=it->Next;
		}
		it=cHead;

		//asigns new update order based on size (relative to new head)
		int UpdateCounter= 0;
		while(it!=NULL){
			it->SetUpdateOrder(NumberOfSegments-UpdateCounter);
			it= it->Next;
			
			
			++UpdateCounter;

		
		}

	


	}
	

	//Reset the update order of body segments

	Next=NULL;
	Previous=NULL;
	if(Next!=NULL){
		Next->Previous=NULL;
	}

	this->MarkForDestroy();//destroy the head that was collided with.  Pointers set to NULL (on destroy())
	
}

void CentipedeHeadSegment::ResetNumberOfInstances(){
	NumberOfInstances =0;
}

void CentipedeHeadSegment::Collision(Blaster* other){

	//create an Enemy explosion object on bulletCollision
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	//on collision this will be destroyed
	this->MarkForDestroy();
}