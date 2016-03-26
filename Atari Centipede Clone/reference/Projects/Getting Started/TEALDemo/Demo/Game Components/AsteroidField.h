// AsteroidField
// Andre Berthiaume, March 2013

#ifndef _AsteroidField
#define _AsteroidField

#include "TEAL\CommonElements.h"

class AsteroidField : public GameObject
{
public:
	AsteroidField();
	virtual void Alarm0();

private:
	int TOP;
	int BOTTOM;
	int GENLINE;
};

#endif _AsteroidField