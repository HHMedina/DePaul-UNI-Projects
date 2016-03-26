#ifndef LEVEL2_H
#define LEVEL2_H

#include <P2Engine.h>
using namespace std;

class Level2: public Scene{


public:

	Level2(){}
	Level2( const Level2 & rhs){ rhs;}
	Level2& operator = (const Level2& rhs){ rhs; return *this;}
	~Level2(){
	}

	virtual void SetUp();

};
#endif