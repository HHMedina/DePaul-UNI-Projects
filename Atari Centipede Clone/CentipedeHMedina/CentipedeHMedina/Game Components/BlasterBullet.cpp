#include "BlasterBullet.h"
#include "Mushroom.h"
#include "Blaster.h"
#include "RecyclingBulletFactory.h"
#include "SoundManager.h"
#include "Spider.h"
#include "Flea.h"
#include "CentipedeBodySegment.h"
#include "CentipedeHeadSegment.h"
#include "Scorpion.h"

BlasterBullet::BlasterBullet(const sf::Vector2f spawnLocation,Blaster& spawner){
	MyShooter = &spawner;
	Position = spawnLocation;
	SPEED = 500*2;
	BulletSprite = AnimatedSprite(ResourceManager::GetTexture("Bullet"), 1, 1);
	BulletSprite.setOrigin(BulletSprite.getTextureRect().width / 2.0f, BulletSprite.getTextureRect().height / 2.0f);
	bitmap = ResourceManager::GetTextureBitmap("Bullet");
	SetCollider(BulletSprite, bitmap,true);
	BulletSprite.setPosition(Position);
	SetDrawOrder(1000);
	this->SetUpdateOrder(3);
	
}

void BlasterBullet::Initialize(sf::Vector2f p,Blaster& spawner ){
	MyShooter = &spawner;
	Position =p;
	RegisterCollision<BlasterBullet>( *this );
	SoundManager::PlayBlasterSound();
	
}

void BlasterBullet::Update(){
	BulletSprite.Update();

	Position.y-=SPEED * Game::FrameTime();
	//if the bullet is past the frame of the window(outside of the "world")
	if(Position.y<=(BulletSprite.getTextureRect().height / 2.0f)){//+MUSHROOMSEGMENT){
		this->MarkForDestroy();
		
	}

	BulletSprite.setPosition(Position);
}
void BlasterBullet::Draw(){
	WindowManager::MainWindow.draw(BulletSprite);
}
void BlasterBullet::Destroy(){
	MyShooter->BulletFired=NULL; // set the shooter as having no bullet
	DeregisterCollision<BlasterBullet>(*this);
}

void BlasterBullet::Collision( Mushroom *other ){
	this->MarkForDestroy();//destroy self upon hitting a mushroom
	
}

void BlasterBullet::Collision( Spider *other){
	this->MarkForDestroy();
}

void BlasterBullet::Collision( Flea *other){
	this->MarkForDestroy();
}

void BlasterBullet::Collision( CentipedeHeadSegment* other){
	this->MarkForDestroy();
}

void BlasterBullet::Collision( CentipedeBodySegment* other){
	this->MarkForDestroy();
}

void BlasterBullet::Collision( Scorpion * other){
	this->MarkForDestroy();
}