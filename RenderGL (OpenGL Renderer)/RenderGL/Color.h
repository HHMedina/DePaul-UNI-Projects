#ifndef __COLOR_H__
#define __COLOR_H__

#include "jmisc.h"
#include "MiscMath.h"
#include <assert.h>
#include <iostream>

using namespace std;

// Uses floats to be compatible with OpenGL's color commands
class rgb
{
public:

	rgb(double R = 0.0, double G = 0.0, double B = 0.0, double A = 1.0) 
	{ Set(R, G, B, A); }

	void Set(double R, double G, double B, double A = 1.0)
	{ SetR(R); SetG(G); SetB(B); SetA(A); }

	void SetR(double R) { comp[0] = (float)R; }
	void SetG(double G) { comp[1] = (float)G; }
	void SetB(double B) { comp[2] = (float)B; }
	void SetA(double A) { comp[3] = (float)A; }

	float R(void) const { return comp[0]; }
	float G(void) const { return comp[1]; }
	float B(void) const { return comp[2]; }
	float A(void) const { return comp[3]; }

	void Clamp(void)
		{ for (int i = 0; i < 4; i++) comp[i] = (float)clamp(comp[i], 0.0, 1.0); }
	void multiply(rgb &res, const rgb &c1, const rgb &c2)
		{ for (int i = 0; i < 4; i++) res.comp[i] = c1.comp[i] * c2.comp[i]; }
	void add(rgb &res, const rgb &c1, const rgb &c2)
		{ for (int i = 0; i < 4; i++) res.comp[i] = c1.comp[i] + c2.comp[i]; }

	float *AsArray() { return comp; }
	const float *AsArray() const { return comp; }

	float &operator[](int i) { assert(i >= 0 && i <= 3); return comp[i]; } 
	const float &operator[](int i) const { assert(i >= 0 && i <= 3); return comp[i]; } 

	rgb &operator*=(double d);
	rgb &operator*=(const rgb &c1);
	rgb &operator+=(const rgb &c1);

	friend rgb operator*(const rgb &c, double d);
	friend rgb operator*(double d, const rgb &c);
	friend rgb operator*(const rgb &c1, const rgb &c2);
	friend rgb operator+(const rgb &c1, const rgb &c2);
	
	// Useful color definitions
	static rgb white;		// Grays
	static rgb lightGray;
	static rgb mediumGray;
	static rgb gray;
	static rgb darkGray;
	static rgb black;
	static rgb red;			// Primaries
	static rgb green;
	static rgb blue;
	static rgb cyan;		// Secondaries
	static rgb magenta;
	static rgb fuchsia;
	static rgb yellow;
	static rgb azure;		// Tertiaries
	static rgb violet;
	static rgb rose;
	static rgb orange;
	static rgb chartreuse;
	static rgb springGreen;
	static rgb brown;		// Misc colors
	static rgb tan;
	static rgb beige;
	static rgb skyBlue;
	static rgb offWhite;	// Same as "Floral White", slightly yellowish

private:

	float comp[4];
};

inline ostream &operator<<(ostream &output, const rgb &c)
{
	output << c.R() << ", " << c.G() << ", " << c.B() << ", " << c.A() << ")" << endl;
	return output;
}

#endif