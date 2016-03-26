#include "AttractionMode.h"
#include "WaveManager.h"
#include "PlayerManager.h"
#include "ModeManager.h"

void AttractionMode::EndOfWaveDecision(){

	WaveManager::LoadPlayerWave();
	PlayerManager::SpawnNewPlayer();

}

void AttractionMode::ActionOnKeyPressedForNum1(){
	ModeManager::SetModeToOnePlayerMode();
}


void AttractionMode::ActionOnKeyPressedForNum2(){
	ModeManager::SetModeToTwoPlayerMode();
}

void AttractionMode::PressEnterOnHighScoreAction(){

}