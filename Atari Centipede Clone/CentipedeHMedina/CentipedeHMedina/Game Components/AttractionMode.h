#ifndef ATTRACTIONMODE_H
#define ATTRACTIONMODE_H
#include "Mode.h"

class AttractionMode: public Mode{

public:

	AttractionMode(){}
	AttractionMode(const AttractionMode&){}
	AttractionMode& operator =(const AttractionMode& ){}
	~AttractionMode(){};

	virtual void EndOfWaveDecision();
	virtual void ActionOnKeyPressedForNum1();
	virtual void ActionOnKeyPressedForNum2();

	virtual void PressEnterOnHighScoreAction();

};
#endif PLAYERONEMODE_H