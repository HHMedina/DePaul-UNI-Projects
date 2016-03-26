#ifndef DESERTFLOOR_H
#define DESERTFLOOR_H

#include "P2Engine.h"
using namespace std;

class DesertFloor:public GameObject{

public:
	
	DesertFloor();
	DesertFloor( const DesertFloor & rhs){ rhs;}
	DesertFloor& operator = (const DesertFloor& rhs){ rhs;}
	virtual ~DesertFloor();
	
	//floor is just a drawable object.
	virtual void Draw();
	virtual void Update();
	Matrix floorScale;
	Vect floorPos;
	Matrix floorRot;
	

};




#endif