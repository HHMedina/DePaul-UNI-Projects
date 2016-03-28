
#include "color.h"

rgb rgb::white(1.0, 1.0, 1.0);
rgb rgb::lightGray(.75, .75, .75);
rgb rgb::mediumGray(0.5, 0.5, 0.5);
rgb rgb::gray(0.5, 0.5, 0.5);
rgb rgb::darkGray(.25, .25, .25);
rgb rgb::black(0.0, 0.0, 0.0);
rgb rgb::red(1.0, 0.0, 0.0);
rgb rgb::green(0.0, 1.0, 0.0);
rgb rgb::blue(0.0, 0.0, 1.0);
rgb rgb::cyan(0.0, 1.0, 1.0);
rgb rgb::magenta(1.0, 0.0, 1.0);
rgb rgb::fuchsia(1.0, 0.0, 1.0);
rgb rgb::yellow(1.0, 1.0, 0.0);
rgb rgb::azure(0.0, 0.5, 1.0);
rgb rgb::violet(0.5, 0.0, 1.0);
rgb rgb::rose(1.0, 0.0, 0.5);
rgb rgb::orange(1.0, 0.5, 0.0);
rgb rgb::chartreuse(0.5, 1.0, 0.0);
rgb rgb::springGreen(0.0, 1.0, 0.5);
rgb rgb::brown(0.6, 0.3, 0.0); 
rgb rgb::tan(0.82, 0.71, 0.55);
rgb rgb::beige(0.96, 0.96, 0.86);
rgb rgb::skyBlue(0.53, 0.81, 0.92); 
rgb rgb::offWhite(1.0, .98, .94);

rgb &rgb::operator*=(double d)
{
	for (int i = 0; i < 3; i++)
		comp[i] *= (float)d;
	return *this;
}

rgb &rgb::operator*=(const rgb &c)
{
	for (int i = 0; i < 3; i++)
		comp[i] *= c.comp[i];
	return *this;
}

rgb &rgb::operator+=(const rgb &c)
{
	for (int i = 0; i < 3; i++)
		comp[i] *= c.comp[i];
	return *this;
}

rgb operator*(const rgb &c, double d)
{
	rgb tmp = c;
	tmp *= d;
	return tmp;
}

rgb operator*(double d, const rgb &c)
{
	rgb tmp = c;
	tmp *= d;
	return tmp;
}

rgb operator*(const rgb &c1, const rgb &c2)
{
	rgb tmp = c1;
	tmp *= c2;
	return tmp;
}

rgb operator+(const rgb &c1, const rgb &c2)
{
	rgb tmp = c1;
	tmp += c2;
	return tmp;
}

