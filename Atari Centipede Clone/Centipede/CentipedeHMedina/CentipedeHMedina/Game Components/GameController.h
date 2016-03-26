#ifndef GAMECONTROLLER
#define GAMECONTROLLER

#include "TEAL\CommonElements.h"

class Level1;
class GameController
{

public:
	static GameController* instance;
	GameController();
	static GameController& Instance(){
		if(! instance){
			instance = new GameController();
			return *instance;
		}
	}

	static void ResetLevel();
private:
	
	GameController( const GameController&){}
	GameController& operator=(const GameController&){}
	~GameController(){}
	 
};

#endif GAMECONTROLLER