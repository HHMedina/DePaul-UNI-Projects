
#include "quat.h"

float quat::Length() const 
{ 
	// To Do
	// 
	// Calculate the length of the quaternion
	return 0.0; 
}

void quat::Normalize() 
{
	// To Do
	//
	// Normalize the quaternion
}

quat quat::conj(void) const 
{ 
	// To Do
	//
	// return the conjugate of this quat
	
	return quat(1.0, 0.0, 0.0, 0.0);
}

quat quat::inverse(void) const
{ 
	// To Do
	// 
	// Return the inverse of this quat

	return quat(1.0, 0.0, 0.0, 0.0);
}

float quat::scale(void) const 
{ 
	return Length(); 
}

// The following work with any quaternion
float quat::angle(void) const 
{ 
	// To Do
	//
	// Return the rotation angle of this quat

	return 0.0;
}

float quat::cosTheta(void) const 
{ 
	// To Do
	// 
	// Return the cosine of the rotation angle of this quat

	return 1.0;
}

float quat::sinTheta(void) const 
{ 
	// To Do
	// 
	// Return the sine of the rotation angle of this quat

	return 0.0;
}

vector quat::unitV(void) const 
{ 
	// To Do
	//
	// Return the unit rotation axis of this quat

	return vector(0.0, 0.0, 0.0);
}

// The following are optimized for unit quaternions
float quat::angleU(void) const 
{ 
	// To Do
	// 
	// Return the rotation angle of this quat

	return 0.0;
}

float quat::cosThetaU(void) const 
{ 
	// To Do
	//
	// Return the cosine of the rotation angle of this quat

	return 1.0;
}

float quat::sinThetaU(void) const 
{ 
	// To Do
	//
	// Return the sine of the rotation angle of this quat

	return 0.0;
}

quat &quat::operator+=(const quat &r)
{ 
	// To Do
	//
	// Add the quat "r" to this quat

	return *this;
}

quat &quat::operator-=(const quat &r)
{ 
	// To Do
	// 
	// Subtract the quat "r" from this quat

	return *this;
}

quat &quat::operator*=(const quat &r)
{ 
	// To Do
	// 
	// Multiply this quat by "r" on the right

	return *this;
}

quat &quat::operator/=(const quat &r)
{ 
	// To Do
	//
	// Divide this quat by the quat "r"

	return *this;
}

vector quat::rotate(vector v) const 
{ 
	// To Do
	// 
	// Calculate the rotation of the vector "v" by this quat

	return vector(0.0, 0.0, 0.0);
}

void quat::QuatFromAngleAxis(float a[4])
{ 
	// To Do
	//
	// Convert the angle axis value stored in the array to a quat.
	// The array contains (angle, ax, ay, az).  Note that "a" may not 
	// be normalized!
}

void quat::QuatFromAngleAxis(float theta, vector V)
{
	// To Do
	//
	// Same as above but with a different type of angle axis storage
}

void quat::QuatToAngleAxis(float a[4]) const 
{ 
	// To Do
	//
	// Do the opposite of the above operation
}

void quat::QuatToAngleAxis(float &theta, vector &V) const
{
	// To Do
	//
	// Once again, but convert it to an angle and an axis vector
}

void quat::QuatFromEulerAngles(float x, float y, float z)
{
	// To Do
	// 
	// Convert the given euler angles to a quat
}

quat operator-(const quat &q)
{ 
	// To Do
	//
	// Return the negative of this quat

	return quat(1.0, 0.0, 0.0, 0.0);
}

quat operator+(const quat &q1, const quat &q2)
{
	// To Do
	// 
	// Return the sum of these two quats

	return quat(1.0, 0.0, 0.0, 0.0);
}

quat operator-(const quat &q1, const quat &q2)
{
	// To Do
	// 
	// Return the difference of these two quats

	return quat(1.0, 0.0, 0.0, 0.0);
}

quat operator*(const quat &q1, const quat &q2)
{
	// To Do
	// 
	// Return the product of these two quats

	return quat(1.0, 0.0, 0.0, 0.0);
}

quat operator/(const quat &q1, const quat &q2)
{
	// To Do
	// 
	// Return the quat of these two quats

	return quat(1.0, 0.0, 0.0, 0.0);
}

float dot(const quat &a, const quat &b)
{
	// To Do
	// 
	// Return the dot product of these two quats

	return 0.0;
}

ostream &operator<<(ostream &output, const quat &q)
{
	output << "(" << q.w << ", <" << q.v[0] << ", " << q.v[1] << ", " << q.v[2] << ">)";
	return output;
}

