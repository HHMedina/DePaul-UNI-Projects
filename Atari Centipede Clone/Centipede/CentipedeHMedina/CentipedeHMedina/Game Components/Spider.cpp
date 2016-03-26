#include "Spider.h"
#include "MushroomField.h"
#include "BlasterBullet.h"
#include "EnemyExplosion.h"
#include "RecyclingEnemyExplosionFactory.h"
#include "RecyclingScoreExplosionFactory.h"
#include "ScoreExplosion.h"
#include "Blaster.h"
#include <math.h>
#include "WaveManager.h"
#include "SoundManager.h"
#include "PlayerExplosion.h"
#include "PlayerManager.h"


Spider::Spider(const sf::Vector2f SpawnLocation,const float speed,const sf::Vector2i InitialDirection,Mushroomfield& father){
	
	//set up definition of top border
	TOPBORDER =WindowManager::MainWindow.getView().getSize().y-(MUSHROOMSEGMENT*11 - MUSHROOMSEGMENT/2);
	BottomBlasterBorder= WindowManager::MainWindow.getView().getSize().y - (MUSHROOMSEGMENT/2)-MUSHROOMSEGMENT;
	

	//sets up time generated seed of rand
	srand(time(NULL));

	//plays spider sound on construction
	SoundManager::PlaySpiderSound();
	
	//set up bool
	IsOnBorder=false;

	//sets up initial location and field Flea Belongs to
	WorldPosition = SpawnLocation;
	FatherMushroomfield = &father;
	
	//sets up inital speed with initial direction
	Speed=speed;
	Direction=InitialDirection;

	//Sets up the visual Sprite
	Sprite = AnimatedSprite(ResourceManager::GetTexture("Spider"),4,2);
	Sprite.SetAnimation(0,5,true,true);
	Sprite.SetLoopSpeed(24.0f);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(WorldPosition);
	Sprite.setScale(2,2);

	//Sets up Collider
	bitmap = ResourceManager::GetTextureBitmap("Spider");
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<Spider>(*this);

	//Sets Draw Order
	SetDrawOrder(999);

	//Set up The precomputed Downward motion with initial speed
	for(int i =0;i<MUSHROOMSEGMENT/speed;i++){
		DownwardQueue.push(PositionAndRotationOffset(sf::Vector2f(0,speed),0,sf::Vector2f(0,1),false));
	}

	//Set up The precomputed Diagonal motion with initial speed
	for(int i =0;i<MUSHROOMSEGMENT/speed;i++){
		DiagonalQueue.push(PositionAndRotationOffset(sf::Vector2f(speed,speed),0,sf::Vector2f(0,1),false));
	}



}


void Spider::Draw(){
	//Draw the Sprite.
	WindowManager::MainWindow.draw(Sprite);

}
void Spider::Update(){
	//updates the Sprite at each frame
	Sprite.Update();

	//if Spider is out of the viewing area -- is in update for precise checking
	if(((WorldPosition.x>WindowManager::MainWindow.getView().getSize().x+SPIDERSIZEX/2)&&Direction.x==1)||((WorldPosition.x<0-SPIDERSIZEX/2)&&Direction.x==-1)){

		this->MarkForDestroy();//Destroy this instance.

		//if the spider dies due to being out of screen, then the next spider comes at a random time.
		WaveManager::SetSpiderAlarm(WaveManager::GetSpiderSpawnTime());

	}

	

	
	if(CurrentQueue.empty()){//if the cycle queue is empty then decide what the next cycle is (random)
	
		int YDirectionchoice = rand()%2;


		if(WorldPosition.y<=TOPBORDER||WorldPosition.y>=BottomBlasterBorder){//if on one of the borders, then change the direction to up or down accordingly
			
			if(WorldPosition.y<=TOPBORDER)//if at the top border
				Direction.y=1;

			if(WorldPosition.y>=BottomBlasterBorder)// if at the bottom border
				Direction.y=-1;

		} else if(YDirectionchoice == 0){// if not at the borders then choose a random direction to go in
			Direction.y =1;
		}else{
			Direction.y =-1;
		}

		int choice = rand()%2;//  randomly picks wether to go diagonal or vertical
		int numberOfMoves = rand()%4+3;//addition of one to prevent int from being 0. This randomly chooses how many diagonals 
									   //or vertical moves to do in the next cycle, with a minimum and a maximum number
									   

		if(choice == 0){// next move is downward queue is choice is 0
			for(numberOfMoves;numberOfMoves>0;--numberOfMoves){
				CurrentQueue.push(DownwardQueue);
			}
		}else if(choice ==1){// next move is diagonal queue if choice is 2
			for(numberOfMoves;numberOfMoves>0;--numberOfMoves){
				CurrentQueue.push(DiagonalQueue);
			}
		}
	}

	
	//If the current cycle queue is not empty then work on the next offset queue 
	if(!CurrentQueue.empty()){

		
		
		//If The Spider is on the top or bottom Border, then empty the Current Cycle
		//Change the vertical direction and make a new decision 
		if((WorldPosition.y<=TOPBORDER||WorldPosition.y>=BottomBlasterBorder)&&!IsOnBorder)
		{
			while(!CurrentQueue.empty()){
				CurrentQueue.pop();
			}
			
			//prevents a loop of decision making based on if it is on the border. (will only make one decision on border instead of infinite.)
			IsOnBorder=true;

		} else if(!CurrentQueue.front().empty()){//if the current queue of the cycle is not empty
			//process the next offset in the queue
			WorldPosition+=sf::Vector2f(CurrentQueue.front().front().PositionOffset.x*Direction.x,CurrentQueue.front().front().PositionOffset.y*Direction.y);
			Sprite.setPosition(WorldPosition);
			//Remove the queue element that was processed
			CurrentQueue.front().pop();

		}else{ //if current queue is empty

		

			//sets if on border to false since it has moves away from border (next cell away and any other cell)
			IsOnBorder=false;
			//If the Spider is on top of a mushroom create a mushroom.
			if(FatherMushroomfield->CheckGridSpace( FatherMushroomfield->WorldPositionToMushroomGrid(WorldPosition) ) ){ //if there is no mushroom in the current position

				//create mushroom
				FatherMushroomfield->DestroyMushroom( FatherMushroomfield->WorldPositionToMushroomGrid(WorldPosition) );
			
			}

			CurrentQueue.pop();// pops the top of the queue because it is empty and moves on to the next queue in the cycle
		}
		
	}
	
	
}
void Spider::Destroy(){
	
	//Stop the sound
	SoundManager::StopSpiderSound();
	
	//Deregister Collision of this Flea
	DeregisterCollision<Spider>(*this);

	//callback method to let wavemanager it can spawn another spider
	WaveManager::SetSpiderToDead();

}


