#ifndef MATRIX_H
#define MATRIX_H

// includes
#include "Enum.h"

// forward declare
#include "Vect4D.h"

#include <xmmintrin.h>
#include <smmintrin.h>

// class
class Matrix
{
public:

	//friend to all of the rvo classes
	friend class MxM;
	friend class MxMxM;
	friend class MxMxMxM;
	friend class MxMxMxMxM;
	// local enumerations
	enum MatrixRowEnum
	{
		MATRIX_ROW_0 =0,
		MATRIX_ROW_1 =1,
		MATRIX_ROW_2 =2,
		MATRIX_ROW_3 =3
	};

	Matrix();	
	Matrix( const Matrix& t );
	Matrix(const __m128& newV0, const __m128& newV1, const __m128& newV2, const __m128& newV3);
	Matrix(const float f0, const float f1, const float f2, const float f3,
		const float f4, const float f5, const float f6, const float f7,
		const float f8, const float f9, const float f10, const float f11,
		const float f12, const float f13, const float f14, const float f15);
	void operator = (const Matrix& rhs) ;
	~Matrix();

	void set( const MatrixRowEnum row, const Vect4D *t );
	void get( const MatrixRowEnum row, Vect4D *vOut );
	void set(const MatrixRowEnum row, const float f0, const float f1, const float f2, const float f3 );

	void setIdentMatrix();
	void setTransMatrix(const Vect4D *t );
	void setScaleMatrix(const Vect4D *s );
	void setRotZMatrix(const float Z_Radians);

	float &operator[](const INDEX_ENUM e);
	
	Matrix operator*( const Matrix &t );
	void operator*=(const Matrix &t);
	void operator-=(const Matrix &t);
	Matrix operator-(const Matrix &t);
	Matrix operator*(const float s);

	float Determinant();
	
	Matrix GetAdjugate();
	void MorphIntoAdjugate();
	void Matrix::operator/=(const float t);
	
	void Matrix::Inverse( Matrix &out );
	
public:

	union
	{
		__m128 m128Arr[4];// for fast access of elements

		float mFloatArr[16]; // for fast access of elements

		struct
		{
			Vect4D v0;
			Vect4D v1;
			Vect4D v2;
			Vect4D v3;
		};

