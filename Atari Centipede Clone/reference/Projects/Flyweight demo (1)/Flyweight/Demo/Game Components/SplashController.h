// SplashController
// Andre Berthiaume, March 2013

#ifndef _SplashController
#define _SplashController

#include "TEAL\CommonElements.h"

#include "Glyph.h"
#include <vector>

class SplashController : public GameObject
{
public:
	SplashController();
	virtual ~SplashController();
	virtual void Draw();
	virtual void KeyPressed(sf::Keyboard::Key k, bool altKey, bool ctrlKey, bool shiftKey, bool systemKey);

private:
	sf::Sprite SplashSprite;

	//SpriteSheet mysheet;
	std::vector<Glyph> word;
};

#endif _SplashController