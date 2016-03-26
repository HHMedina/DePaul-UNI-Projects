#include "PlayerTwoMode.h"
#include "PlayerManager.h"
#include "WaveManager.h"
#include "Player.h"
#include "Player0.h"
#include "Player1.h"
#include "Player2.h"
#include "ModeManager.h"
#include "HighScoreInput.h"

void PlayerTwoMode::EndOfWaveDecision(){

	
	if(PlayerManager::GetPlayer()->GetPlayerLives()==0){ // checks to see if current player is dead
		ConsoleMsg::WriteLine("Game Over");
		if(WaveManager::IsScoreANewHighScore()){//if score is a new high score

			HighScoreInput::StartInput();//initiate highscore input
			PlayerManager::GetPlayer()->ClearPlayerField();
			WaveManager::DestroyAllWaveEntities();
			WaveManager::DisableSpiderSpawn();
			CurrentlyInHighScore =true;

		}else if(PlayerManager::BothPlayersAreDead()&&!CurrentlyInHighScore){ //if both players are dead and player is not in a highscore
			PlayerManager::GetPlayer()->ClearPlayerField();
			WaveManager::DestroyAllWaveEntities();
			WaveManager::DisableSpiderSpawn();
			PlayerManager::GetPlayer()->UpdatePlayerLivesRemaining();// if both players are dead update player 2 lives. (last player)
			ModeManager::SetModeToAttractor();
			ItIsGameOver =true;
		}
	}

	if(!AreBothPlayersDead&&!CurrentlyInHighScore&&!ItIsGameOver){//if both players are dead/ attractor mode as been activated.. no need for anymore player processing
		PlayerManager::GetPlayer()->ClearPlayerField();// clear the previous player's field
	
		PlayerManager::SwapPlayers();
		WaveManager::LoadPlayerWave();//load the new players current wave
		PlayerManager::GetPlayer()->RespawnPlayerField();//respawn new players field

		if(PlayerManager::GetPlayer()->GetPlayerLives()>0){
			PlayerManager::SpawnNewPlayer();
			PlayerManager::LowerLivesByOne();	
		}

	}
	AreBothPlayersDead =false;//set back to not dead (so regular wave checking can occur)
	CurrentlyInHighScore=false;//set back to false (so regular wave checking can occur)
	ItIsGameOver =false;
	PlayerManager::GetPlayer()->UpdatePlayerLivesRemaining();
}

void PlayerTwoMode::ActionOnKeyPressedForNum1(){
}


void PlayerTwoMode::ActionOnKeyPressedForNum2(){

}

void PlayerTwoMode::PressEnterOnHighScoreAction(){

	if(PlayerManager::BothPlayersAreDead()){

		ModeManager::SetModeToAttractor();

	}else{
	
		PlayerManager::SwapPlayers();
		WaveManager::LoadPlayerWave();//load the new players current wave
		PlayerManager::GetPlayer()->RespawnPlayerField();//respawn new players field

		if(PlayerManager::GetPlayer()->GetPlayerLives()>0){
			PlayerManager::SpawnNewPlayer();
			PlayerManager::LowerLivesByOne();
			PlayerManager::GetPlayer()->UpdatePlayerLivesRemaining();
		}

	}

}