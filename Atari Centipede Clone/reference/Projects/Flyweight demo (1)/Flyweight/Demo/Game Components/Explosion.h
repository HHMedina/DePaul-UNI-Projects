// Explosion
// Andre Berthiaume, March 2013

#ifndef _Explosion
#define _Explosion

#include "TEAL\CommonElements.h"

class Explosion : public GameObject
{
public:
	Explosion( sf::Vector2f pos, float speed );
	virtual void Update();
	virtual void Draw();

private:
	AnimatedSprite MainSprite;
	sf::Sound kaboom;

	sf::Vector2f Pos;
	float Speed;
};


#endif _Explosion