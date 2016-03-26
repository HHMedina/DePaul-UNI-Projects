#ifndef DEATHMATCHLEVEL_H
#define DEATHMATCHLEVEL_H

#include <P2Engine.h>
using namespace std;

class DesertFloor;
class DeathMatchLevel: public Scene{


public:

	DeathMatchLevel(){}
	DeathMatchLevel( const DeathMatchLevel & rhs){ rhs;}
	DeathMatchLevel& operator = (const DeathMatchLevel& rhs){ rhs;return *this;}
	~DeathMatchLevel(){
	}

	virtual void SetUp();
	virtual void OnSceneEnd();

	//pointer members here to allow for calling mark on destroy explicitly
	DesertFloor* desertFloor;
};
#endif