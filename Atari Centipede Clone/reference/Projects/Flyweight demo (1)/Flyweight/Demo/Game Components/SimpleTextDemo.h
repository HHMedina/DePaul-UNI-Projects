// SimpleTextDemo
// Andre Berthiaume, May 2013

#ifndef _SimpleTextDemo
#define _SimpleTextDemo

#include "TEAL/CommonElements.h"
#include <list>
#include "AwesomeFont.h"
#include "KeysFont.h"

class SimpleTextDemo : public GameObject
{
public:
	SimpleTextDemo();
	virtual void TextEntered(sf::Uint32 UnicodeValue);
	virtual void Draw();

private:
	AwesomeFont myfont;
	std::list<Glyph> symbols;
	sf::Vector2f nextpos;
};


#endif _SimpleTextDemo