#ifndef SPIDER_H
#define SPIDER_H


#include "TEAL\CommonElements.h"
#include "PositionAndRotationOffset.h"
#include <queue>
#include  <stdlib.h>
#include <time.h>

class Mushroomfield;
class BlasterBullet;
class Blaster;

class Spider:public GameObject{

	friend class RecyclingSpiderFactory;

public:
	
	Spider(const sf::Vector2f SpawnPosition,const float speed,const sf::Vector2i InitialDirection,Mushroomfield&);
	Spider(const Spider&){/*Prevent Copy constructor*/}
	~Spider(){/*no Dynamically allocated object members to delete*/}

	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void Collision( GameObject *other ){};
	virtual void Collision( BlasterBullet *other );
	virtual void Collision( Blaster *other );

	//definition of scores
	static const int CloseScoreValue= 900;
	static const int MediumScoreValue= 600;
	static const int FarScoreValue= 300;

	//definition of Distances
	static const int CloseDistance = 64;
	static const int MediumDistance = 100;



private:
	
	//Used for Visual representation and Collision
	AnimatedSprite Sprite;
	CollisionTools::TextureBitmap bitmap;

	//Position of the Spider
	sf::Vector2f WorldPosition;

	Mushroomfield * FatherMushroomfield;

	float Speed;
	sf::Vector2i Direction;

	//used to initialize via Factory
	void Initialize(sf::Vector2f SpawnPosition,const float speed,const sf::Vector2i InitialDirection, Mushroomfield&);

	//Used to Hold precomputed Downward motions
	std::queue<PositionAndRotationOffset> DownwardQueue;
	std::queue<PositionAndRotationOffset> DiagonalQueue;

	
	//holds current job
	//std::queue<PositionAndRotationOffset> CurrentQueue;

	//holds the current job that will be repeated x amount of times.
	//This allows long diagonal and long downward and upward motion
	//This also allows the checking during each queue process of the cycle
	//to check if the spider has reached the top border or the bottom border
	//and stop the current cycle, change direction and continue from there
	std::queue<std::queue<PositionAndRotationOffset>> CurrentQueue;
	//Consts
	static const int MUSHROOMSEGMENT =16;
	float TOPBORDER;
	float BottomBlasterBorder;
	static const int SPIDERSIZEX = 32;

	int HP;

	//bool to signal if on border
	bool IsOnBorder;

	


};





#endif SPIDER_H