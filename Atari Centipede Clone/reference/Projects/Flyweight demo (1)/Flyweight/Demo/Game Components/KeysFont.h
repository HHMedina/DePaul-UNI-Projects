// KeysFont
// Andre Berthiaume, May 2013

#ifndef _KeysFont
#define _KeysFont

#include "SpriteSheet.h"

class KeysFont : public SpriteSheet
{
public:
	KeysFont();

private:
	virtual int CharToIndex( char& c ); 
};


#endif _KeysFont