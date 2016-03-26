#include "output.h"
#include <vector>
#include <algorithm>

int A[] = 
{
	6,
	7,
	3,
	2,
	8,
	1,
	9,
	4
};


void Problem_1( )
{
	// create file
	io::create( "Hector Medina", "problem1.txt" );

	// Original data
	
	int *p = &A[0];
	fprintf(io::getHandle(),"original\n");
	for( int i = 0; i < 8; i++)
	{
		fprintf(io::getHandle(),"        A[%2d]: %d   \n", i, *p++ );
	}

	// a) Load the A[] data into a vector
	//    print it to the file (begin to end) order
	std::vector<int> Vect_A;
	Vect_A.reserve(8);//avoid reallocation of undelying container
	Vect_A.insert(Vect_A.begin(),A,A+8);
	
	fprintf(io::getHandle(), "\nstl: vector\n");
	for (unsigned int i = 0; i < Vect_A.size(); i++)
	{
		fprintf(io::getHandle(), "        Vect_A[%2d]: %d   \n", i, Vect_A[i]);
	}

	// b) Insert the number '5' between 2 and 8
	//    No brute force, assume there are 1000000 numbers before 2 and after 8.
	//    No iterative, simple approach, use the appropriate STL calls for efficency.
	//    print it to the file (begin to end) order
	
	Vect_A.insert(std::find(Vect_A.begin(), Vect_A.end(), 8), 5);

	fprintf(io::getHandle(), "\nstl: insert '5' into vector\n");
	for (unsigned int i = 0; i < Vect_A.size(); i++)
	{
		fprintf(io::getHandle(), "        Vect_A[%2d]: %d   \n", i, Vect_A[i]);
	}
	

	// c) sort the vector
	//    print it to the file (begin to end) order
	std::sort(Vect_A.begin(), Vect_A.end());

	fprintf(io::getHandle(), "\nstl: sort vector\n");
	for (unsigned int i = 0; i < Vect_A.size(); i++)
	{
		fprintf(io::getHandle(), "        Vect_A[%2d]: %d   \n", i, Vect_A[i]);
	}

	// bye bye
	fprintf(io::getHandle(),"\n");
	io::destroy();
}