		struct
		{
			__m128 mm0;
			__m128 mm1;
			__m128 mm2;
			__m128 mm3;
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



/*

//VAddV Vect + Vect
class MxM
{
public:

	const Matrix& M1;
	const Matrix& M2;

	MxM(const Matrix& t1, const Matrix& t2)
		:M1(t1), M2(t2)
	{
	}

	operator Matrix()  //conversion to Vect2D (final step)
	{
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m0), M2.v0.m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m1), M2.v1.m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m2), M2.v2.m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m3), M2.v3.m);

		__m128 newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m4), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m5), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m6), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m7), M2.v3.m);

		__m128 newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m8), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m9), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m10), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m11), M2.v3.m);

		__m128 newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m12), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m13), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m14), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m15), M2.v3.m);

		__m128 newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		return Matrix(newX, newY, newZ, newW);
	}


	~MxM(){}

private:
	MxM();

	void operator =(const MxM& rhs);

};

inline MxM operator * (const Matrix& t1, const Matrix& t2)
{
	return MxM(t1, t2);
}

class MxMxM
{
public:

	const Matrix& M1;
	const Matrix& M2;
	const Matrix& M3;

	MxMxM(const MxMxM&);
	MxMxM(const MxM& t1, const Matrix& t2)
		:M1(t1.M1),M2(t1.M2), M3(t2)
	{
	}

	operator Matrix()  //conversion to Vect2D (final step)
	{
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m0), M2.v0.m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m1), M2.v1.m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m2), M2.v2.m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m3), M2.v3.m);

		__m128 newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m4), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m5), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m6), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m7), M2.v3.m);

		__m128 newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m8), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m9), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m10), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m11), M2.v3.m);

		__m128 newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m12), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m13), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m14), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m15), M2.v3.m);

		__m128 newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));


		multCol0 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[3]), M3.v3.m);

		newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[3]), M3.v3.m);

		
		newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[3]), M3.v3.m);

		
		newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[3]), M3.v3.m);

		
		newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		return Matrix(newX, newY, newZ, newW);
	}


	~MxMxM(){}

private:
	MxMxM();
	MxMxM& operator =(const MxMxM& );
};

inline MxMxM operator * (const MxM& t1, const Matrix& t2)
{
	return MxMxM(t1, t2);
}


class MxMxMxM
{
public:

	const Matrix& M1;
	const Matrix& M2;
	const Matrix& M3;
	const Matrix& M4;

	MxMxMxM(const MxMxM& t1, const Matrix& t2)
		:M1(t1.M1), M2(t1.M2), M3(t1.M3), M4(t2)
	{
	}

	MxMxMxM(const MxMxMxM&);

	operator Matrix()  //conversion to Vect2D (final step)
	{
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m0), M2.v0.m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m1), M2.v1.m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m2), M2.v2.m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m3), M2.v3.m);

		__m128 newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m4), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m5), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m6), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m7), M2.v3.m);

		__m128 newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m8), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m9), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m10), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m11), M2.v3.m);

		__m128 newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m12), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m13), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m14), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m15), M2.v3.m);

		__m128 newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));


		multCol0 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[3]), M3.v3.m);

		newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[3]), M3.v3.m);


		newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[3]), M3.v3.m);


		newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[3]), M3.v3.m);


		newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[3]), M4.v3.m);

		newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[3]), M4.v3.m);


		newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[3]), M4.v3.m);


		newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[3]), M4.v3.m);


		newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));



		return Matrix(newX, newY, newZ, newW);
	}


	~MxMxMxM(){}

private:
	MxMxMxM();
	MxMxMxM& operator = (const MxMxMxM& );
};

inline MxMxMxM operator * (const MxMxM& t1, const Matrix& t2)
{
	return MxMxMxM(t1, t2);
}

class MxMxMxMxM
{
public:

	const Matrix& M1;
	const Matrix& M2;
	const Matrix& M3;
	const Matrix& M4;
	const Matrix& M5;

	MxMxMxMxM(const MxMxMxM& t1, const Matrix& t2)
		:M1(t1.M1), M2(t1.M2), M3(t1.M3), M4(t1.M4), M5(t2)
	{
	}

	operator Matrix()  //conversion to Vect2D (final step)
	{
		__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m0), M2.v0.m);
		__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m1), M2.v1.m);
		__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m2), M2.v2.m);
		__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m3), M2.v3.m);

		__m128 newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m4), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m5), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m6), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m7), M2.v3.m);

		__m128 newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m8), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m9), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m10), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m11), M2.v3.m);

		__m128 newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&M1.m12), M2.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&M1.m13), M2.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&M1.m14), M2.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&M1.m15), M2.v3.m);

		__m128 newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));


		multCol0 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[3]), M3.v3.m);

		newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[3]), M3.v3.m);


		newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[3]), M3.v3.m);


		newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[0]), M3.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[1]), M3.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[2]), M3.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[3]), M3.v3.m);


		newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[3]), M4.v3.m);

		newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[3]), M4.v3.m);


		newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[3]), M4.v3.m);


		newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[0]), M4.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[1]), M4.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[2]), M4.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[3]), M4.v3.m);


		newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[0]), M5.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[1]), M5.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[2]), M5.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newX.m128_f32[3]), M5.v3.m);

		newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[0]), M5.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[1]), M5.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[2]), M5.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newY.m128_f32[3]), M5.v3.m);


		newY = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[0]), M5.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[1]), M5.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[2]), M5.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newZ.m128_f32[3]), M5.v3.m);


		newZ = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

		multCol0 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[0]), M5.v0.m);
		multCol1 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[1]), M5.v1.m);
		multCol2 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[2]), M5.v2.m);
		multCol3 = _mm_mul_ps(_mm_load1_ps(&newW.m128_f32[3]), M5.v3.m);


		newW = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));



		return Matrix(newX, newY, newZ, newW);
	}


	~MxMxMxMxM(){}
	MxMxMxMxM(const MxMxMxMxM&);
private:
	MxMxMxMxM();
	MxMxMxMxM& operator =(const MxMxMxMxM& );
};

inline MxMxMxMxM operator * (const MxMxMxM& t1, const Matrix& t2)
{
	return MxMxMxMxM(t1, t2);
}
*/
#endif  // Matrix.h
