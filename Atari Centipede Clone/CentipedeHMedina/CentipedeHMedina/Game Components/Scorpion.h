
#ifndef SCORPION_H
#define SCORPION_H


#include "TEAL\CommonElements.h"
#include "PositionAndRotationOffset.h"
#include <queue>

class Mushroomfield;
class Mushroom;
class BlasterBullet;
class Blaster;
class Scorpion:public GameObject{

	friend class RecyclingScorpionFactory;

public:
	
	Scorpion(const sf::Vector2f SpawnPosition, const float direction,Mushroomfield&);
	Scorpion(const Scorpion&){}//Prevent Copy constructor
	~Scorpion(){}//no Dynamically allocated object members to delete

	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void Collision(BlasterBullet * other);
	virtual void Collision(Blaster * other);

	static const int ScoreValue= 1000;
	
private:
	
	//Used for Visual representation and Collision
	AnimatedSprite Sprite;
	CollisionTools::TextureBitmap bitmap;

	//Position of the Scorpion
	sf::Vector2f WorldPosition;
	Mushroomfield * FatherMushroomfield;

	//speed and the direction it is going
	float Speed;
	int Direction;

	//used to initialize via Factory
	void Initialize(sf::Vector2f,const int direction, Mushroomfield&);

	//Used to Hold precomputed Horizontal motion
	std::queue<PositionAndRotationOffset> HorizontalQueue;

	//holds current job
	std::queue<PositionAndRotationOffset> CurrentQueue;
	//Consts
	static const int MUSHROOMSEGMENT =16;
	static const int FACERIGHT = -1;
	static const int FACELEFT = 1;
	static const int ScorpionSpeed =2;
};





#endif SCORPION_H
