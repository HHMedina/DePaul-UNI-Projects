#ifndef CENTIPEDEHEADSEGMENT_H
#define CENTIPEDEHEADSEGMENT_H

#include "PositionAndRotationOffset.h"
#include <queue>
#include "CentipedeSegment.h"
#include "TEAL\CommonElements.h"

class Mushroomfield;
class Blaster;

class CentipedeHeadSegment: public CentipedeSegment{
	friend class CentipedeFactory;
	friend class RecyclingCHeadSegmentFactory;
	friend class CentipedeBodySegment;
public:
	CentipedeHeadSegment(const sf::Vector2f WorldPosition,const int speed, Mushroomfield& FatherField, const sf::Vector2f InitialDirection);

	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void Collision( BlasterBullet *other);
	virtual void Collision( Blaster* other);

	static void ResetNumberOfInstances();

	static const int ScoreValue= 100;

private:
	//initializer used for factory
	virtual void Initialize(const sf::Vector2f WorldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection);


	//used to hold precomputed movements
	std::queue<PositionAndRotationOffset> TurnOffsetQueue;
	std::queue<PositionAndRotationOffset> HorizontalOffsetQueue;

	//used to tell if was turning (to change direction)
	bool IsTurning;
	bool HasTouchedBottom;
	bool IsPoisonedState;

	float TopBlasterBorder;
	float BottomBlasterBorder;
	int TurningNumber;

	static int NumberOfInstances;
	int TagNumber;



};


#endif CENTIPEDEHEADSEGMENT_H