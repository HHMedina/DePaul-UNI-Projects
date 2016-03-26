#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H
#include "TEAL\CommonElements.h" 
#include "Controller.h"

//forward class declaration
class Blaster;
class Playercontroller: public Controller{
	
public:

	Playercontroller(Blaster&);
	Playercontroller(Playercontroller&);
	~Playercontroller();

	virtual void KeyPressed (sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);
	virtual void move();

protected:
	Blaster * blaster;

	bool CurrentlyInvincible;
};
#endif PLAYERCONTROLLER_H