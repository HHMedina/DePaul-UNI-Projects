#include "EnemyExplosion.h"
#include "SoundManager.h"


EnemyExplosion::EnemyExplosion(const sf::Vector2f SpawnLocation){
	
	//set up visual sprite
	Sprite = AnimatedSprite(ResourceManager::GetTexture("EnemyExplosion"),3,2,80.0f);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(SpawnLocation);
	Sprite.SetAnimation(0,5);
	Sprite.setScale(2,2);
	SetDrawOrder(1200);

	//set up sound
	SoundManager::PlayEnemyDeathSound();
}



void EnemyExplosion::Draw(){
	WindowManager::MainWindow.draw(Sprite);

}
void EnemyExplosion::Update(){
	Sprite.Update();

	//if the animation has ended then destroy the explosion
	if(Sprite.IsLastAnimationFrame()){
		this->MarkForDestroy();
	}
	
}
void EnemyExplosion::Destroy(){
	
}

void EnemyExplosion::Initialize(const sf::Vector2f SpawnLocation){
	//replays explosion
	//SoundManager::PlayEnemyDeathSound();

	//set the sprite to the new location and at the beginning of the animation
	Sprite.SetFrame(0);
	Sprite.setPosition(SpawnLocation);

}
