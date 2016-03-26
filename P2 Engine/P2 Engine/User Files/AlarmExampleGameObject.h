#ifndef ALARMEXAMPLEGAMEOBJECT_H
#define ALARMEXAMPLEGAMEOBJECT_H

#include "P2Engine.h"
using namespace std;

class AlarmExampleGameObject:public GameObject{

public:
	
	AlarmExampleGameObject();
	AlarmExampleGameObject( const  AlarmExampleGameObject & rhs){ rhs;}
	AlarmExampleGameObject& operator = (const  AlarmExampleGameObject& rhs){ rhs;}
	virtual ~AlarmExampleGameObject();
	
	virtual void KeyDown(AZUL_KEY key);
	virtual void KeyUp(AZUL_KEY key);
	virtual void Update(); 
	//virtual void Draw();

	virtual void Alarm0();
	virtual void OnTerminate();
	
	
private:
	static const int INITIALTIME =10;

};

#endif