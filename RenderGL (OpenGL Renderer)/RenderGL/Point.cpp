#pragma warning(disable : 4996) // Turns off secure_crt warnings in VS 2005 and up

#include "point.h"
#include <math.h>
#include "jmisc.h" // Miscellaneous routines, including sqr()

point point::origin = point(0, 0, 0);

point::point(float x, float y, float z, float w)
{
	p[0] = x;
	p[1] = y;
	p[2] = z;
	p[3] = w;
}

void point::Normalize(void)
{
	assert(IsNotZero(p[3]));

	float tmp = 1.0f / p[3];
	p[0] *= tmp;
	p[1] *= tmp;
	p[2] *= tmp;
	p[3] = 1.0f;
}

bool IsZero(const point &p)
{
	return (IsZero(p[0]) && IsZero(p[1]) && IsZero(p[2]));
}

bool IsEqual(const point &p, const point &q)
{
	assert(p.IsNormal() && q.IsNormal());

	return (IsEqual(p[0], q[0]) && IsEqual(p[1], q[1]) && IsEqual(p[2], q[2]));
}

bool IsNotEqual(const point &p, const point &q) 
{ 
	return !IsEqual(p, q); 
}

float Distance(const point &p, const point &q)
{
	return sqrt(sqr(p[0] - q[0]) + sqr(p[1] - q[1]) + sqr(p[2] - q[2]));
}

point Average(const point &p, const point &q, float w)
{
	float w1 = 1.0f - w;
	return point(w1 * p[0] + w * q[0], w1 * p[1] + w * q[1], w1 * p[2] + w * q[2]);
}

point operator+(const point &p, const point &q)
{
	assert (p.IsNormal());
	assert (q.IsNormal());
	return point(p[0] + q[0], p[1] + q[1], p[2] + q[2]);
}

point &point::operator+=(const point &q)
{
	assert(IsNormal());
	assert(q.IsNormal());
	p[0] += q[0]; p[1] += q[1]; p[2] += q[2];
	return *this;
}

point operator*(float d, const point &p)
{
	assert(p.IsNormal());
	return point(d*p[0], d*p[1], d*p[2], p[3]);
}

point operator*(const point &p, float d)
{
	assert(p.IsNormal());
	return point(d*p[0], d*p[1], d*p[2], p[3]);
}

point &operator*=(point &p, float d)
{
	assert(p.IsNormal());

	p[0] *= d;
	p[1] *= d;
	p[2] *= d;

	return p;
}

point &operator/=(point &p, float d)
{
	assert(p.IsNormal());

	p[0] /= d;
	p[1] /= d;
	p[2] /= d;

	return p;
}

point operator/(const point &p, float d)
{
	assert(p.IsNormal());
	return point(p[0]/d, p[1]/d, p[2]/d, p[3]);
}

istream &operator>>(istream &input, point &p)
{
	input >> p[0] >> p[1] >> p[2];
	return input;
}

ostream &operator<<(ostream &output, const point &p)
{
	output << p[0] << " " << p[1] << " " << p[2];
	return output;
}

