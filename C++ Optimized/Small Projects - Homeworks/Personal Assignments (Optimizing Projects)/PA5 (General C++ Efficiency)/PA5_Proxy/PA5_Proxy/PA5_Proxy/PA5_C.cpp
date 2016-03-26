#include "PA5_C.h"

#if !PROXY_ENABLE

Vect2D Vect2D::operator + (const Vect2D &tmp) const
{
	Vect2D sum;

	sum.x = this->x + tmp.x;
	sum.y = this->y + tmp.y;
	return (sum);//return VAddV instead of vect.

};

#endif

