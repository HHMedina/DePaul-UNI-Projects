#include "Blaster.h"
#include "BlasterBullet.h"
#include "Mushroom.h"
#include "PlayerController.h"
#include "Controller.h"
#include "PlayerExplosion.h"
#include "RecyclingBulletFactory.h"
#include "WaveManager.h"
#include "AttracterMode.h"
#include "Spider.h"
#include "PlayerManager.h"
#include "Flea.h"
#include "CentipedeBodySegment.h"
#include "CentipedeHeadSegment.h"
#include "RecyclingPlayerExplosion.h"
#include "Scorpion.h"

Blaster::Blaster(int type){

	Speed=5;

	IsInvincible =false;

	KeyBoard = new Playercontroller(*this); // creates a new controller only if its a new blaster
	AI = new AttracterMode(*this); //creates a new controller only if its a new blaster

	
	this->SetUpdateOrder(3);
	BulletFired= NULL;
	//sets the controller;
	if(type == 0){
		controller = AI;
	}else if(type == 1){
		controller = KeyBoard;
	}
	
	//Movement Border Variables
	TopBorder = WindowManager::MainWindow.getView().getSize().y-(7*MUSHROOMSEGMENT)+MUSHROOMSEGMENT/2;
	BottomBorder = WindowManager::MainWindow.getView().getSize().y-(BLASTERSEGMENT/2)-MUSHROOMSEGMENT;
	//movement Variables
	
	//Speed = 150*2;	
	//sets visual aspects of the gameobject
	bitmap = ResourceManager::GetTextureBitmap("Blaster");
	Sprite = AnimatedSprite( ResourceManager::GetTexture("Blaster"),1,1,1,0,0,false);
	Sprite.setOrigin(Sprite.getTextureRect().width/2.0f,Sprite.getTextureRect().height/2.0f);
	//sets the blasters initial position
	Position = sf::Vector2f(WindowManager::MainWindow.getView().getSize().x/2.0f,WindowManager::MainWindow.getView().getSize().y-(BLASTERSEGMENT/2)-MUSHROOMSEGMENT);
	SetDrawOrder(1000);

	Sprite.setPosition(Position);

	//sets the collider and registers gameobject for collision checking
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<Blaster>(*this);
	
	
}

void Blaster::Initialize(const int type){

	
	IsInvincible =false;

	Speed=5;	

	BulletFired= NULL;
	//sets the controller;
	if(type == 0){
		controller = AI;
	}else if(type == 1){
		controller = KeyBoard;
	}


	Position = sf::Vector2f(WindowManager::MainWindow.getView().getSize().x/2.0f,WindowManager::MainWindow.getView().getSize().y-(BLASTERSEGMENT/2)-MUSHROOMSEGMENT);
	SetDrawOrder(1000);

	Sprite.setPosition(Position);

	//sets the collider and registers gameobject for collision checking
	SetCollider(Sprite,bitmap,true);
	RegisterCollision<Blaster>(*this);
}

Blaster::~Blaster(){

	//clean up objects
	delete controller;
	delete KeyBoard;
	delete AI;
}

void Blaster::Draw(){
	//draws the main sprite
	WindowManager::MainWindow.draw(Sprite);
}

void Blaster::Destroy(){
	
	//destroy all enties from the current wave
	WaveManager::DestroyAllWaveEntities();
	
	//remove from observer list
	DeregisterInput();
	DeregisterCollision<Blaster>(*this);
	
	//Destroy any spawned bullets
	if(BulletFired!=NULL){
		BulletFired->MarkForDestroy();
		BulletFired=NULL;
	}

}



void Blaster::Collision( Mushroom *other){
	//on collision, the player will be set to the last position (clip of mushroom)
	Position =LastPosition;
	Sprite.setPosition(Position);
	
	
}

void Blaster::Collision( Spider *other){

	//create player explosion
	PlayerManager::SetPlayerExplosion( RecyclingPlayerExplosionFactory::CreatePlayerExplosion(this->Position) );

	//on collision, the player will be destroyed
	this->MarkForDestroy();
	
}

void Blaster::Collision( CentipedeHeadSegment *other){
	
	//create player explosion
	PlayerManager::SetPlayerExplosion( RecyclingPlayerExplosionFactory::CreatePlayerExplosion(this->Position) );

	//on collision, the player will be destroyed
	this->MarkForDestroy();
	
	
}

void Blaster::Collision( CentipedeBodySegment *other){
	
	//create player explosion
	PlayerManager::SetPlayerExplosion( RecyclingPlayerExplosionFactory::CreatePlayerExplosion(this->Position) );

	//on collision, the player will be destroyed
	this->MarkForDestroy();
	
}

void Blaster::Collision( Flea *other){
	
	//create player explosion
	PlayerManager::SetPlayerExplosion( RecyclingPlayerExplosionFactory::CreatePlayerExplosion(this->Position) );

	//on collision, the player will be destroyed
	this->MarkForDestroy();
	
	
}

void Blaster::Collision( Scorpion *other){

	
	//create player explosion
	PlayerManager::SetPlayerExplosion( RecyclingPlayerExplosionFactory::CreatePlayerExplosion(this->Position) );

	//on collision, the player will be destroyed
	this->MarkForDestroy();
	
	
}


void Blaster::Update(){
	//calls the controller's movement system function
	if(controller!=NULL){
		controller->move();
	}
}

sf::Vector2f Blaster::GetPosition(){

	return Position;
}

void Blaster::DestroyBullet(){

	if(BulletFired!=NULL){
		BulletFired->MarkForDestroy();	
	}

}



