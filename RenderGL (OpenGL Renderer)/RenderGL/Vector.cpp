#pragma warning(disable : 4996) // Turns off secure_crt warnings in VS 2005 and up

#include <math.h>
#include "jmisc.h" // Miscellaneous routines, including sqr()
#include "vector.h"

////////////////////////////////////////////////////////////////////////////
// Methods of the vector class
////////////////////////////////////////////////////////////////////////////

vector::vector(float x, float y, float z)
{
	v[0] = x; 
	v[1] = y; 
	v[2] = z;
	v[3] = 0.0;
}

float vector::Length(void) const 
{
	return sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
}

void vector::Normalize(void)
{
	// Check if it is the zero vector.
	float len = Length();
	assert(IsNotZero(len));

	v[0] /= len; v[1] /= len; v[2] /= len; 
}

vector &vector::operator+=(const vector &w)
{
	v[0] += w[0]; v[1] += w[1]; v[2] += w[2];
	return *this;
}

vector &vector::operator-=(const vector &w)
{
	v[0] -= w[0]; v[1] -= w[1]; v[2] -= w[2];
	return *this;
}

vector &vector::operator*=(float d)
{
	v[0] *= d; v[1] *= d; v[2] *= d;
	return *this;
}

vector &vector::operator/=(float d)
{
	assert(IsNotZero(d));

	v[0] /= d; v[1] /= d; v[2] /= d;
	return *this;
}

// Calculates the projection of v onto w
vector vector::Proj(const vector &w)
{
	vector tmpV = *this;   // C++ syntax to query the value of "this" object
	assert(IsNotZero(w.Length()));

	float lenWSqr = Dot(w, w);
	return Dot(tmpV, w) / lenWSqr * w;
}

vector vector::Perp(const vector &w)
{
	vector tmpV = *this;
	return tmpV - tmpV.Proj(w);
}

bool IsZero(const vector &v)
{
	return (IsZero(v[0]) && IsZero(v[1]) && IsZero(v[2]));
}

bool IsEqual(const vector &v, const vector &w)
{
	return (IsEqual(v[0], w[0]) && IsEqual(v[1], w[1]) && IsEqual(v[2], w[2]));
}

bool IsNotEqual(const vector &v, const vector &w)
{ 
	return !IsEqual(v, w); 
}

float Dot(const vector &v, const vector &w)
{
	return v[0]*w[0] + v[1]*w[1] + v[2]*w[2];
}

vector Cross(const vector &v, const vector &w)
{
	return vector(v[1]*w[2] - v[2]*w[1],
		          v[2]*w[0] - v[0]*w[2],
				  v[0]*w[1] - v[1]*w[0]);
}

vector operator+(const vector &v, const vector &w)
{
	return vector(v[0] + w[0], v[1] + w[1], v[2] + w[2]);
}

vector operator-(const vector &v, const vector &w)
{
	return vector(v[0] - w[0], v[1] - w[1], v[2] - w[2]);
}

vector operator*(float d, const vector &v)
{
	return vector(d * v[0], d * v[1], d * v[2]);
}

vector operator*(const vector &v, float d)
{
	return d * v;
}

vector operator/(const vector &v, float d)
{
	assert(IsNotZero(d));

	return vector(v[0] / d, v[1] / d, v[2] / d);
}

point operator+(const point &p, const vector &v)
{
	return point(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
}

// Note that this one really doesn't make sense geometrically, 
// but sometimes we define certain operators for symmetry and 
// for ease of programming.  But you should be careful to see
// what the operation means when you do it.
point operator+(const vector &v, const point &p)
{
	return p + v;
}

point &operator+=(point &p, const vector &v)
{
	assert(p.IsNormal());
	p[0] += v[0]; p[1] += v[1]; p[2] += v[2];
	return p;
}

point operator-(const point &p, const vector &v)
{
	assert (p.IsNormal());
	return point(p[0] - v[0], p[1] - v[1], p[2] - v[2]);
}

point &operator-=(point &p, const vector &v)
{
	assert (p.IsNormal());
	p[0] -= v[0]; p[1] -= v[1]; p[2] -= v[2];
	return p;
}

vector operator-(const point &p, const point &q)
{
	assert (p.IsNormal());
	assert (q.IsNormal());
	return vector(p[0] - q[0], p[1] - q[1], p[2] - q[2]);
}

vector operator-(const vector &v)
{
	return vector(-v[0], -v[1], -v[2]);
}

istream &operator>>(istream &input, vector &v)
{
	input >> v[0] >> v[1] >> v[2];
	return input;
}

ostream &operator<<(ostream &output, const vector &v)
{
	output << v[0] << " " << v[1] << " " << v[2];
	return output;
}
