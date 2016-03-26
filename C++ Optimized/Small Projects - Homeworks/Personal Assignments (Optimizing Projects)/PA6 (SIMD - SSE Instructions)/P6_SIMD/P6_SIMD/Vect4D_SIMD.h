#ifndef VECT4D_SIMD_H
#define VECT4D_SIMD_H

#include <xmmintrin.h>
#include <smmintrin.h>  

class Matrix_SIMD;

class Vect4D_SIMD
{
public:

	Vect4D_SIMD()
		:x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{
	}

	Vect4D_SIMD( const Vect4D_SIMD &tmp)
		:x(tmp.x), y(tmp.y), z(tmp.z), w(tmp.w)
	{
	}

	Vect4D_SIMD( const float tx, const float ty, const float tz, const float tw )
		:x(tx), y(ty), z(tz), w(tw)
	{
	}

	~Vect4D_SIMD()
	{
	}

	void operator = (const Vect4D_SIMD &tmp)
	{
		m = tmp.m;//single copy instruction as opposed to 4
	}

	void set( const float xVal, const float yVal, const float zVal, const float wVal )
	{
		x = xVal;
		y = yVal;
		z = zVal;
		w = wVal;
	}

	Vect4D_SIMD operator + (const Vect4D_SIMD &tmp)
	{		
		return Vect4D_SIMD(_mm_add_ps(this->m, tmp.m));
	}

	void operator += (const Vect4D_SIMD &tmp)
	{
		this->m = _mm_add_ps(this->m,tmp.m); //perform on this vector.
		
	}

	Vect4D_SIMD operator - (const Vect4D_SIMD &tmp)
	{
		return Vect4D_SIMD(_mm_sub_ps(this->m, tmp.m));
	}

	void operator -= (const Vect4D_SIMD &tmp)
	{
		this->m = _mm_sub_ps(this->m, tmp.m); //perform on this vector.
		
	}

	Vect4D_SIMD operator * (const Vect4D_SIMD &tmp)
	{
		return Vect4D_SIMD(_mm_mul_ps(this->m, tmp.m));
	};

	void operator *= (const Vect4D_SIMD &tmp)
	{
		this->m = _mm_mul_ps(this->m, tmp.m); //perform on this vector.
		
	}

	Vect4D_SIMD operator / (const Vect4D_SIMD &tmp)
	{
		return Vect4D_SIMD(_mm_div_ps(this->m, tmp.m));
	}

	void operator /= (const Vect4D_SIMD &tmp)
	{
		this->m = _mm_div_ps(this->m, tmp.m); //perform on this vector.
		
	}

	float dot( const Vect4D_SIMD &tmp )
	{
		return _mm_dp_ps(this->m, tmp.m, 0xF4).m128_f32[2]; //access directly and avoid temp vect4D
	}

	Vect4D_SIMD Vect4D_SIMD::operator * ( const Matrix_SIMD &m);

	Vect4D_SIMD(const __m128 mVal)
		:m(mVal)
	{
	}

public:

	// Level 4 complains nameless struct/union ...
	#pragma warning( disable : 4201)

	// anonymous union
	union 
	{
		__m128	m;

		// anonymous struct
		struct 
			{
			float x;
			float y;
			float z;
			float w;
			};
	};
};

#endif



