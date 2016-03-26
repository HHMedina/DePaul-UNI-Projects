// Vect4D.h
//
// This is a 4 dimensional Vect4D class
#ifndef Vect4D_H
#define Vect4D_H

// includes
#include "Enum.h"

#include <xmmintrin.h>
#include <smmintrin.h>

// Foward Declarations
class Matrix;

// class
class Vect4D
{
public:
	friend class Matrix;
	friend class MxM;
	friend class MxMxM;
	friend class MxMxMxM;
	friend class MxMxMxMxM;
	
	Vect4D();	
	Vect4D(const float tx, const float ty, const float tz, const float tw = 1.0f);
	Vect4D(const Vect4D& rhs);
	Vect4D(const __m128&);
	Vect4D::Vect4D(const Vect4D& rhs, float _w);
	void operator = (const Vect4D& rhs);
	~Vect4D();



	void norm( Vect4D &out );
	void set(const float tx, const float ty, const float tz, const float tw = 1.0f);
	
	Vect4D operator + ( const Vect4D& t );
	Vect4D operator - ( const Vect4D& t );
	Vect4D operator * (const float scale);

	void operator += (const Vect4D &tmp)
	{
		this->m = _mm_add_ps(this->m, tmp.m); //perform on this vector.
		m.m128_f32[3] = 1.0f;
	}

	void operator -= (const Vect4D &tmp)
	{
		this->m = _mm_sub_ps(this->m, tmp.m); //perform on this vector.
		m.m128_f32[3] = 1.0f;
	}

	void operator *= (const float tmp)
	{
		this->m = _mm_mul_ps(this->m, _mm_load1_ps(&tmp)); //perform on this vector.

	}

	void Cross(const Vect4D& vin, Vect4D &vout);

	float &operator[](const VECT_ENUM e);


public:

	// Level 4 complains nameless struct/union ...
	#pragma warning( disable : 4201)
	// anonymous union
	union
	{
		__m128	m;

		float fArr[4];

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

#endif  //Vect4D.h
