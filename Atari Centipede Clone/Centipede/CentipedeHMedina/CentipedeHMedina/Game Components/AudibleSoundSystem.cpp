#include "AudibleSoundSystem.h"

AudibleSoundSystem::AudibleSoundSystem(){

	//set up all the audio buffers eith respectful loops
	FleaSound.setBuffer(ResourceManager::GetSound("FleaSound"));
	FleaSound.setVolume(25);
	FleaSound.setLoop(false);

	BlasterSound.setBuffer(ResourceManager::GetSound("BlasterShot"));
	BlasterSound.setVolume(25);
	BlasterSound.setLoop(false);

	ScorpionSound.setBuffer(ResourceManager::GetSound("ScorpionSound"));
	ScorpionSound.setVolume(25);
	ScorpionSound.setLoop(true);

	EnemyDeathSound.setBuffer(ResourceManager::GetSound("EnemyDeathSound"));
	EnemyDeathSound.setVolume(25);
	EnemyDeathSound.setLoop(false);

	PlayerDeathSound.setBuffer(ResourceManager::GetSound("PlayerDeathSound"));
	PlayerDeathSound.setVolume(25);
	PlayerDeathSound.setLoop(false);

	SpiderSound.setBuffer(ResourceManager::GetSound("SpiderSound"));
	SpiderSound.setVolume(25);
	SpiderSound.setLoop(true);

	CentipedeSound.setBuffer(ResourceManager::GetSound("CentipedeSound"));
	CentipedeSound.setVolume(25);
	CentipedeSound.setLoop(true);

}

AudibleSoundSystem::~AudibleSoundSystem(){
	
}


void AudibleSoundSystem::PlayBlasterSound(){
	BlasterSound.play();
}

void AudibleSoundSystem::PlayScorpionSound(){
	ScorpionSound.play();
}

void AudibleSoundSystem::PlayEnemyDeathSound(){
	EnemyDeathSound.play();
}

void AudibleSoundSystem::PlayPlayerDeathSound(){
	PlayerDeathSound.play();
}

void AudibleSoundSystem::PlayFleaSound(){
	FleaSound.play();
}

void AudibleSoundSystem::PlaySpiderSound(){
	SpiderSound.play();
}

void AudibleSoundSystem::PlayCentipedeSound(){
	CentipedeSound.play();
}

void AudibleSoundSystem::StopSpiderSound(){
	SpiderSound.stop();
}

void AudibleSoundSystem::StopScorpionSound(){
	ScorpionSound.stop();
}

void AudibleSoundSystem::StopCentipedeSound(){
	CentipedeSound.stop();
}

void AudibleSoundSystem::StopAllSounds(){
	FleaSound.stop();
	ScorpionSound.stop();
	BlasterSound.stop();
	EnemyDeathSound.stop();
	PlayerDeathSound.stop();
	CentipedeBeatSound.stop();
	SpiderSound.stop();
	CentipedeSound.stop();
}