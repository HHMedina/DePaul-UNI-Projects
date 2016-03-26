#include <stdio.h>

// ----------------------------------------------------------------------
// Problem 7:
//
// Rules: Fix the code below to have the correct number of components in Car
// ----------------------------------------------------------------------

class Car
{ 
public:
	enum class Color
	{ 
		Blue, 
		Red,
		Black,
		White
	} color;

	int	wheelDrive;
	int	numDoors;
	int	wings;

};

class Volkswagon : public Car
{
 public: 
	Volkswagon()
	{ 
		wheelDrive = 2; 
		numDoors = 4;
		wings = 0;
	}
};

class Audi: public Volkswagon
{ 
public:
	Audi() 
	{ 
		color = Color::Black;
		wheelDrive += 2; 
	}

};

// -----------------------------------------------------
// DO NOT MODIFY (below this line) 
// -----------------------------------------------------
int main()
{	
	// test 7 header
	printf("\nTest_7: \n\n");

	Audi m;

	printf("Expected: Color:2 numDoors:4 wheelDrive:4 wings:0\n" );
	printf("  Actual: Color:%d numDoors:%d wheelDrive:%d wings:%d\n", m.color, m.numDoors, m.wheelDrive, m.wings);
	
	return 0;
}