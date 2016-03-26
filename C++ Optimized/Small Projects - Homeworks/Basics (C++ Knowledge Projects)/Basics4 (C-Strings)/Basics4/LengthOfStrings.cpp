
#include <stdio.h>
#include <string.h>

#include"LengthOfStrings.h"



size_t LengthOfString_1( const char  * const input)
{
	// using strlen()
	return strlen(input);
}

size_t LengthOfString_2( const char  * const input)
{
	// using char by char
	int counter = 0;
	const char* temp = input;
	while (temp[counter] != '\0')
	{				
		counter++;
	}

	return counter;
}
