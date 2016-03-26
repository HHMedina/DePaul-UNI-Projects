#ifndef MATRIX_SIMD_H
#define MATRIX_SIMD_H

#include "Vect4D_SIMD.h"

#include <xmmintrin.h>
#include <smmintrin.h> 




class Matrix_SIMD
{
public:
	Matrix_SIMD()
		:v0({ 0.0f, 0.0f, 0.0f, 0.0f }), v1({ 0.0f, 0.0f, 0.0f, 0.0f }), v2({ 0.0f, 0.0f, 0.0f, 0.0f }), v3({ 0.0f, 0.0f, 0.0f, 0.0f })
	{
	}

	Matrix_SIMD( const Vect4D_SIMD &tV0, const Vect4D_SIMD &tV1, const Vect4D_SIMD &tV2, const Vect4D_SIMD &tV3)
		:v0(tV0), v1(tV1), v2(tV2), v3(tV3)
	{
	}

	Matrix_SIMD(const __m128 &m0, const __m128 &m1, const __m128 &m2, const __m128 &m3)
		:v0(m0), v1(m1), v2(m2), v3(m3)
	{
	}
	
	Matrix_SIMD(const Matrix_SIMD& rhs)
		:v0(rhs.v0), v1(rhs.v1), v2(rhs.v2), v3(rhs.v3)
	{
	}

	void operator =(const Matrix_SIMD& rhs)
	{
		//copy m because of a single copy instruction for each m instead of 4 for each vect
		this->v0.m = rhs.v0.m;
		this->v1.m = rhs.v1.m;
		this->v2.m = rhs.v2.m;
		this->v3.m = rhs.v3.m;
	}

	~Matrix_SIMD(){}

	Matrix_SIMD operator * ( const Matrix_SIMD & t);

	
public:

	union 
		{
		struct 
			{
			Vect4D_SIMD v0;
			Vect4D_SIMD v1;
			Vect4D_SIMD v2;
			Vect4D_SIMD v3;
			};

		struct 
			{
			float m0;
			float m1;
			float m2;
			float m3;
			float m4;
			float m5;
			float m6;	
			float m7;
			float m8;
			float m9;
			float m10;
			float m11;
			float m12;
			float m13;
			float m14;
			float m15;
			};
		};
};

#endif