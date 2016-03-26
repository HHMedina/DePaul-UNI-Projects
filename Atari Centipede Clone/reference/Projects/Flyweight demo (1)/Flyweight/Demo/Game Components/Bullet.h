// Bullet
// Andre Berthiaume, Aug 2012

#ifndef _Bullet
#define _Bullet

#include "TEAL\CommonElements.h"

//Forward declarations
class Asteroid;

class Bullet : public GameObject
{
public:
	Bullet(sf::Vector2f p);

	void Initialize(sf::Vector2f p); 

	virtual void Update();
	virtual void Draw();
	virtual void Destroy();

	virtual void Collision( GameObject *go){};
	virtual void Collision( Asteroid *other );


private:
	float SPEED;

	sf::Vector2f Pos;
	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;
	
};

#endif _Bullet