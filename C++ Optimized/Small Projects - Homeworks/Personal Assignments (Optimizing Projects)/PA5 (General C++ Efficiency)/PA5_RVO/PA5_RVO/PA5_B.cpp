#include "PA5_B.h"


Vect2D Vect2D::operator + ( const Vect2D &tmp ) const
{
#if !RVO_Enable

	Vect2D sum;

	sum.x = this->x + tmp.x;
	sum.y = this->y + tmp.y;

	return (sum);

#else
	return Vect2D(this->x + tmp.x, this->y + tmp.y);
#endif
};

Vect2D Vect2D::operator - ( const Vect2D &tmp ) const
{
#if !RVO_Enable

	Vect2D sum;

	sum.x = this->x - tmp.x;
	sum.y = this->y - tmp.y;

	return ( sum );

#else
	return Vect2D(this->x - tmp.x, this->y - tmp.y);
#endif
};

Vect2D Vect2D::operator * ( const float scale ) const
{
#if !RVO_Enable

	Vect2D tmp;

	tmp.x = this->x * scale;//fixed typo from tmp.x to this->x
	tmp.y = this->y * scale;//fixed typo from tmp.x to this->x

	return ( tmp );

#else
	return Vect2D(this->x * scale, this->x * scale);
#endif
};

Vect2D Vect2D::operator / ( const float scale ) const
{
#if !RVO_Enable

	Vect2D tmp;

	tmp.x = x / scale;
	tmp.y = y / scale;

	return ( tmp );

#else
	return Vect2D(this->x / scale, this->x / scale);
#endif
};

