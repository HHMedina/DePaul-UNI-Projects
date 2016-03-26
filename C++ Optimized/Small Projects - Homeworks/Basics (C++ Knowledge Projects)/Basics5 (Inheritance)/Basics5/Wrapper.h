#ifndef WRAPPER_H
#define WRAPPER_H

#include "MiddlewareNative.h"

// For this problem, you are wrapping a poor interface provided by the middleware vendors
// Feel free to modify and add to this class

// The class needs to derive privately from Vect2D_Native

// Requirements:

// 1) variables:  x, y should be private
// 2) prevent methods setID() and getID() from being called //done
// 3) Rework getX() and getY() to take float references // done
// 4) prevent the original getX() and getY() from being called // done
// 5) allow setX() and setY() to be called as is. //done

// The unit test has a section that is #if 0 - toggle it to make sure your code is producing the correct errors

class GameVect: private Vect2D_Native
{
public:
	
	void setX(const float& value){
		Vect2D_Native::setX(value); //call original function
	}

	void setY(const float& value){
		Vect2D_Native::setY(value); //call original function
	}

	void getX(float& outX) const {
		outX = x;
	}

	void getY(float& outY) const {
		outY = y;
	}

private:

	//Intentionally left bodyless in the case the user calls function, crash will occur.
	void setID();
	void getID();
	float getX();
	float getY();
};


#endif