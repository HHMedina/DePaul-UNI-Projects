#ifndef __QUAT_H__
#define __QUAT_H__

#include <assert.h>
#include <iostream>
using namespace std;

#include "vector.h"
#include "jmisc.h"
#include "Loader.h"
#include "Matrix.h"

class quat
{
public:

	quat() : w(1.0), v(0.0, 0.0, 0.0) { }
	quat(float W, float X = 0.0, float Y = 0.0, float Z = 0.0) : w(W), v(X, Y, Z) { }
	quat(float W, vector V) : w(W), v(V) { }

	float &operator[](int i) { assert(i >= 0 && i <= 3); if (i == 0) return w; else return v[i - 1]; }
	float operator[](int i) const { assert(i >= 0 && i <= 3); if (i == 0) return w; else return v[i - 1]; }

	float Length() const;
	void Normalize();
	
	quat conj(void) const;
	quat inverse(void) const;

	float scale(void) const;

	// The following work with any quaternion
	float angle(void) const;
	float cosTheta(void) const;
	float sinTheta(void) const;
	vector unitV(void) const;

	// The following are optimized for unit quaternions
	float angleU(void) const;
	float cosThetaU(void) const;
	float sinThetaU(void) const;
	vector unitVU(void) const { return unitV(); } // Calculating the vector is the same as for a non-unit

	quat &operator+=(const quat &r);
	quat &operator-=(const quat &r);
	quat &operator*=(const quat &r);
	quat &operator/=(const quat &r);

	vector rotate(vector v) const;

	void QuatFromAngleAxis(float a[4]);
	void QuatFromAngleAxis(float theta, vector V);
	void QuatToAngleAxis(float a[4]) const;
	void QuatToAngleAxis(float &theta, vector &V) const;
	void QuatFromEulerAngles(float x, float y, float z);

	float w;
	vector v;
};

quat operator-(const quat &q);
quat operator+(const quat &q1, const quat &q2);
quat operator-(const quat &q1, const quat &q2);
quat operator*(const quat &q1, const quat &q2);
quat operator/(const quat &q1, const quat &q2);
float dot(const quat &a, const quat &b);
ostream &operator<<(ostream &output, const quat &q);

///////////////////////////////////////////////////////////////////
// The following are only meant for unit quaternions!!!!!!	     //
///////////////////////////////////////////////////////////////////

inline quat exp(const quat q)
{
	assert(q.w == 0);

	float theta = q.v.Length();
	if (fabs(theta) < 1e-9)
		return quat(1, 0, 0, 0);
	else
		return quat(cos(theta), sin(theta) * q.v / theta);
}

inline quat log(const quat q)
{
	float theta = q.angleU();
	if (fabs(theta) < 1e-9)
		return quat(0, 0, 0, 0);
	else
	{
		vector v = q.unitVU();
		return quat(0, theta*v);
	}
}

inline quat chooseClosest(const quat &a, const quat &b)
{
	return (dot(a, b) >= 0 ? b : -b);
}

#endif