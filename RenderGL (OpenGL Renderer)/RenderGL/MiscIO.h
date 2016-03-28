#ifndef __MISCIO_H__
#define __MISCIO_H__

#include <iostream>
using namespace std;

#define MAX_NAME_LENGTH 256 
#define MAX_INPUT_LENGTH 512

void Trim(char *str);
bool isnumeric(char *str);
void LCase(char *token);

#endif