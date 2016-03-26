#ifndef PLAYERONEMODE_H
#define PLAYERONEMODE_H
#include "Mode.h"

class PlayerOneMode: public Mode{

public:

	PlayerOneMode(){}
	PlayerOneMode(const PlayerOneMode&){}
	PlayerOneMode& operator =(const PlayerOneMode& ){}
	~PlayerOneMode(){};

	virtual void EndOfWaveDecision();
	virtual void ActionOnKeyPressedForNum1();
	virtual void ActionOnKeyPressedForNum2();
	virtual void PressEnterOnHighScoreAction();


};
#endif PLAYERONEMODE_H