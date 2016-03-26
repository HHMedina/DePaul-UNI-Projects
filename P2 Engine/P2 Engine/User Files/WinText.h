#ifndef WINTEXT_H
#define WINTEXT_H

#include "P2Engine.h"
using namespace std;

class WinText:public GameObject{

public:
	
	WinText();
	WinText( const WinText & rhs){ rhs;}
	WinText& operator = (const WinText& rhs){ rhs;}
	virtual ~WinText();
	
	//WinText is just a drawable object.
	virtual void Draw();
	
	Matrix winScale;
	Vect winPos;
	Matrix winRot;
	

};




#endif