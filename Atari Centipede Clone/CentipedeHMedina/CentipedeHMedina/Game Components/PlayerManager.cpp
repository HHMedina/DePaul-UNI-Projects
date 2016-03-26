#include "PlayerManager.h"
#include "Player.h"
#include "Player0.h"
#include "Player1.h"
#include "Player2.h"
#include "Blaster.h"

#include "PlayerExplosion.h"

PlayerManager * PlayerManager::instance =NULL;

PlayerManager::PlayerManager(){
	//create players
	player0 = new Player0();
    player1 = new Player1();
	player2 = new Player2();

	blasterExplosion =NULL;
	CurrentPlayer=player0; // game starts out with player 0;
	shooter=NULL;
}

PlayerManager::~PlayerManager(){

	//delete Dynamically allocated memory
	delete player1;
	delete player2;
	delete player0;


}

void PlayerManager::AddToCurrentScore(int value){

	Instance().CurrentPlayer->AddToPlayerScore(value);

}

Player* PlayerManager::GetPlayer(){

	return Instance().CurrentPlayer;

}

void PlayerManager::SwapPlayers(){

	if(Instance().CurrentPlayer==Instance().player1){ // if player 1 switch to player 2
		Instance().CurrentPlayer=Instance().player2;
	}else if(Instance().CurrentPlayer==Instance().player2){ // if player 2 switch to player 1
		Instance().CurrentPlayer=Instance().player1;
	}

}

void PlayerManager::LowerLivesByOne(){
	Instance().CurrentPlayer->LowerLivesByOne();
}

void PlayerManager::ChangePlayerToPlayer1(){
	Instance().CurrentPlayer = Instance().player1;
}

void PlayerManager::ChangePlayerToPlayer0(){
	Instance().CurrentPlayer = Instance().player0;
}

void PlayerManager::SpawnNewPlayer(){

	
	Instance().shooter = &Instance().CurrentPlayer->SpawnNewPlayer();
	

}

void PlayerManager::DestroyBlaster(){

	if(Instance().shooter!=NULL){
		Instance().shooter->MarkForDestroy();
		Instance().shooter=NULL;
	}

}

bool PlayerManager::ThereIsABlaster(){

	if(Instance().shooter!=NULL){
		return true;
	}

	return false;
}

void PlayerManager::SetPlayerExplosion(PlayerExplosion& set){

	Instance().blasterExplosion = &set;

}

void PlayerManager::DestroyPlayerExplosion(){// used to clean up when switchign modes
	
	if(Instance().blasterExplosion!=NULL){
		Instance().blasterExplosion->MarkForDestroy();
		Instance().blasterExplosion=NULL;
	}

}

void PlayerManager::Player0Initialize(){
	Instance().player0->PlayerInitialize();
}

void PlayerManager::Player1Initialize(){
	Instance().player1->PlayerInitialize();

}

void PlayerManager::Player2Initialize(){
	Instance().player2->PlayerInitialize();

}

Player* PlayerManager::GetPlayer2Pointer(){

	return Instance().player2;
}

bool PlayerManager::BothPlayersAreDead(){

	if(Instance().player1->GetPlayerLives()==0&&Instance().player2->GetPlayerLives()==0){ // if both lives are value 0
		return true; //return true (both are dead)
	}
	return false; //if not then return both are not dead
}

void PlayerManager::DestroyBlasterBullet(){

	if(Instance().shooter!=NULL){
		Instance().shooter->DestroyBullet();//destroys bullet if there is one //used for mode switching clean up
	}

}