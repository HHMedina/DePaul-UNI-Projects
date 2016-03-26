// Asteroid.h
// Andre Berthiaume, March 2013

#ifndef _Asteroid
#define _Asteroid

#include "TEAL\CommonElements.h"

class Bullet; // Note: Forward declare to avoid include-loop since Bullet.h includes Aseroids.h

class Asteroid : public GameObject
{
public:
	Asteroid( sf::Vector2f p );
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();

	virtual void Collision( GameObject *go){};
	virtual void Collision( Bullet *other );

	void Explode();


private:
	sf::Vector2f Pos;
	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap* bitmap;

	float scale;
	int health;
	int MAXHEALTH;
	float SPEED;
};

#endif _Asteroid