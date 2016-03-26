#include <Collidable.h>
#include <iostream>
using namespace std;

bool Collidable::TestCollisionPair(Collidable* a, Collidable * b){
		
		//allerts the user if there is no collidable before crashing
		assert(a->collisionModel!=NULL &&"Colllidable: A Collidable does not have a collision model, but is registered for collision");
		assert(b->collisionModel!=NULL &&"Colllidable: A Collidable does not have a collision model, but is registered for collision");
		
		assert(a && b);//check for valid pointers
		

		//initial unit vectors
		//note: Will not be affected by world transform's translation
		Vect aVector(0,0,1,0);
		Vect bVector(0,0,1,0);

		//initial Radius values
		float iARadius = a->collisionModel->radius;
		float iBRadius = b->collisionModel->radius;

		 
		//initial Center values
		Vect iACenter = a->collisionModel->center;
		Vect iBCenter = b->collisionModel->center;

		//Find transformed a center and a vector
		Vect newCenterA = iACenter * a->colWorld; //multiply by world trans new center position
		Vect transAVector = aVector * a->colWorld;

		//get a's scale factor
		float scaleFac_A = (transAVector).mag() ; // vector length is not affected by translation or rotation


		//Find transformed b center and b vector
		Vect newCenterB = iBCenter * b->colWorld; //multiply by world trans new center position
		Vect transBVector = bVector * b->colWorld;

		//get b's scale factor
		float scaleFac_B = (transBVector).mag() ;
			

		//Get Distance between transformed centers
		float dist = (newCenterA  - newCenterB).mag();
		
		float radiusAdded =scaleFac_A*iARadius + scaleFac_B *iBRadius;
		//check if distance between is more than the radius added
		if(dist < radiusAdded ){
			return true; //collision
		}

		return false; //no collision
		
	}