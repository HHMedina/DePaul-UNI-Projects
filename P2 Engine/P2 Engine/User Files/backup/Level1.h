#ifndef LEVEL1_H
#define LEVEL1_H

#include <P2Engine.h>
using namespace std;

class Level1: public Scene{


public:

	Level1(){}
	Level1( const Level1 & rhs){ rhs;}
	Level1& operator = (const Level1& rhs){ rhs;return *this;}
	~Level1(){
	}

	virtual void SetUp();
	virtual void OnSceneEnd();
};
#endif