#include <stdio.h>
#include <string.h>

#include "CopyToBuffer.h"


// retrieve the message
char * dog::getMessage()
{
	return this->buff;
}
	
// user sets the message
void dog::setMessage( const char * const inMessage )
{
	// Do the copy to internal variable buff
	// make sure it's safe
	// ----> do work here

	if (inMessage == '\0'){//empty string

		buff[0] = '\0';

	}
	else
	{
		int place = 0;
		while (inMessage[place] != '\0' && place < BUFF_SIZE-1)
		{
			buff[place] = inMessage[place];
			place++;
		}

		//add terminating null
		buff[place] = '\0';
	}

}

// prints the mesage
void dog::print()
{
	printf("message: %s\n",this->buff);
}


dog::dog()
{
	memset( this->buff, 0xaa, BUFF_SIZE );
}

dog::~dog()
{
	memset( this->buff, 0xaa, BUFF_SIZE );
}


