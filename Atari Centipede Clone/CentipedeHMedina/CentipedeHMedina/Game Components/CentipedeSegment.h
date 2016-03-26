#ifndef CENTIPEDESEGMENT_H
#define CENTIPEDESEGMENT_H
#include "TEAL\CommonElements.h"
#include <stack>
#include "PositionAndRotationOffset.h"
#include "Mushroomfield.h"

class BlasterBullet;
class CentipedeSegment: public GameObject{
	friend class CentipedeFactory;
	friend class CentipedeHeadSegment;
	friend class CentipedeBodySegment;
public:
	CentipedeSegment(){}
	CentipedeSegment(CentipedeSegment&){}
	~CentipedeSegment(){}

	//gameObject methods
	virtual void Update()=0;
    virtual void Draw()=0;
	virtual void Destroy()=0;
	virtual void Collision( GameObject *other ){};
	virtual void Collision( BlasterBullet *other)=0;

private:
	//vectors for position of sprite and direction
	sf::Vector2f WorldPosition;
	sf::Vector2f Direction;
	

	//previous and next Segments
	CentipedeSegment * Next;
	CentipedeSegment * Previous;

	//stacks used to hold the job queues
	std::queue<PositionAndRotationOffset> CurrentQueue;
	std::queue<PositionAndRotationOffset> FutureQueue;
	
	//The mushroom field pointer
	Mushroomfield* Field;

	//const ints for definitions of magic numbers
	static const int MUSHROOMSEGMENT =16;
	static const int FACINGRIGHT = 180;
	static const int FACINGLEFT = 0;

	//sprite and bitmap(collisions)
	AnimatedSprite Sprite;
	CollisionTools::TextureBitmap bitmap;

	//speed definition
	int Speed;

};


#endif CENTIPEDESEGMENT_H