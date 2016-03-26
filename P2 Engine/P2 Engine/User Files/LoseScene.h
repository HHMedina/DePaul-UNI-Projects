#ifndef LOSESCENE_H
#define LOSESCENE_H

#include <P2Engine.h>
using namespace std;

class LoseScene: public Scene{


public:

	LoseScene(){}
	LoseScene( const LoseScene & rhs){ rhs;}
	LoseScene& operator = (const LoseScene& rhs){ rhs;return *this;}
	~LoseScene(){
	}

	virtual void SetUp();

};
#endif