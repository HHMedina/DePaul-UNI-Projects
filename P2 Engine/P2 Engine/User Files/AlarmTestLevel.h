#ifndef ALARMTESTLEVEL_H
#define ALAEMTESTLEVEL_H

#include <P2Engine.h>
using namespace std;


class AlarmTestLevel: public Scene{


public:

	AlarmTestLevel(){}
	AlarmTestLevel( const AlarmTestLevel & rhs){ rhs;}
	AlarmTestLevel& operator = (const AlarmTestLevel& rhs){ rhs;return *this;}
	~AlarmTestLevel(){
	}

	virtual void SetUp();
	virtual void OnSceneEnd();


};
#endif