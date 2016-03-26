#ifndef FLOOR_H
#define FLOOR_H

#include "P2Engine.h"
using namespace std;

class Floor:public GameObject{

public:
	
	Floor();
	Floor( const Floor & rhs){ rhs;}
	Floor& operator = (const Floor& rhs){ rhs;}
	virtual ~Floor();
	
	//floor is just a drawable object.
	virtual void Draw();
	virtual void Update();
	Matrix floorScale;
	Vect floorPos;
	Matrix floorRot;
	

};




#endif