#ifndef FLEA_H
#define FLEA_H


#include "TEAL\CommonElements.h"
#include "PositionAndRotationOffset.h"
#include <queue>

class Mushroomfield;
class BlasterBullet;
class Blaster;
class Flea:public GameObject{

	friend class RecyclingFleaFactory;

public:
	
	Flea(const sf::Vector2f SpawnPosition,const float speed,Mushroomfield&);
	Flea(const Flea&){/*Prevent Copy constructor*/}
	~Flea(){/*no Dynamically allocated object members to delete*/}

	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void Collision( BlasterBullet *other );
	virtual void Collision( Blaster *other );

	static const int ScoreValue= 200;
private:
	
	//Used for Visual representation and Collision
	AnimatedSprite Sprite;
	CollisionTools::TextureBitmap bitmap;

	//Position of the Flea
	sf::Vector2f WorldPosition;
	Mushroomfield * FatherMushroomfield;

	float Speed;
	float SpeedAfterBeingHit;
	float SecondSpeedMultiplier;

	//used to initialize via Factory
	void Initialize(sf::Vector2f,const float speed, Mushroomfield&);

	//Used to Hold precomputed Downward motions
	std::queue<PositionAndRotationOffset> DownwardQueue;
	std::queue<PositionAndRotationOffset> SecondSpeedDownwardQueue;

	//holds current job
	std::queue<PositionAndRotationOffset> CurrentQueue;

	//Consts
	static const int MUSHROOMSEGMENT =16;

	//float holding the chance of spawning a mushroom at each checking interval in percentage.
	float MushroomSpawningChance;

	int HP;


	//Bool telling the flea if it can spawn mushrooms - used for disabling spawn after collision with first bullet
	bool CanSpawnMushrooms;

};





#endif FLEA_H