#include "Player2.h"
#include "HUD.h"
#include "Mushroomfield.h"
#include "Blaster.h"
#include "BlasterFactory.h"

Player2::Player2(){
	CurrentScore=0;
	NumberOfLives=3;
	CurrentWave =0;
	PlayerField = new Mushroomfield();
	PlayerField->Initialize(StartNumberOfMushrooms);
}

Player2::~Player2(){

	//delete Dynamically allocated memory
	delete PlayerField;
}

void Player2::AddToPlayerScore(int value){
	CurrentScore+= value;
	HUD::SetPlayer2Score(CurrentScore);

	if(CurrentScore>HUD::GetHighestScoreDisplayNumber()){// on new score, if current score is better than highscore, change hud highscore to 
		HUD::SetHighScore(CurrentScore);
	}
	
}
int Player2::GetPlayerScore(){
	return CurrentScore;
}

int& Player2::GetPlayerLives(){
	return NumberOfLives;
}

void Player2::UpdatePlayerLivesRemaining(){
	HUD::SetPlayer2LivesRemaining(NumberOfLives);
}

void Player2::ClearPlayerField(){

	PlayerField->ClearTheField();
}

void Player2::RespawnPlayerField(){

	PlayerField->RespawnField();
}

Mushroomfield&  Player2::GetPlayerField(){

	return *PlayerField;
}

int Player2::GetCurrentWave(){

	return CurrentWave;
}

void Player2::SetCurrentWave(int waveNumber){
	CurrentWave = waveNumber;
}

void Player2::IncrementPlayerCurrentWave(){

	++CurrentWave;
}

void Player2::LowerLivesByOne(){

	--NumberOfLives;
}

Blaster& Player2::SpawnNewPlayer(){

	return RecyclingBlasterFactory::CreateBlaster(1);
}


void Player2::PlayerInitialize(){
	
	CurrentScore=0;
	NumberOfLives=3;
	CurrentWave =0;
	PlayerField->Initialize(StartNumberOfMushrooms);
}