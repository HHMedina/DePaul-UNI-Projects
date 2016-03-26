#ifndef MONKEY_H
#define MONKEY_H
#include "stdio.h"
class Monkey
{
public:

	// insert your code here

	Monkey()
		:x(111), y(222), status(new char[25])
	{
		numStringsCreated++;
		strcpy_s(this->status,25, "Initialized with default");
	}

	Monkey(int _x, int _y)
		:x(_x), y(_y),status(new char[20])
	{
		numStringsCreated++;
		strcpy_s(this->status,20, "Initialized by user");
	}

	Monkey(const Monkey& rhs)
		:x(rhs.x), y(rhs.y),status(new char[strlen( rhs.status )+1])
	{
		numStringsCreated++; 
		strcpy_s(this->status, strlen(rhs.status)+1, rhs.status);
	}


	const Monkey& operator = (const Monkey& rhs)
	{
		if (this != &rhs){
			this->x = rhs.x;
			this->y = rhs.y;

			delete status;
			numStringsDestroyed++;

			status = new char[strlen(rhs.status)+1];
			numStringsCreated++;
			strcpy_s(this->status, strlen(rhs.status)+1, rhs.status);
			
		}

		return *this;
	}

	~Monkey()
	{
		delete status;
		numStringsDestroyed++;
	}
	// accessors
	int getX();
	int getY();
	char *getStatus();
	void printStatus();

	// global variables (incremented each creation or destruction)
	static int numStringsCreated;
	static int numStringsDestroyed;

private:
	int x;
	int y;
	char *status;
};


#endif