#include "GameController.h"
#include "Bomb.h"
#include "SceneManager.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "RecyclingBombFactory.h"

GameController* GameController::instance =NULL;

void GameController::DeleteMe(){
	delete instance;
	instance=NULL;
}

void GameController::SpawnRandomBomb(){
	
	if(!Instance()->playerWonAlready){//dont spawn another bomb if the player won already.
		RecyclingBombFactory::CreateBomb(rand()%randomBoxLength,0,rand()%randomBoxLength);
	}
}

void GameController::GoToWinScreen(){

	//changes scene to the winning screen.
	SceneManager::ChangeScene(new WinScene() );
}

void GameController::GoToLoseScreen(){

	//changes to the losing screen
	SceneManager::ChangeScene( new LoseScene() );
}

void GameController::IncrementBombsDiffused(){

	++Instance()->bombsDiffused;

	//check to see if the player has won.
	if(Instance()->bombsDiffused >= Instance()->diffusesNeededToWin){
		Instance()->playerWonAlready =true;
		Instance()->GoToWinScreen();
	}
}