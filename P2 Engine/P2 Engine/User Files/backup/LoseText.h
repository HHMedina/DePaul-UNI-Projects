#ifndef LOSETEXT_H
#define LOSETEXT_H

#include "P2Engine.h"
using namespace std;

class LoseText:public GameObject{

public:
	
	LoseText();
	LoseText( const LoseText & rhs){ rhs;}
	LoseText& operator = (const LoseText& rhs){ rhs;}
	virtual ~LoseText();
	
	//WinText is just a drawable object.
	virtual void Draw();
	
	Matrix winScale;
	Vect winPos;
	Matrix winRot;
	

};




#endif