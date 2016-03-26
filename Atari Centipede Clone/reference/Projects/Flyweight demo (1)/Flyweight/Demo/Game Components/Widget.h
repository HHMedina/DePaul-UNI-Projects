// Widget
// Andre Berthiaume, March 2013

#ifndef _Widget
#define _Widget

#include "TEAL\CommonElements.h"

class Widget : public GameObject
{
public:
	Widget( sf::Vector2f p );
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();

	virtual void Collision( GameObject *go){};

	int GetValue();


private:
	sf::Vector2f Pos;
	sf::CircleShape MainShape;
	
	float SPEED;
	float ROTSPEED;
	float RADIUS;

	int pcount;
	int Value;
	sf::Font LabelFont;
	sf::Text PointLabel;
};

#endif _Widget