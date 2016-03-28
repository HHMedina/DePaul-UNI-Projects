
#include "miscio.h"
#include <string.h>
#include <limits.h>
#include "jmisc.h"
#include <stdlib.h>
#include <ctype.h>

bool isnumeric(char *str)
{
   for(int j = 0; str[j]; ++j)
      if(!(isdigit(str[j]) || isspace(str[j]))) 
		  return false;
   return true;
}

void Trim(char *str)
{
	int first, last;
	for (first = 0; (str[first] == ' ') || (str[first] == '\t') || (str[first] == '\n'); first++);
	for (last = strlen(str) - 1; (str[last] == ' ') || (str[last] == '\t') || (str[last] == '\n'); last--);
	str[last + 1] = '\0';

	for (int i = first; i <= last + 1; i++)
		str[i - first] = str[i];
}

void LCase(char *token)
{
	int len = strlen(token);
	for (int i = 0; i < len; i++)
		if (token[i] >= 'A' && token[i] <= 'Z')
			token[i] = (token[i] - 'A' + 'a');
}

