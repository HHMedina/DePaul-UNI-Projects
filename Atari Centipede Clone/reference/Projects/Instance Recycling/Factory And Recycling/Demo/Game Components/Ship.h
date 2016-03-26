// Ship
// Andre Berthiaume, Aug 2012

#ifndef _Ship
#define _Ship

#include "TEAL\CommonElements.h"

// Forward declarations
class Widget;
class Asteroid;

class Ship : public GameObject
{
public:
	Ship();
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	virtual void KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);
	
	virtual void Collision( GameObject *other ){};
	void Collision(Widget *other);
	void Collision(Asteroid *other);

private:
	float SPEED;
	float HBORDER;
	float TBORDER;
	sf::Vector2f GunOffset;

	float offset;

	sf::Vector2f Pos;
	sf::Vector2f Impulse;
	float friction;

	AnimatedSprite MainSprite;
	CollisionTools::TextureBitmap bitmap;

	sf::Sound FireSnd;
};

#endif _Ship