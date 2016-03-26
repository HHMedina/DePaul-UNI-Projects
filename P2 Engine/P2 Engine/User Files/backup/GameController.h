#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H


#include "P2Engine.h"


//class Bucket;
class GameController{

public:
	
	static void IncrementBombsDiffused();
	
	static void SpawnRandomBomb();

	static void GoToWinScreen();

	static void GoToLoseScreen();

	static void DeleteMe();
	
private:
	static GameController * instance;//This is a singleton

	static GameController* Instance(){
		if(instance==nullptr){
			instance= new GameController();
		}
		return instance;

	}

	GameController(){bombsDiffused=0;playerWonAlready=false; };
	GameController( const GameController & rhs){ rhs;}
	GameController& operator = (const GameController& rhs){ rhs;}
	~GameController(){}

	static const int randomBoxLength = 180; 
	int bombsDiffused;
	static const int diffusesNeededToWin = 8;
	bool playerWonAlready;
};




#endif