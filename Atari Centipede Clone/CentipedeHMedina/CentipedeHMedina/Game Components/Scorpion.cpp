
#include "Scorpion.h"
#include "MushroomField.h"
#include "Mushroom.h"
#include "BlasterBullet.h"
#include "RecyclingEnemyExplosionFactory.h"
#include "EnemyExplosion.h"
#include "WaveManager.h"
#include "SoundManager.h"
#include "PlayerManager.h"
#include "Blaster.h"

Scorpion::Scorpion(const sf::Vector2f SpawnLocation,const float direction,Mushroomfield& father){
	
	//sets up initial location and field Flea Belongs to
	WorldPosition = SpawnLocation;
	FatherMushroomfield = &father;
	
	//sets up inital speed and direction of motion
	
	Direction =direction;

	

	//Sets up the visual Sprite
	Sprite = AnimatedSprite(ResourceManager::GetTexture("Scorpion"),4,1);
	Sprite.SetAnimation(0,3,true,true);
	Sprite.SetLoopSpeed(10.0f);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(WorldPosition);
	
	//sets up orientation of sprite corresponding to movement
	if(direction == 1){
		Sprite.setScale(FACERIGHT,1);
	}

	//Sets up Collider
	bitmap = ResourceManager::GetTextureBitmap("Scorpion");
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<Scorpion>(*this);

	//Sets Draw Order
	SetDrawOrder(1001);

	//Set up The precomputed Horizontal motion
	for(int i =0;i<MUSHROOMSEGMENT/ScorpionSpeed;i++){
		HorizontalQueue.push(PositionAndRotationOffset(sf::Vector2f(ScorpionSpeed,0),0,sf::Vector2f(direction,0),false));
	}


}


void Scorpion::Draw(){
	//Draw the Sprite.
	WindowManager::MainWindow.draw(Sprite);

}
void Scorpion::Update(){
	//updates the Sprite at each frame
	Sprite.Update();

	if(CurrentQueue.empty()){//if it is empty

		//if Scorpion is out of the viewing area (depending on initial direction)
		if((WorldPosition.x>=WindowManager::MainWindow.getView().getSize().x+(MUSHROOMSEGMENT+MUSHROOMSEGMENT/2)&&Direction == 1)||(WorldPosition.x<=0-(MUSHROOMSEGMENT+MUSHROOMSEGMENT/2)&&Direction == -1)){
			this->MarkForDestroy();//Destroy this instance.
			
		}
		
		if(FatherMushroomfield->CheckGridSpace(FatherMushroomfield->WorldPositionToMushroomGrid(WorldPosition))){ //if Scorpion is on a mushroom
			FatherMushroomfield->AccessMushroom(FatherMushroomfield->WorldPositionToMushroomGrid(WorldPosition) ).Poison();//set the mushroom to poison state
		}



		//copy the HoriztonalQueue and to the the current job and do the first step (to avoid no movement at decision frame)
		CurrentQueue = HorizontalQueue;


	}

	//If the current queue is nto empty then work on the offsets
	if(!CurrentQueue.empty()){

		//process the next offset in Current queue
		WorldPosition+=sf::Vector2f(CurrentQueue.front().PositionOffset.x*Direction,CurrentQueue.front().PositionOffset.y*CurrentQueue.front().Direction.y);
		Sprite.setPosition(WorldPosition);
		
		//Remove the queue element that was processed
		CurrentQueue.pop();

	}

}
void Scorpion::Destroy(){

	//starts the wave manager's alarm which will trigger another spawn
	WaveManager::SetScorpionAlarm(WaveManager::GetScorpionSpawnTime());
	WaveManager::SetScorpionToDead();
	//Stops sound
	SoundManager::StopScorpionSound();
	

	//Deregister Collision of this Scorpion
	DeregisterCollision<Scorpion>(*this);
}


void Scorpion::Initialize(sf::Vector2f SpawnLocation,const int direction, Mushroomfield& father){

	//restarts the sound
	SoundManager::PlayScorpionSound();

	//sets up location aspects
	WorldPosition = SpawnLocation;
	FatherMushroomfield = &father;
	Sprite.setPosition(WorldPosition);

	//sets up new direction
	Direction=direction;

	//sets up the direction of the sprite
	if(direction == 1){
		Sprite.setScale(FACERIGHT,1);
	}else{
		Sprite.setScale(FACELEFT,1);
	}

	//resets up collision
	RegisterCollision<Scorpion>( *this );
	
	

}

void Scorpion::Collision(BlasterBullet * other){
	//adds score to current score
	PlayerManager::AddToCurrentScore(ScoreValue);

	//create an explosion object on bulletCollision
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	this->MarkForDestroy();

}

void Scorpion::Collision(Blaster * other){

	//create an Enemy explosion object on bulletCollision
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	//destroy self
	this->MarkForDestroy();
}