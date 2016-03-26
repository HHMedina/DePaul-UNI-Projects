#ifndef SOUNDTESTLEVEL_H
#define SOUNDTESTLEVEL_H

#include <P2Engine.h>
class SoundTestLevel: public Scene{


public:

	SoundTestLevel(){}
	SoundTestLevel( const SoundTestLevel & rhs){ rhs;}
	SoundTestLevel& operator = (const SoundTestLevel& rhs){ rhs;return *this;}
	~SoundTestLevel(){
	}

	virtual void SetUp();
	virtual void OnSceneEnd();

};
#endif