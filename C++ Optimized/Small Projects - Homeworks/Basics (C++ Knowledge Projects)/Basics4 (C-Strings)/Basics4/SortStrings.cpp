#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#pragma warning( disable : 4996 )

#include "SortStrings.h"


int compare(const void * a, const void * b)
{
	return strcmp(*(char**)a,*(char**)b);
}

int compareLength(const void * a, const void * b)
{
	unsigned int lengthA = strlen(*(char**)a);
	unsigned int lengthB = strlen(*(char**)b);

	if (lengthA<lengthB){
		return -1;
	} else if (lengthA>lengthB)
	{
		return 1;
	}
	else{

		//if they are the same then sort based on greater value (Alphabetical order)
		return strcmp(*(char**)a, *(char**)b);
	}
}

void ReorderAlphabetical(const char * const inString, char * const outString )
{
	// 1) reorder the words in the string, words are separated with spaces
	// 2) sort words on alphabetical order, (a begin, z end)
	// 3) you don't know how many words 
	// 4) if dynamically create any buffers inside this routine, clean them up
	// 5) use strtok and strcat in your answer
	// 6) hint: qsort() - might be a good function to know

	char* inCopy= new char[strlen(inString) + 1];
	strcpy(inCopy,inString);
	
	//find the amount of words
	int numberOfWords=0;
	
	const char* it = inString;
	while (*it != '\0'){
		if (*it == ' '){
			numberOfWords++;
		}
		it++;
	}
	numberOfWords++;

	

	char** charArr = new char*[numberOfWords];

	int counter = 0;
	char* tokenHolder = strtok(inCopy, " ");
	while (tokenHolder != NULL){
		
		charArr[counter] = tokenHolder;
		counter++;
		tokenHolder = strtok(NULL," ");
	}
	
	qsort(charArr, numberOfWords,sizeof(char*),compare);
	
	//clear string
	outString[0] = '\0';
	for (int i = 0; i < numberOfWords; i++){
		strcat(outString, charArr[i]);
		if (i<numberOfWords-1){
			strcat(outString, " ");
		}
	}
	
	delete[] inCopy;
	delete[] charArr;
}



void ReorderWordLength(const char * const inString, char * const outString )
{
	// 1) reorder the words in the string, words are separated with spaces
	// 2) sort words on their word length order, (short, longest)
	// 3) you don't know how many words 
	// 4) if dynamically create any buffers inside this routine, clean them up
	// 5) use strtok and strcat in your answer
	// 6) hint: qsort() - might be a good function to know

	char* inCopy = new char[strlen(inString) + 1];
	strcpy(inCopy, inString);

	//find the amount of words
	int numberOfWords = 0;

	const char* it = inString;
	while (*it != '\0'){
		if (*it == ' '){
			numberOfWords++;
		}
		it++;
	}
	numberOfWords++;



	char** charArr = new char*[numberOfWords];

	int counter = 0;
	char* tokenHolder = strtok(inCopy, " ");
	while (tokenHolder != NULL){

		charArr[counter] = tokenHolder;
		counter++;
		tokenHolder = strtok(NULL, " ");
	}

	qsort(charArr, numberOfWords, sizeof(char*), compareLength);

	//clear string
	outString[0] = '\0';
	for (int i = 0; i < numberOfWords; i++){
		strcat(outString, charArr[i]);
		if (i<numberOfWords - 1){
			strcat(outString, " ");
		}
	}
	
	delete[] inCopy;
	delete[] charArr;
	
}



