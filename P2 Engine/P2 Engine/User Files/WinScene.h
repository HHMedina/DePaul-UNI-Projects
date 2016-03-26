#ifndef WINSCENE_H
#define WINSCENE_H

#include <P2Engine.h>
using namespace std;

class WinScene: public Scene{


public:

	WinScene(){}
	WinScene( const WinScene & rhs){ rhs;}
	WinScene& operator = (const WinScene& rhs){ rhs;return *this;}
	~WinScene(){
	}

	virtual void SetUp();

};
#endif