#ifndef MODE_H
#define MODE_H

class Mode{

public:
	virtual void EndOfWaveDecision()=0;

	virtual void ActionOnKeyPressedForNum1()=0;
	virtual void ActionOnKeyPressedForNum2()=0;
	virtual void PressEnterOnHighScoreAction()=0;

};
#endif MODE_H