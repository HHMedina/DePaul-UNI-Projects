
#ifndef BLASTER_H
#define BLASTER_H
#include "TEAL\CommonElements.h" 

#include <stack>

//class forwarding declarations
class Mushroom;
class BlasterBullet;
class Controller;
class Playercontroller;
class PlayerExplosion;
class Spider;
class Flea;
class CentipedeHeadSegment;
class CentipedeBodySegment;
class Scorpion;

class Blaster: public GameObject{

	//Friend class declarations
	friend class BlasterBullet;
	friend class Playercontroller;
	friend class Controller;
	friend class Spider;
	friend class AttracterMode;

public:
	//0 = AI
	//1 = Keyboard Controls
	Blaster(int typeOfBlaster);

	Blaster(Blaster&);
	~Blaster();

	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	
	void SetCanShootOn();
	bool GetCanShoot();
	sf::Vector2f GetPosition();
	virtual void Collision( GameObject *other ){};
	virtual void Collision( Mushroom *other);
	virtual void Collision( Spider *other);
	virtual void Collision( Flea *other);
	virtual void Collision( CentipedeHeadSegment* other);
	virtual void Collision( CentipedeBodySegment* other);
	virtual void Collision( Scorpion * other);

	void SetBlasterToInvincible();
	//movement members
	int Speed;
	float Offset;
	
	void Initialize(const int type);
	void DestroyBullet();
	
protected:
	

	//consts
	static const int MaxNumberOfBullets;
	sf::Vector2f LastPosition;
	
	sf::Vector2f Position;
	sf::Vector2f BulletSpawn; 
	float BottomBorder;
	float TopBorder;
	
	bool canShoot;
	bool IsInvincible;

	//pointer to hold currentController
	Controller* controller;

	//Controllers Available
	Controller* KeyBoard;
	Controller* AI;

	BlasterBullet* BulletFired;
	//stack to hold bullets Ship is responsible for
	//std::stack<BlasterBullet*> bullets

	AnimatedSprite Sprite;
	CollisionTools::TextureBitmap bitmap;

	static const int MUSHROOMSEGMENT=16;
	static const int BLASTERSEGMENT=16;
};


#endif BLASTER_H