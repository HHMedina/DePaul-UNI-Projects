#ifndef ATTRACTERMODE_H
#define ATTRACTERMODE_H
#include "TEAL\CommonElements.h" 
#include "Controller.h"
#include <queue>
#include "PositionAndRotationOffset.h"

//forward class declaration
class Blaster;
class AttracterMode: public Controller{
	
public:

	AttracterMode(Blaster&);
	AttracterMode(AttracterMode&);
	~AttracterMode();//no dynamically allocated objects to destroy

	virtual void move();
	virtual void Alarm0();
	virtual void Alarm1();

protected:

	//used to hold movement definitions
	std::queue<PositionAndRotationOffset> StopQueue;
	std::queue<PositionAndRotationOffset> DiagonalQueue;

	//holds the current job that will be repeated x amount of times.
	//This allows long diagonal and long downward and upward motion
	//This also allows the checking during each queue process of the cycle
	//to check if the spider has reached the top border or the bottom border
	//and stop the current cycle, change direction and continue from there
	std::queue<std::queue<PositionAndRotationOffset>> CurrentQueue;

	sf::Vector2f Direction;

	bool IsOnBorder;
	bool CanMove;

	//consts
	static const int AttractModeSpeed = 2;
	static const int MUSHROOMSEGMENT =16;
	static const int NumberOfSpeedOffsetsToPutInDiagonalQueue =24;
	static const float TimeInbetweenMovingAndPausing;

	Blaster * blaster;
};
#endif ATTRACTERMODE_H
