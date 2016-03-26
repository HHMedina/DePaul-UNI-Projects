// SimpleTextDemo
// Andre Berthiaume, May 2013

#include "SimpleTextDemo.h"

SimpleTextDemo::SimpleTextDemo()
{
	RegisterInput( InputFlags::TextEntered );

	std::string text = "HELLO WORLD";
	nextpos = sf::Vector2f(100,100);
	
	for (int i=0; i < text.length(); i++)
	{
		symbols.push_back(  myfont.GetGlyph(text.at(i), nextpos) );
		nextpos.x += myfont.CellWidth();
	}
}

void SimpleTextDemo::TextEntered(sf::Uint32 UnicodeValue) 
{
	symbols.push_back( myfont.GetGlyph( (char) UnicodeValue, nextpos ) );
	nextpos.x += myfont.CellWidth();
}

void SimpleTextDemo::Draw()
{
	std::list<Glyph>::iterator it;
	for (it = symbols.begin(); it != symbols.end(); it++)
		it->Draw();
}