
#include <stdio.h>
#include <string.h>
#include "ComparingStrings.h"

int StringCompare_1( const char  * const s1, const char * const s2)
{
	// using strcmp()
	return strcmp(s1,s2);
}

int StringCompare_2( const char  * const s1, const char * const s2)
{
	// using memcmp() & strlen()

	return memcmp(s1,s2,strlen(s2));
}

int StringCompare_3( const char  * const s1, const char * const s2)
{
	int place = 0;
	while (s1[place] != '\0' && s2[place] != '\0'){

		if (s1[place] != s2[place])
		{
			//return either -1 or 1
			if (s1[place]>s2[place]){
				return 1;
			}
			else
			{
				return -1;
			}

		}

		place++;
	}

	if (s1[place] == '\0' && s2[place] == '\0'){//check and see if the c strings both are at a terminating null
		return 0;
	}

	// using char by char comparisions
	return 0;
}

