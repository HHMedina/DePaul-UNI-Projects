// WidgetGenerator
// Andre Berthiaume, March 2013

#ifndef _WidgetGenerator
#define _WidgetGenerator

#include "TEAL/CommonElements.h"

class WidgetGenerator : public GameObject
{
public:
	WidgetGenerator();
	virtual void Alarm0();

private:
	int TOP;
	int BOTTOM;
	int GENLINE;
};

#endif _WidgetGenerator