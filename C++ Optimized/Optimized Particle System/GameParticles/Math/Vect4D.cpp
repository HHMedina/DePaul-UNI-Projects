#include <math.h>
#include <assert.h>

#include "Vect4D.h"

//newest

Vect4D::Vect4D()
	:m(_mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f))
{
}

Vect4D::Vect4D(const Vect4D& rhs)
	: m(rhs.m)
{
}

Vect4D::Vect4D(const Vect4D& rhs,float _w)
	: m(rhs.m),w(_w)
{
}

Vect4D::Vect4D(const float tx, const float ty,const float tz,const float tw)
	: x(tx), y(ty), z(tz), w(tw)
{
}

Vect4D::Vect4D(const __m128& rhs)
	: m(rhs)
{
	
}

void Vect4D::operator =(const Vect4D& rhs)
{
	m = rhs.m;
}

Vect4D::~Vect4D()
{
	// nothing to delete
}

void Vect4D::norm(Vect4D& out)
{
	
	__m128 mag = _mm_dp_ps(out.m, out.m,0x7f);

	if (0.0f < mag.m128_f32[0])
	{
		out.m = _mm_mul_ps(_mm_rsqrt_ps(mag), m);

	}
	
}

Vect4D Vect4D::operator + ( const Vect4D& t ) 
{
	
	return Vect4D(_mm_add_ps(m,t.m),1.0f);
	
}

Vect4D Vect4D::operator - ( const Vect4D& t )
{	
	return Vect4D(_mm_sub_ps(m, t.m),1.0f);
}

Vect4D Vect4D::operator *(const float scale)
{
	return Vect4D(_mm_mul_ps(m,_mm_load1_ps(&scale)));
}

float& Vect4D::operator[](const VECT_ENUM e)
{
	return fArr[e];
	
}

void Vect4D::Cross( const Vect4D& vin, Vect4D& vout)
{

	vout.m = _mm_sub_ps(
		_mm_mul_ps(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(vin.m, vin.m, _MM_SHUFFLE(3, 1, 0, 2))),
		_mm_mul_ps(_mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(vin.m, vin.m, _MM_SHUFFLE(3, 0, 2, 1)))
		);
	
}

void Vect4D::set(const float tx, const float ty, const float tz, const float tw)
{
	x = tx;
	y = ty;
	z = tz;
	w = tw;
	
	//m = _mm_set_ps(tw,tz,ty,tx);

}

// End of file