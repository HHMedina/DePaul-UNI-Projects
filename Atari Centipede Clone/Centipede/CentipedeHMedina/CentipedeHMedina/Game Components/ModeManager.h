#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include "TEAL\CommonElements.h"

class Mode;

class ModeManager: public GameObject{

private:
	static ModeManager* instance;
	ModeManager();
	ModeManager(const ModeManager&){}
	ModeManager& operator =(const ModeManager& other){}
	~ModeManager();

	static ModeManager& Instance(){
		if(!instance){
			instance = new ModeManager();
		}
		return *instance;
	}

	Mode * currentMode;
	Mode * attractorMode;
	Mode * onePlayerMode;
	Mode * twoPlayerMode;

public:
	
	virtual void KeyPressed (sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);
	virtual void Destroy();
	//starts the game in attractor mode
	static void StartUp();

	//Clean up the current Mode... Set the mode to attractorMode and instantiate
	//attractor mode.. Also reset current wave for each player and current score and Lives count
	static void SetModeToAttractor();

	//Clean up the current Mode... Set the mode to PlayMode and instantiate
	//PlayMode mode
	static void SetModeToOnePlayerMode();
	static void SetModeToTwoPlayerMode();

	//mode dictates what happens a the end of a wave
	static void EndOfWaveDecision();

	static void PressEnterOnHighScoreAction();


};
#endif MODEMANAGER_H