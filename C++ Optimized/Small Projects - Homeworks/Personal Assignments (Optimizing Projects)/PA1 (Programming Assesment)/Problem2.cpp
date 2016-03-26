#include <math.h>
typedef struct Vect // Vector struct for positions
{
	float x;
	float y;
	float z;
} Vect_t;


/*
inVect = the vector to be normalized
*/
void VectNormalize(Vect_t& inVect){

	//check for zero vector... if so return 0 vect
	if (inVect.x == 0 && inVect.y == 0 && inVect.z == 0)
	{
		//do no more..the zero vector will stay as a zero vector
	}
	else{
		
		//find the length
		float vLength = sqrt(inVect.x*inVect.x + inVect.y * inVect.y + inVect.z*inVect.z);

		inVect.x /= vLength;
		inVect.y /= vLength;
		inVect.z /= vLength;
	}

}