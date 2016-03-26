// AwesomeFont
// Andre Berthiaume, May 2013

#ifndef _AwesomeFont
#define _AwesomeFont

#include "SpriteSheet.h"

class AwesomeFont : public SpriteSheet
{
public:
	AwesomeFont();

private:
	virtual int CharToIndex( char& c ); 
};


#endif _AwesomeFont