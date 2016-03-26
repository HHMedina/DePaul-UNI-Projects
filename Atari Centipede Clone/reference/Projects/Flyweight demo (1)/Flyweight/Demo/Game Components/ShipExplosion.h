// ShipExplosion
// Andre Berthiaume, March 2013

#ifndef _ShipExplosion
#define _ShipExplosion

#include "TEAL\CommonElements.h"

class ShipExplosion : public GameObject
{
public:
	ShipExplosion( sf::Vector2f pos );
	virtual void Update();
	virtual void Draw();

private:
	AnimatedSprite MainSprite;
	sf::Sound kaboom;
};


#endif _ShipExplosion