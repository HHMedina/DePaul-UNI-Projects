#include "Player0.h"
#include "HUD.h"
#include "Mushroomfield.h"
#include "Blaster.h"
#include "BlasterFactory.h"

Player0::Player0(){
	CurrentScore=0;
	NumberOfLives=3;
	CurrentWave =0;
	PlayerField = new Mushroomfield();
	PlayerField->Initialize(StartNumberOfMushrooms);//StartNumberOfMushrooms
}

Player0::~Player0(){
	//delete Dynamically allocated memory
	delete PlayerField;
}

void Player0::AddToPlayerScore(int value){
		
}

int Player0::GetPlayerScore(){
	return CurrentScore;
}

int& Player0::GetPlayerLives(){
	return NumberOfLives;
}

void Player0::UpdatePlayerLivesRemaining(){
}

void Player0::ClearPlayerField(){

	PlayerField->ClearTheField();
}

void Player0::RespawnPlayerField(){

	PlayerField->RespawnField();
}

Mushroomfield&  Player0::GetPlayerField(){

	return *PlayerField;
}

int Player0::GetCurrentWave(){

	return CurrentWave;
}

void Player0::SetCurrentWave(int waveNumber){
	CurrentWave = waveNumber;
}


void Player0::IncrementPlayerCurrentWave(){

	++CurrentWave;
}

void Player0::LowerLivesByOne(){

}

Blaster& Player0::SpawnNewPlayer(){

	return RecyclingBlasterFactory::CreateBlaster(0);
}

void Player0::PlayerInitialize(){

	CurrentScore=0;
	NumberOfLives=3;
	CurrentWave =0;
	PlayerField->Initialize(StartNumberOfMushrooms);

}