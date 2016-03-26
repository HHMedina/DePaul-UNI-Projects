#include "Player1.h"
#include "HUD.h"
#include "Mushroomfield.h"
#include "Blaster.h"
#include "BlasterFactory.h"

#include "WaveManager.h"

Player1::Player1(){
	CurrentScore=0;
	NumberOfLives=3;
	CurrentWave =0;
	PlayerField = new Mushroomfield();
	PlayerField->Initialize(StartNumberOfMushrooms);//StartNumberOfMushrooms
}

Player1::~Player1(){
	//delete Dynamically allocated memory
	delete PlayerField;
}

void Player1::AddToPlayerScore(int value){
	CurrentScore+= value;
	HUD::SetPlayer1Score(CurrentScore);

	if(CurrentScore>HUD::GetHighestScoreDisplayNumber()){// on new score, if current score is better than highscore, change hud highscore to 
		HUD::SetHighScore(CurrentScore);
	}
	
}

int Player1::GetPlayerScore(){
	return CurrentScore;
}

int& Player1::GetPlayerLives(){
	return NumberOfLives;
}

void Player1::UpdatePlayerLivesRemaining(){
	HUD::SetPlayer1LivesRemaining(NumberOfLives);
}

void Player1::ClearPlayerField(){

	PlayerField->ClearTheField();
}

void Player1::RespawnPlayerField(){

	PlayerField->RespawnField();
}

Mushroomfield&  Player1::GetPlayerField(){

	return *PlayerField;
}

int Player1::GetCurrentWave(){

	return CurrentWave;
}

void Player1::SetCurrentWave(int waveNumber){
	CurrentWave = waveNumber;
}

void Player1::IncrementPlayerCurrentWave(){

	++CurrentWave;
}

void Player1::LowerLivesByOne(){

	--NumberOfLives;
}

Blaster& Player1::SpawnNewPlayer(){

	return RecyclingBlasterFactory::CreateBlaster(1);
}

void Player1::PlayerInitialize(){

	CurrentScore=0;
	NumberOfLives=3;
	CurrentWave =0;
	PlayerField->Initialize(StartNumberOfMushrooms);
}
