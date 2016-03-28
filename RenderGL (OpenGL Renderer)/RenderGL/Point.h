#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
#include <assert.h>
#include <math.h>
#include "MiscMath.h"

using namespace std;

class point
{
public:

	point(float x = 0.0, float y = 0.0, float z = 0.0, float w = 1.0);

	void Normalize(void);
	bool IsNormal(void) const { return IsEqual(p[3], 1.0); }

	float *AsArray(void) { return p; }
	const float *AsArray(void) const { return p; }

	float &operator[](unsigned int i) { assert(i >= 0 && i <= 3); return p[i]; }
	float operator[](unsigned int i) const { assert(i >= 0 && i <= 3); return p[i]; }

	point &operator+=(const point &q);

	static point origin;

private:

	float p[4];
};

////////////////////////////////////////////////////////////
// Global mathematical operations
////////////////////////////////////////////////////////////

bool IsZero(const point &p);
inline bool IsNotZero(const point &p) { return !IsZero(p); }
bool IsEqual(const point &p, const point &q);
bool IsNotEqual(const point &p, const point &q);

float Distance(const point &p, const point &q);
point Average(const point &p, const point &q, float w = .5);

point operator+(const point& p, const point& q);
point operator*(float d, const point &p);
point operator*(const point &p, float d);
point operator/(const point &p, float d);

point &operator*=(point &p, float d);
point &operator/=(point &p, float d);

istream &operator>>(istream &input, point &v);
ostream &operator<<(ostream &output, const point &v);

#endif