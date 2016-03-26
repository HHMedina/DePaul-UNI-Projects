// KeysFont
// Andre Berthiaume, May 2013

#include "KeysFont.h"
#include "TEAL\ResourceManager.h"

KeysFont::KeysFont()
	: SpriteSheet( ResourceManager::GetTexture("Keys"), 11, 4 )
{}

// Computing the magic value of the cellIndex for each char
int KeysFont::CharToIndex( char& c )
{
	int cellIndex = 0;

	if ( c >= 'A' && c <= 'Z')
		cellIndex = (int) c - (int) 'A';	
	else if ( c >= '0' && c <= '9')
		cellIndex = (int) c - (int) '0' + 26;
	else if (c == ' ')
		cellIndex = 37;
	else
		cellIndex = 0;
	return cellIndex;
}