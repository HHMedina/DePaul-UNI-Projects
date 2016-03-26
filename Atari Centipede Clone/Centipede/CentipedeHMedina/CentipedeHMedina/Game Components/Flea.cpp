#include "Flea.h"
#include "MushroomField.h"
#include  <stdlib.h>
#include <time.h>
#include "BlasterBullet.h"
#include "EnemyExplosion.h"
#include "RecyclingEnemyExplosionFactory.h"
#include "WaveManager.h"
#include "Blaster.h"
#include "SoundManager.h"
#include "PlayerManager.h"

Flea::Flea(const sf::Vector2f SpawnLocation,const float speed,Mushroomfield& father){
	
	
	//gives ability to spawn mushrooms initially
	CanSpawnMushrooms=false;

	//Sets initial HP
	HP=2;

	//sets up time generated seed of rand
	srand(time(NULL));

	//sets up the chance to spawn mushrooms
	MushroomSpawningChance = 20.0f;

	//sets up initial location and field Flea Belongs to
	WorldPosition = SpawnLocation;
	FatherMushroomfield = &father;
	
	//sets up speed multiplier
	SecondSpeedMultiplier =2; //doubles

	//sets up inital speed
	Speed=speed;

	//sets up the second speed after being hit
	SpeedAfterBeingHit = speed *SecondSpeedMultiplier;

	//Sets up the visual Sprite
	Sprite = AnimatedSprite(ResourceManager::GetTexture("Flea"),2,2);
	Sprite.SetAnimation(0,3,true,true);
	Sprite.SetLoopSpeed(24.0f);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(WorldPosition);
	
	//Sets up Collider
	bitmap = ResourceManager::GetTextureBitmap("Flea");
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<Flea>(*this);

	//Sets Draw Order
	SetDrawOrder(999);

	//Set up The precomputed Downward motion with initial speed
	for(int i =0;i<MUSHROOMSEGMENT/speed;i++){
		DownwardQueue.push(PositionAndRotationOffset(sf::Vector2f(0,speed),0,sf::Vector2f(0,1),false));
	}

	//Set up The precomputed Downward motion with the speed after being hit
	for(int i =0;i<MUSHROOMSEGMENT/SpeedAfterBeingHit;i++){
		SecondSpeedDownwardQueue.push(PositionAndRotationOffset(sf::Vector2f(0,SpeedAfterBeingHit),0,sf::Vector2f(0,1),false));
	}


}


void Flea::Draw(){
	//Draw the Sprite.
	WindowManager::MainWindow.draw(Sprite);

}
void Flea::Update(){
	//updates the Sprite at each frame
	Sprite.Update();
	

	if(CurrentQueue.empty()){//if it is empty

		//if Flea is out of the viewing area
		if(WorldPosition.y>WindowManager::MainWindow.getView().getSize().y){
			this->MarkForDestroy();//Destroy this instance.
		}

		if(CanSpawnMushrooms){//if Flea is capable of spawning Mushrooms
			if(!FatherMushroomfield->CheckGridSpace( FatherMushroomfield->WorldPositionToMushroomGrid(WorldPosition) ) ){ //if there is no mushroom in the current position
				//create a mushroom at current coordinate based on probability
				if(rand()%100<MushroomSpawningChance){ //if the random number is less than theSpawning chance. if chance = 20 then 0-19 out of 100 cause a spawn. 20/100 = 20% chance if chance =100 then, every number is below 100 (0 - 99)
					FatherMushroomfield->CreateMushroomAtCoordinate( FatherMushroomfield->WorldPositionToMushroomGrid(WorldPosition) );
				}
			}
		}

		//copy the correct DownwardQueue and to the the current job and do the first step (to avoid no movement at decision frame)
		if(CanSpawnMushrooms){//if in first stage
			CurrentQueue = DownwardQueue;//do normal queue
		}else{
			CurrentQueue = SecondSpeedDownwardQueue;//do faster queue
		}
		

	}
	


	//If the current queue is nto empty then work on the offsets
	if(!CurrentQueue.empty()){

		//process the next offset in Current queue
		WorldPosition+=sf::Vector2f(CurrentQueue.front().PositionOffset.x*CurrentQueue.front().Direction.x,CurrentQueue.front().PositionOffset.y*CurrentQueue.front().Direction.y);
		Sprite.setPosition(WorldPosition);

		//Remove the queue element that was processed
		CurrentQueue.pop();

	}
}
void Flea::Destroy(){
	
	
	//set flea to dead so wavemanager can be able to spawn flea again
	WaveManager::SetFleaToDead();
	WaveManager::CheckToSpawnFlea();

	//Deregister Collision of this Flea
	DeregisterCollision<Flea>(*this);
}


void Flea::Initialize(sf::Vector2f SpawnLocation,const float speed, Mushroomfield& father){
	
	SoundManager::PlayFleaSound();

	//resets initial
	HP=2;

	//resets ability to spawn mushroom
	CanSpawnMushrooms=true;

	//sets up location aspects
	WorldPosition = SpawnLocation;
	FatherMushroomfield = &father;
	Sprite.setPosition(WorldPosition);

	//sets up new speed
	Speed=speed;

	//sets up the second speed after being hit
	SpeedAfterBeingHit = speed *SecondSpeedMultiplier;

	//resets up collision
	RegisterCollision<Flea>( *this );
	

	//clear the initial speed precomputed queue
	while(!DownwardQueue.empty()){
		DownwardQueue.pop();
	}

	//clear the initial secondspeed precomputed queue
	while(!SecondSpeedDownwardQueue.empty()){
		SecondSpeedDownwardQueue.pop();
	}

	//Set up The new precomputed Downward motion with new  initial speed
	for(int i =0;i<MUSHROOMSEGMENT/speed;i++){
		DownwardQueue.push(PositionAndRotationOffset(sf::Vector2f(0,speed),0,sf::Vector2f(0,1),false));
	}

	//Set up The new precomputed Downward motion with the new speed after being hit
	for(int i =0;i<MUSHROOMSEGMENT/SpeedAfterBeingHit;i++){
		SecondSpeedDownwardQueue.push(PositionAndRotationOffset(sf::Vector2f(0,SpeedAfterBeingHit),0,sf::Vector2f(0,1),false));
	}

}

void Flea::Collision( BlasterBullet *other ){

	if(HP<2){//if no hp will be left after this collision
		//adds score to current score
		PlayerManager::AddToCurrentScore(ScoreValue);

		//create an explosion object on destroy
		RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);
	
		this->MarkForDestroy();//destroy this flea
	}else{
		CanSpawnMushrooms=false;//else if has been hit then disable mushroom spawning ability
		--HP; //lower HP

		//empties the current queue so that faster queue can start immediately.
		//this will create a slight offset, but it doesnt matter because mushrooms no longer
		//will be spawned AND due to the nature of the Mushroomchecking in the Mushroomfield,
		//which checks if its within a cell, not the exact middle of the cell.
		while(!CurrentQueue.empty()){
			CurrentQueue.pop();
		}
	}


};

void Flea::Collision( Blaster *other ){

	//create an Enemy explosion object on bulletCollision
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	//on collision the Flea will be destroyed
	this->MarkForDestroy();

};