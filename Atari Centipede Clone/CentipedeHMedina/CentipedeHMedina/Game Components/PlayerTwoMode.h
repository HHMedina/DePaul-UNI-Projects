#ifndef PLAYERTWOMODE_H
#define PLAYERTWOMODE_H
#include "Mode.h"

class PlayerTwoMode: public Mode{

public:

	PlayerTwoMode(){ AreBothPlayersDead=false;CurrentlyInHighScore=false;ItIsGameOver=false;}
	PlayerTwoMode(const PlayerTwoMode&){}
	PlayerTwoMode& operator =(const PlayerTwoMode& ){}
	~PlayerTwoMode(){};

	virtual void EndOfWaveDecision();
	virtual void ActionOnKeyPressedForNum1();
	virtual void ActionOnKeyPressedForNum2();

	virtual void PressEnterOnHighScoreAction();

private:

	bool AreBothPlayersDead;
	bool CurrentlyInHighScore;
	bool ItIsGameOver;
};
#endif PLAYERTWOMODE_H