void Spider::Initialize(sf::Vector2f SpawnLocation,const float speed,const sf::Vector2i InitialDirection, Mushroomfield& father){
	
	//reset direction
	Direction =InitialDirection;


	SoundManager::PlaySpiderSound();

	//resets initial
	HP=2;

	//sets up location aspects
	WorldPosition = SpawnLocation;
	FatherMushroomfield = &father;
	Sprite.setPosition(WorldPosition);

	//sets up new speed
	Speed=speed;

	//resets collision
	RegisterCollision<Spider>( *this );
	

	//clear the initial speed precomputed queue
	while(!DownwardQueue.empty()){
		DownwardQueue.pop();
	}

	//clear the initial secondspeed precomputed queue
	while(!DiagonalQueue.empty()){
		DiagonalQueue.pop();
	}

	//Set up The new precomputed Downward motion with new  initial speed
	for(int i =0;i<MUSHROOMSEGMENT/speed;i++){
		DownwardQueue.push(PositionAndRotationOffset(sf::Vector2f(0,speed),0,sf::Vector2f(0,1),false));
	}

	//Set up The precomputed Diagonal motion with initial speed
	for(int i =0;i<MUSHROOMSEGMENT/speed;i++){
		DiagonalQueue.push(PositionAndRotationOffset(sf::Vector2f(speed,speed),0,sf::Vector2f(0,1),false));
	}

 }

void Spider::Collision( BlasterBullet *other ){

	
	float DistanceApart = std::sqrtf( std::powf((WorldPosition.x-other->MyShooter->Position.x),2) + std::powf(WorldPosition.y-other->MyShooter->Position.y,2) );

	if( DistanceApart<=CloseDistance){// if within close proximity
		RecyclingScoreExplosionFactory::CreateScoreExplosion(CloseScoreValue,WorldPosition);
		PlayerManager::AddToCurrentScore(CloseScoreValue);
	}else if(DistanceApart<=MediumDistance){//if within medium proximity
		RecyclingScoreExplosionFactory::CreateScoreExplosion(MediumScoreValue,WorldPosition);
		PlayerManager::AddToCurrentScore(MediumScoreValue);
	} else{//far proximity
		RecyclingScoreExplosionFactory::CreateScoreExplosion(FarScoreValue,WorldPosition);
		PlayerManager::AddToCurrentScore(FarScoreValue);
	}

	//When the spider gets killed by the bullet, it will cause another spawn exactly 4 seconds after death
	WaveManager::SetSpiderAlarm(WaveManager::TimeInbetweenSpiderOnDeath);

	//create an explosion object on destroy
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	this->MarkForDestroy();//destroy this spider

};

void Spider::Collision( Blaster *other ){
	//create an explosion object on destroy
	RecyclingEnemyExplosionFactory::CreateEnemyExplosion(WorldPosition);

	this->MarkForDestroy();//destroy this spider
};