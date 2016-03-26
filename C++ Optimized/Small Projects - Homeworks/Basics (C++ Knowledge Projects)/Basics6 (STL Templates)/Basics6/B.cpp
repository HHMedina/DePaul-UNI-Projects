#include "output.h"
#include <list>
#include <functional>

int B[] = 
{
	66,
	77,
	33,
	22,
	88,
	11,
	99,
	44
};

void PrintSTLList(const char* const sectionName,const std::list<int>& _list)
{
	int counter = 0;
	fprintf(io::getHandle(),"\n%s\n", sectionName);
	std::list<int>::const_iterator end = _list.end();
	for (std::list<int>::const_iterator iter = _list.begin(); iter != end;iter++)
	{
		fprintf(io::getHandle(), "        List_B[%d]: %d   \n", counter, *iter);
		counter++;
	}
	
}

void Problem_2( )
{
	// create file
	io::create( "Hector Medina", "problem2.txt" );

	// Original data
	
	int *p = &B[0];
	fprintf(io::getHandle(),"original\n");
	for( int i = 0; i < 8; i++)
	{
		fprintf(io::getHandle(),"        B[%2d]: %d   \n", i, *p++ );
	}

	// a) Load the B[] data into a list
	//    print it to the file (begin to end) order
	std::list<int> List_B(B,B+8);
	PrintSTLList("stl: init list",List_B);

	// b) Insert the number '55' between 22 and 88
	//    No brute force, assume there are 1000000 numbers before 22 and after 88.
	//    No iterative, simple approach, use the appropriate STL calls for efficency.
	//    print it to the file (begin to end) order
	List_B.insert( std::find(List_B.begin(),List_B.end(),88), 55); //insert 55 before 88
	PrintSTLList("stl: insert '55' into list",List_B);

	// c) sort the list in reverse order using the built in functional functions
	//    print it to the file (begin to end) order
	List_B.sort(std::greater<int>());
	PrintSTLList("stl: sort list in reverse order using built in function", List_B);

	// bye bye
	fprintf(io::getHandle(),"\n");
	io::destroy();
}