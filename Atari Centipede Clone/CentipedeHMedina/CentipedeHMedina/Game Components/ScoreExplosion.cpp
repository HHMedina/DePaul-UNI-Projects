#include "ScoreExplosion.h"



ScoreExplosion::ScoreExplosion(const int WhichScore,const sf::Vector2f SpawnLocation){
	
	//set up visual sprite based on which score is being presented
	if(WhichScore==300){

	Sprite = AnimatedSprite(ResourceManager::GetTexture("Score300"),1,1,0.0f,0,0,false,false);

	} else if(WhichScore==600){

		Sprite = AnimatedSprite(ResourceManager::GetTexture("Score600"),1,1,0.0f,0,0,false,false);

	} else{

		Sprite = AnimatedSprite(ResourceManager::GetTexture("Score900"),1,1,0.0f,0,0,false,false);
	}


	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	Sprite.setPosition(SpawnLocation);
	Sprite.setScale(2,2);
	SetDrawOrder(1000);
	this->SetAlarm(0,LifeTime);

}






void ScoreExplosion::Draw(){
	WindowManager::MainWindow.draw(Sprite);

}
void ScoreExplosion::Update(){
	
	
}
void ScoreExplosion::Destroy(){
	
}

void ScoreExplosion::Initialize(const int WhichScore,const sf::Vector2f SpawnLocation){

	//set the sprite to the new location and at the beginning of the animation
	if(WhichScore==300){

	Sprite = AnimatedSprite(ResourceManager::GetTexture("Score300"),1,1,0.0f,0,0,false,false);

	} else if(WhichScore==600){

		Sprite = AnimatedSprite(ResourceManager::GetTexture("Score600"),1,1,0.0f,0,0,false,false);

	} else{

		Sprite = AnimatedSprite(ResourceManager::GetTexture("Score900"),1,1,0.0f,0,0,false,false);
	}

	Sprite.setScale(2,2);
	Sprite.setPosition(SpawnLocation);
	Sprite.setOrigin(Sprite.getTextureRect().width / 2.0f, Sprite.getTextureRect().height / 2.0f);
	this->SetAlarm(0,LifeTime);
}
void ScoreExplosion::Alarm0(){

	this->MarkForDestroy();

}