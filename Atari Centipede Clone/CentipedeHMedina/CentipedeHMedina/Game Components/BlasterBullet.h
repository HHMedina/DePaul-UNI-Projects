#ifndef BlasterBULLET_H
#define BlasterBULLET_H

#include "TEAL\CommonElements.h"

class Blaster;
class Mushroom;
class Spider;
class Flea;
class CentipedeHeadSegment;
class CentipedeBodySegment;
class Scorpion;

class BlasterBullet: public GameObject{

	friend class Playercontroller;
	friend class Spider;
public:
	BlasterBullet(const sf::Vector2f SpawnLocation,Blaster& Spawner);
	BlasterBullet(const BlasterBullet&){};

	void Update();
	void Draw();
	void Destroy();

	virtual void Collision( GameObject *other ){};
	virtual void Collision( Mushroom *other);
	virtual void Collision( Spider *other);
	virtual void Collision( Flea *other);
	virtual void Collision( CentipedeHeadSegment* other);
	virtual void Collision( CentipedeBodySegment* other);
	virtual void Collision( Scorpion * other);

	void Initialize(sf::Vector2f p,Blaster& Spawner);

private:
	
	Blaster * MyShooter;//the shooter of the bullet
	float SPEED;
	sf::Vector2f Position;
	CollisionTools::TextureBitmap bitmap;
	AnimatedSprite BulletSprite;

	bool HasBeenShot; //used for positioning bullet on the blaster
	bool SoundHasPlayed;//used to stop sound from playing at every update
	//consts
	static const int MUSHROOMSEGMENT=16;
};

#endif BlasterBULLET_H