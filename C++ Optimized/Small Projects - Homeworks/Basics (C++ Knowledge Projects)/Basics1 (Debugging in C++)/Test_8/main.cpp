#include <stdio.h>

// ----------------------------------------------------------------------
// Problem 8:
//
// Rules: Fix the code to have the correct day printing for St Pat's day.
//        Please keep the virtual function init() as a restriction
//        You can make this work with a small addition
// ----------------------------------------------------------------------

class Holiday
{
public:
	virtual void init()
	{
	}
	
	Holiday() 
	{
		init(); //is this really needed? its a call to
				//an empty function, that could have been called
				//from the class that was being constructed.
				//if this is here, then init() in holiday will be called everytime
				//that an subclass is constructed. If the subclass calls its init()
				//construction, why is holiday's init() call needed?
				//I'm thinking about this as if holiday was an abstract class.
	}
};

class StPats : public Holiday
{
public:
	const char *day;
	
	StPats()
	{
		init();
	}

	virtual void init() override
	{ 
		day = "March 17"; 
	}
};

// -----------------------------------------------------
// DO NOT MODIFY (below this line) 
// -----------------------------------------------------
int main()
{	
	// test 8 header
	printf("\nTest_8: \n\n");

	StPats x; 

	printf( "St Patrick's Day is %s\n" , x.day );
	
	return 0;
}