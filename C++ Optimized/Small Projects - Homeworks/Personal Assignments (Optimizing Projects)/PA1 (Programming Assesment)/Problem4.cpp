#include <math.h>
#include <float.h>
typedef struct Vect // Vector struct for positions
{
	float x;
	float y;
	float z;
} Vect_t;
/***********************************************************************
*
* Function: findMaxDistance()
*
* Input:
* int nPlayers - number of players
* Vect_t *playerArray - the array of players
* Output:
* float outMaxDist - the maxDistance between any two players
* float outMinDist - the minDistance between any two players
***********************************************************************/
void findMinMaxDistance(int nPlayers, Vect_t *playerArray,float& outMinDist,float& outMaxDist)
{
	int i, j; // counter variables
	Vect_t tmpVect; // temporary vector
	float tmpDistSqrd; // temporary distance
	float maxDistSqrd; // current max distance
	float minDistSqrd; // current min distance

	// initialize the max/min distance to zero
	maxDistSqrd = 0.0f;
	minDistSqrd = FLT_MAX; 
	for (i = 0; i < nPlayers; i++)
	{
		for (j = i+1; j < nPlayers; j++)
		{
			// Find a vector between point i and j
			tmpVect.x = playerArray[i].x - playerArray[j].x;
			tmpVect.y = playerArray[i].y - playerArray[j].y;
			tmpVect.z = playerArray[i].z - playerArray[j].z;
			// Get its length^2 since sqrt can be taken out as inequalities are the same when operations are done to both side.
			tmpDistSqrd = (float)(tmpVect.x * tmpVect.x
				+ tmpVect.y * tmpVect.y
				+ tmpVect.z * tmpVect.z);
			// determine if it’s a new maximum length
			if (tmpDistSqrd > maxDistSqrd)
			{
				// yes so keep it
				maxDistSqrd = tmpDistSqrd;
			}
			else if(tmpDistSqrd<minDistSqrd)
			{
				//keep it
				minDistSqrd = tmpDistSqrd;

			}
		} //for(j)
	} // for(i)

	//set output values
	outMaxDist = sqrt(maxDistSqrd);
	outMinDist = sqrt(minDistSqrd);
} // End of findMaxDistance()
