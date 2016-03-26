#include "ModeManager.h"
#include "HUD.h"
#include "WaveManager.h"

#include "PlayerManager.h"
#include "Player.h"
#include "Player1.h"
#include "Player2.h"
#include "SoundManager.h"

#include "Mode.h"
#include "PlayerOneMode.h"
#include "PlayerTwoMode.h"
#include "AttractionMode.h"
#include "TEAL\CommonElements.h"

ModeManager* ModeManager::instance=NULL;

ModeManager::ModeManager(){

	attractorMode = new AttractionMode();
	onePlayerMode = new PlayerOneMode();
	twoPlayerMode = new PlayerTwoMode();

	//RegisterInput(InputFlags::KeyPressed);
}

void ModeManager::StartUp(){
	Instance().SetModeToAttractor();

}

ModeManager::~ModeManager(){
	delete attractorMode;
	delete onePlayerMode;
	delete twoPlayerMode;
	delete currentMode;
	DeregisterInput();
}

void ModeManager::SetModeToAttractor(){

	Instance().RegisterInput(InputFlags::KeyPressed);// allow input for mode manager in attractor mode
	Instance().currentMode=Instance().attractorMode;

	PlayerManager::Player0Initialize();
	PlayerManager::Player1Initialize();
	PlayerManager::Player2Initialize();
	PlayerManager::DestroyBlasterBullet();
	PlayerManager::DestroyBlaster();//destroys the blaser on field (if there is one)
	PlayerManager::DestroyPlayerExplosion();//destroys player explosion if there is one
	WaveManager::DestroyAllWaveEntities();

	PlayerManager::ChangePlayerToPlayer0();
	SoundManager::SetToSilentMode();
	PlayerManager::LowerLivesByOne();
	PlayerManager::SpawnNewPlayer();
	
	HUD::Initialize();
	PlayerManager::GetPlayer()->RespawnPlayerField();
	HUD::ShowHighScores();
	WaveManager::StartUp();
	
	HUD::RemovePlayer1LivesDisplay();
	HUD::RemovePlayer2LivesDisplay();
	
	
}


void ModeManager::SetModeToOnePlayerMode(){

	Instance().DeregisterInput();//do not allow input for mode manager in player one mode

	//Clean up attractor mode stuff
	PlayerManager::DestroyBlasterBullet();
	PlayerManager::DestroyBlaster();//destroys the blaser on field (if there is one)
	PlayerManager::DestroyPlayerExplosion();//destroys player explosion if there is one
	HUD::RemoveHighScoresList();//MUST GO FIRST!
	PlayerManager::GetPlayer()->ClearPlayerField();//MUST GO AFTER HUD::RemoveHighScoresList()//removes the attracotr (player 0 mushrooms)
	WaveManager::DestroyAllWaveEntities();
	
	//Set to real player not dummy player
	Instance().currentMode=Instance().onePlayerMode;

	//set up what Player one Mode Means
	PlayerManager::ChangePlayerToPlayer1();
	PlayerManager::Player1Initialize(); // initialize player 1 to initial stats
	SoundManager::SetToAudibleMode();

	//spawn a new player
	PlayerManager::LowerLivesByOne();
	PlayerManager::SpawnNewPlayer();
	PlayerManager::GetPlayer()->RespawnPlayerField();

	//initialize HUD
	HUD::Initialize();

	//load player wave
	WaveManager::LoadPlayerWave();

	//remove non one player mode elements
	HUD::RemovePlayer2LivesDisplay();
	HUD::RemovePlayer2ScoreDisplay();

}

void ModeManager::SetModeToTwoPlayerMode(){

	Instance().DeregisterInput();//do not allow input for mode manager in player one mode

	//Clean up attractor mode stuff
	PlayerManager::DestroyBlasterBullet();
	PlayerManager::DestroyBlaster();//destroys the blaser on field (if there is one)
	PlayerManager::DestroyPlayerExplosion();//destroys player explosion if there is one
	HUD::RemoveHighScoresList();//MUST GO FIRST!
	PlayerManager::GetPlayer()->ClearPlayerField();//MUST GO AFTER HUD::RemoveHighScoresList()
	WaveManager::DestroyAllWaveEntities();

	Instance().currentMode=Instance().twoPlayerMode;

	PlayerManager::ChangePlayerToPlayer1();//start as player 1
	//set all scores and lives to initial value
	PlayerManager::Player1Initialize();
	PlayerManager::Player2Initialize();

	//make sounds audible
	SoundManager::SetToAudibleMode();

	//spawn a player with his or her field
	PlayerManager::LowerLivesByOne();
	PlayerManager::SpawnNewPlayer();
	PlayerManager::GetPlayer()->RespawnPlayerField();

	//start HUD
	HUD::Initialize();
	WaveManager::LoadPlayerWave();//load the players wave (first wave)

}

void ModeManager::EndOfWaveDecision(){

	Instance().currentMode->EndOfWaveDecision();
}


void ModeManager::KeyPressed (sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey){

	

	if(k==sf::Keyboard::Num1){//if one is pressed
		Instance().currentMode->ActionOnKeyPressedForNum1(); //does action corresponding to mode

	}

	if(k==sf::Keyboard::Num2){
		Instance().currentMode->ActionOnKeyPressedForNum2(); //does action corresponding to mode

	}


}

void ModeManager::Destroy(){
	DeregisterInput();
}

void ModeManager::PressEnterOnHighScoreAction(){
	Instance().currentMode->PressEnterOnHighScoreAction(); //does action corresponding to mode
}