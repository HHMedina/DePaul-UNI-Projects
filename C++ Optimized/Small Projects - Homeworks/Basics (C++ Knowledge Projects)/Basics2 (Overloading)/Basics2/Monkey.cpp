#include <stdio.h>
#include <string.h>

#include "Monkey.h"


int Monkey::getX()
{
	return this->x;
}

int Monkey::getY()
{
	return this->y;
}

char *Monkey::getStatus()
{
	return this->status;
};

void Monkey::printStatus()
{
	printf("Monkey (Status): %s\n", this->status);
}


// END of File