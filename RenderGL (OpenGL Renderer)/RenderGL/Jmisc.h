// jmisc.h

#ifndef __JMISC_H__
#define __JMISC_H__

#define NOMINMAX

#include <math.h>
#include <stdarg.h>

const double PI = 3.14159265358979323;
const float PIf = 3.1415926536f;

#ifndef	uint
#define uint unsigned int
#endif

bool IsWhitespace(char k);

/*
template <class Item>
Item max(const Item& x, const Item& y)
{
	return (x < y ? y : x);
}

template <class Type>
Type min(const Type &x, const Type& y)
{
	return (x < y ? x : y);
}

template <class Type>
inline void swap(Type &x, Type &y)
{
     Type tmp = y;
     y = x; x = tmp;
}
*/

template <class Type>
inline Type sqr(const Type& x)
{
	return x*x;
}
 
inline double dist(double x1, double y1, double x2, double y2)
{
	return sqrt(sqr(x2 - x1) + sqr(y2 - y1));
}

inline double dist(double *p1, double *p2)
{
	double len = 0;

	for (int i = 0; i < 3; i++)
		len += sqr(p2[i] - p1[i]);

	return sqrt(len);
}

void jMessageBox(char *message, ...);
void complain(char *str, ...);

// The following are declared __cdecl, in case someone decides to 
// use this file in a project using __fastcall.
void __cdecl unitVectorAverage(float ave[3], int n, ...);
void __cdecl unitVectorAverage(float ave[3], int n, float vects[][3]);
void __cdecl vectorAverage(float ave[3], int n, float vects[][3]);

double ran1(void);
void sran1(long Seed);
double normalRan(void);

#endif
