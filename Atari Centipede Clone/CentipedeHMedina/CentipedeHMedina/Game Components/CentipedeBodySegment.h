#ifndef CENTIPEDEBODYSEGMENT_H
#define CENTIPEDEBODYSEGMENT_H

#include <queue>
#include "TEAL\CommonElements.h"
#include "CentipedeSegment.h"
#include "PositionAndRotationOffset.h"

class Mushroomfield;
class Blaster;
class CentipedeBodySegment: public CentipedeSegment{
	friend class CentipedeFactory;
	friend class RecyclingCBodySegmentFactory;

public:
	CentipedeBodySegment(sf::Vector2f WorldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection);

	virtual void Update();
	virtual void Draw();
	virtual void Destroy();

	//GameObject Collision Methods
	virtual void Collision( GameObject *other ){};
	virtual void Collision( BlasterBullet *other);
	virtual void Collision( Blaster* other );
	static const int ScoreValue= 10;
		
private:
	//initializer used for factory
	virtual void Initialize(const sf::Vector2f worldPosition,const int Speed,Mushroomfield&,const sf::Vector2f InitialDirection);
	
	
};


#endif CENTIPEDEBODYSEGMENT_H