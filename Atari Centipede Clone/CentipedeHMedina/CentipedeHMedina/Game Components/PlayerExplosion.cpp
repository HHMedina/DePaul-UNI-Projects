#include "PlayerExplosion.h"
#include "WaveManager.h"
#include "SoundManager.h"
#include "PlayerManager.h"

PlayerExplosion::PlayerExplosion(sf::Vector2f BirthPosition){
	
	//set up the visual and animation of the PlayerExplosion
	PlayerExplosionSprite = AnimatedSprite( ResourceManager::GetTexture("PlayerExplosion"),4,2,8.0f);
	PlayerExplosionSprite.SetAnimation(0,7);
	PlayerExplosionSprite.setOrigin(PlayerExplosionSprite.getTextureRect().width/2.0f,PlayerExplosionSprite.getTextureRect().height/2.0f);
	PlayerExplosionSprite.setPosition(BirthPosition);
	SetDrawOrder(1000);
	
	
}

void PlayerExplosion::Initialize(sf::Vector2f BirthPosition){
	PlayerExplosionSprite.SetFrame(0);
	PlayerExplosionSprite.setPosition(BirthPosition);

	//on creation PlayerDeathSound
	SoundManager::PlayPlayerDeathSound(); // alwasy called when playerexplosion is created, regardless if new or not (prevent two sounds playing at once)
}

PlayerExplosion::PlayerExplosion (PlayerExplosion& other){
	PlayerExplosionSprite = other.PlayerExplosionSprite;
	PlayerExplosionBitmap = other.PlayerExplosionBitmap;
}

void PlayerExplosion::Update(){
	PlayerExplosionSprite.Update();


	//If on last frame tell wavemanager player has died and remove the refernence to the explosion 
	//( It is referenced so that it can be deleted upon mode switching)
	if (PlayerExplosionSprite.IsLastAnimationFrame()){
		PlayerManager::DestroyPlayerExplosion();
		WaveManager::PlayerDied();
	}
	

}

void PlayerExplosion::Draw(){
	WindowManager::MainWindow.draw(PlayerExplosionSprite);
}