// Test structures and classes for Problem 1 and 2
// Enjoy
// - Ed Keenan
#include <string.h>

struct Test_A
{
	Test_A()
		:r(2), g(3), b(4)
	{
	}
	char r;
	char g;
	char b;
};


struct Test_B
{
	Test_B()
		:x(0), y(0)
	{

	}
	int   x;
	int   y;
};


class Test_C
{
public:
	Test_C()
		:x(0), b(0)
	{

	}

private:
	int   x;
	char  b;
};

class Test_D
{
public:
	Test_D()
		:c(0x51), x(0x52), b(0x53)
	{
	}
private:
	char  c;
	int   x;
	char  b;
};

struct Test_E
{
	Test_E()
		:p(0), x(51), y(52), a(53), b(54), c(55)
	{
	}

	void  *p;
	float x;
	int   y;
	char  a;
	char  b;
	char  c;
};

struct Test_F
{

	Test_F()
		:a(0), t(0), s(0)
	{
	}
	char   a;
	double t;
	float  s;
};

struct Test_G
{
	Test_G()
		:t(0), s(0), a(0)
	{
	}
	double t;
	float  s;
	char   a;
};

struct vect
{
	vect()
		:vx(0), vy(0), d(0), vz(0), vw(0)
	{
	}
	float vx;
	float vy;
	char  d;
	float vz;
	float vw;
};

struct Test_H
{
	Test_H()
		:a(0), v1(), c(0), b(0)
	{
	}
	int		a;
	vect	v1;
	char	c;
	int		b;
};

class Test_I
{
public:
	Test_I()
		:x(), z(0), p(0)
	{
		memset(name, 0, sizeof(name));
	}
	char	name[63];
	int		x;
	char	z;
	int		*p;
};

struct Test_J
{
	Test_J()
		:x(0), z(0), d(0), p(0), c(0), y(0)
	{
		memset(name, 0, sizeof(name));
	}

	char	name[122];
	int		x;
	char	z;
	double	d;
	int		*p;
	char	c;
	float	y;

};
