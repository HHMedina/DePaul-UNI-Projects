#include "GameController.h"
#include "Level1.h"

GameController* GameController::instance;

void GameController::ResetLevel(){
	 SceneManager::ChangeScene( new Level1 );

}