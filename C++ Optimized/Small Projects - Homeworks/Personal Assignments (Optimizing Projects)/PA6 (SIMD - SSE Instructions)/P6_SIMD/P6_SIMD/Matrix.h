#ifndef MATRIX_H
#define MATRIX_H

#include "Vect4D.h"

class Matrix
{
public:


	Matrix()
		:v0({ 0.0f, 0.0f, 0.0f, 0.0f }), v1({ 0.0f, 0.0f, 0.0f, 0.0f }), v2({ 0.0f, 0.0f, 0.0f, 0.0f }), v3({ 0.0f, 0.0f, 0.0f, 0.0f })
	{
	};

	Matrix( const Vect4D &tv0, const Vect4D &tv1, const Vect4D &tv2, const Vect4D &tv3)
	:v0(tv0),v1(tv1),v2(tv2),v3(tv3)
	{
	};

	Matrix(const Matrix& rhs)
		:v0(rhs.v0), v1(rhs.v1), v2(rhs.v2), v3(rhs.v3)
	{
	}

	void operator =(const Matrix& rhs)
	{
		this->v0 = rhs.v0;
		this->v1 = rhs.v1;
		this->v2 = rhs.v2;
		this->v3 = rhs.v3;
	}

	~Matrix(){}

	Matrix operator * (const Matrix &t);


// Level 4 complains nameless struct/union ...
#pragma warning( disable : 4201)

union 
	{

	struct 
		{
		Vect4D v0;
		Vect4D v1;
		Vect4D v2;
		Vect4D v3;
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