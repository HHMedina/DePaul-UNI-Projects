#include "output.h"
#include <vector>
#include <algorithm>

struct Vect
{

	//*****************************************
	//	Added this operator because it allows
	//	sort to work. Could also be put outside
	//	of the vect class, but there is no real
	//	reason as to why that should be preferred.
	//	Effecive STL says to prefer adding operator
	//	to allow easy use with stl
	//*****************************************
	bool operator <(const Vect& rhs)
	{
		//Check if this a is less than other a
		if (this->a < rhs.a)
		{
			return true;
		}
		if (this->a > rhs.a)
		{
			return false;
		}

		//Check if this b is less than other b
		if (this->b < rhs.b){
			return true;
		}
		if (this->b > rhs.b)
		{
			return false;
		}

		//Check if this c is less than other c
		if (this->c < rhs.c){
			return true;
		}
		if (this->c > rhs.c)
		{
			return false;
		}
		
		return false;
		

	}

	int a;
	int b;
	int c;
};

Vect V[] =
{
	{3,5,6},
	{6,5,3},
	{3,4,7},
	{7,2,5},
	{1,3,5},
	{3,2,5},
	{2,3,4},
	{3,5,5},
	{7,3,2},
	{4,3,2},
	{1,7,1}
};

void PrintVector(const char * const sectionName,const std::vector<Vect>& _vector)
{
	fprintf(io::getHandle(), "\n%s\n",sectionName);

	for (unsigned int i = 0; i < _vector.size();i++)
	{
		fprintf(io::getHandle(), "        V[%2d]: %d  %d  %d \n", i, _vector[i].a, _vector[i].b, _vector[i].c);
	}
}


void Problem_4( )
{
	// create file
	io::create( "Hector Medina", "problem4.txt" );
	

	// Original data
	
	Vect *p = &V[0];
	fprintf(io::getHandle(),"original\n");
	for( int i = 0; i < 11; i++)
	{
		fprintf(io::getHandle(),"        V[%2d]: %d  %d  %d \n", i, p->a, p->b, p->c );
		p++;
	}

	// a) Load the V[] data into a vector
	//    print it to the file (begin to end) order
	std::vector<Vect> Vect_V(V,V+11);
	PrintVector("stl: vector",Vect_V);

	// b) Sort the new STL vector with strict weak ordering
	//    print it to the file (begin to end) order
	std::sort(Vect_V.begin(),Vect_V.end());
	PrintVector("stl: strict weak ordering", Vect_V);

	// bye bye
	fprintf(io::getHandle(),"\n");
	io::destroy();
}