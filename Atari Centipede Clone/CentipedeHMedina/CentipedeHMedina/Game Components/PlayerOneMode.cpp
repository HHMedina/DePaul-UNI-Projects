#include "PlayerOneMode.h"
#include "WaveManager.h"
#include "ModeManager.h"

#include "PlayerManager.h"
#include "Player.h"
#include "Player0.h"
#include "Player1.h"
#include "Player2.h"

#include "HighScoreInput.h"

void PlayerOneMode::EndOfWaveDecision(){

	WaveManager::LoadPlayerWave();

	if(PlayerManager::GetPlayer()->GetPlayerLives()>=0){
		if(PlayerManager::GetPlayer()->GetPlayerLives()>0){

			PlayerManager::SpawnNewPlayer();
			PlayerManager::LowerLivesByOne();	
		}else{
			//it is game over ..clean up the scene
			PlayerManager::GetPlayer()->ClearPlayerField();
			WaveManager::DestroyAllWaveEntities();
			WaveManager::DisableSpiderSpawn();

			if(WaveManager::IsScoreANewHighScore()){//if score is a new high score
				HighScoreInput::StartInput();//initiate highscore input
			}else{
				ModeManager::SetModeToAttractor();//go to attractor mode
			}
		}
	}
	PlayerManager::GetPlayer()->UpdatePlayerLivesRemaining();

}

void PlayerOneMode::ActionOnKeyPressedForNum1(){
}


void PlayerOneMode::ActionOnKeyPressedForNum2(){

	
	

}

void PlayerOneMode::PressEnterOnHighScoreAction(){

	ModeManager::SetModeToAttractor();

}