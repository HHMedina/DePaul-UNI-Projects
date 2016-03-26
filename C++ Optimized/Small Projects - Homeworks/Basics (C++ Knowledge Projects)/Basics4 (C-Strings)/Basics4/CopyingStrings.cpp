
#include <stdio.h>
#include <string.h>

#pragma warning( disable : 4996 )

#include "CopyingStrings.h"
 

char *StringCopy_1( char * const dest, const char * const input)
{
	strcpy(dest,input);
	// using strcpy()
	return dest;
}

char *StringCopy_2( char  * const dest, const char * const input)
{
	memcpy(dest,input,strlen(input)+1);
	// using memcpy() and strlen()
	return dest;
}

char *StringCopy_3( char  * const dest, const char * const input)
{
		
	int i = 0;
	while (input[i]!='\0')
	{
		dest[i] = input[i];
		i++;
	}

	//add terminating null
	dest[i] = '\0';

	// using char by char copies
	return dest;
}

