#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "TEAL\CommonElements.h" 
//forward class declaration
class Blaster;
class Controller:public GameObject{
public:

	Controller(Blaster&);
	Controller(Controller&);
	Controller(/*no dynamically allocated objects to destroy*/){}
	virtual void move()=0;
protected:
	Blaster * blaster;
};
#endif CONTROLLER_H