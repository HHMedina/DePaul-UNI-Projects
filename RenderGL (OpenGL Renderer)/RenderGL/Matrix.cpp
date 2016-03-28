#include <iostream>
using namespace std;

#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include "jmisc.h"
#include "matrix.h"
#include "MiscMath.h"

template <class Item>
Item max(const Item& x, const Item& y)
{
	return (x < y ? y : x);
}

template <class Type>
Type min(const Type &x, const Type& y)
{
	return (x < y ? x : y);
}

template <class Type>
inline void swap(Type &x, Type &y)
{
	Type tmp = y;
	y = x; x = tmp;
}

matrix::matrix(int r, int c, float diagVal)
{
	m = NULL;
	mFloat = NULL;

	Resize(r, c, true);
	int lim = min(rows, cols);
	for (int i = 0; i < lim; i++)
		m[i*rows + i] = diagVal;
}

matrix::matrix(const matrix &m1) 
{
	m = NULL;
	mFloat = NULL;

	Resize(m1.Rows(), m1.Cols(), false);
	memcpy(m, m1.m, rows * cols * sizeof(float));
}	

void matrix::Resize(int r, int c, bool clearNewMem)
{
	if (m) delete [] m;
	if (mFloat) delete [] mFloat;

	rows = r;
	cols = c;
	m = new float[r*c];
	mFloat = new float[r*c];
	if (clearNewMem)
		memset(m, 0, r*c*sizeof(float));
}

void matrix::SwapRows(unsigned int i, unsigned int j)
{
	float tmp;
	float *d1 = m + i, *d2 = m + j;
	assert(i < rows && j < rows);

	if (i == j)
		return;

	for (unsigned int k = 0; k < cols; k++, d1 += cols, d2 += cols)
	{
		tmp = (*d1);
		(*d1) = (*d2);
		(*d2) = tmp;
	}
}

void matrix::AddRowMultiple(float d, unsigned int i, unsigned int j)
{
	float *d1 = m + i, *d2 = m + j;
	assert(i < rows && j < rows);

	for (unsigned int k = 0; k < cols; k++, d1 += cols, d2 += cols)
		(*d2) += d * (*d1);
}

void matrix::AddRowMultiple(float d, unsigned int i, unsigned  j, int startCol)
{
	float *d1 = m + i + startCol * cols, *d2 = m + j + startCol * cols;
	assert(i < rows && j < rows);

	for (unsigned int k = startCol; k < cols; k++, d1 += cols, d2 += cols)
		(*d2) += d * (*d1);
}

void matrix::MultiplyRow(float d, unsigned int i)
{
	float *d1 = m + i;
	assert(i < rows);

	for (unsigned int k = 0; k < cols; k++, d1 += cols)
		(*d1) *= d;
}

void matrix::Gauss(void)
{
	float pivotVal;
	int pivotIndex;
	float *d1;

	for (unsigned int i = 0; i < min(rows, cols); i++)
	{
		d1 = m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		if (IsNotZero(pivotVal))
		{
			SwapRows(i, pivotIndex);
			MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = i + 1; j < rows; j++)
				AddRowMultiple(-m[i*rows + j], i, j, i);
		}
	}
}

void matrix::GaussJordan(void)
{
	float pivotVal;
	int pivotIndex;
	float *d1;

	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		if (IsNotZero(pivotVal))
		{
			SwapRows(i, pivotIndex);
			MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				AddRowMultiple(-m[i*rows + j], i, j);
			}
		}
	}
}

float matrix::Invert(void) // returns the determinant i.e. the product of the pivots
{
	assert(rows == cols);

	matrix inv(rows, cols, 1.0f);
	float pivotVal;
	int pivotIndex;
	float *d1;
	float det = 1.0f;

	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		det *= pivotVal;
		if (IsNotZero(pivotVal))
		{
			SwapRows(i, pivotIndex);
			inv.SwapRows(i, pivotIndex);
			MultiplyRow(1.0f / pivotVal, i);
			inv.MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				float val = -m[i*rows + j];
				AddRowMultiple(val, i, j, i);
				inv.AddRowMultiple(val, i, j);
			}
		}
	}
	memcpy(m, inv.m, rows * cols * sizeof(float));
	return det;
}

matrix matrix::Inverse(void)
{
	assert(rows == cols);
	matrix inv(rows, cols, 1.0f);
	SolveGaussJordan(inv);
	return inv;
}

matrix matrix::Adjoint3x3(void)
{
	assert(rows == 3 && cols == 3);
	
	matrix tmp(3, 3);
	tmp.m[0] = m[rows + 1] * m[2*rows + 2] - m[2*rows + 1] * m[rows + 2];
	tmp.m[3] = m[2*rows + 1] * m[2] - m[1] * m[2*rows + 2];
	tmp.m[6] = m[1] * m[rows + 2] - m[rows + 1] * m[2];
	tmp.m[1] = m[rows + 2] * m[2*rows] - m[2*rows + 2] * m[rows];
	tmp.m[4] = m[2*rows + 2] * m[0] - m[2] * m[2*rows];
	tmp.m[7] = m[2] * m[rows] - m[rows + 2] * m[0];
	tmp.m[2] = m[rows] * m[2*rows + 1] - m[2*rows] * m[rows + 1];
	tmp.m[5] = m[2*rows] * m[1] - m[0] * m[2*rows + 1];
	tmp.m[8] = m[0] * m[rows + 1] - m[rows] * m[1];

	return tmp;
}

void matrix::SolveGaussJordan(matrix &RHS)
{
	assert(RHS.rows = rows);

	matrix tmp(*this);
	float pivotVal;
	int pivotIndex;
	float *d1;

	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = tmp.m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		if (IsNotZero(pivotVal))
		{
			tmp.SwapRows(i, pivotIndex);
			RHS.SwapRows(i, pivotIndex);
			tmp.MultiplyRow(1.0f / pivotVal, i);
			RHS.MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				float val = -tmp.m[i*rows + j];
				tmp.AddRowMultiple(val, i, j, i);
				RHS.AddRowMultiple(val, i, j);
			}
		}
	}
}

float matrix::Determinant(void)
{
	matrix tmp(*this);
	float pivotVal;
	int pivotIndex;
	float *d1;
	float det = 1.0f;

	for (unsigned int i = 0; i < rows; i++)
	{
		d1 = tmp.m + i * rows + i;
		pivotVal = *d1;
		pivotIndex = i;
		for (unsigned int j = i + 1; j < rows; j++)
		{
			d1++;
			if (fabs(*d1) > fabs(pivotVal))
			{
				pivotVal = *d1;
				pivotIndex = j;
			}
		}
		det *= pivotVal;
		if (IsNotZero(pivotVal))
		{
			tmp.SwapRows(i, pivotIndex);
			tmp.MultiplyRow(1.0f / pivotVal, i);
			for (unsigned int j = 0; j < rows; j++)
			{
				if (j == i)
					continue;
				float val = -tmp.m[i*rows + j];
				tmp.AddRowMultiple(val, i, j, i);
			}
		}
	}
	return det;
}

matrix matrix::Transpose(void)
{
	matrix t(rows, cols);
	for (unsigned int i = 0; i < rows; i++)
		for (unsigned int j = 0; j < cols; j++)
			t(i, j) = (*this)(j, i);
	return t;
}

void matrix::Copy(const matrix &m1)
{
	if (m1.rows * m1.cols != rows * cols)
	{
		delete [] m;
		delete [] mFloat;
		m = new float[m1.rows * m1.cols];
		mFloat = new float[m1.rows * m1.cols];
	}
	rows = m1.rows;
	cols = m1.cols;
	memcpy(m, m1.m, rows * cols * sizeof(float));
}

void matrix::Print(void) const
{
	for (unsigned int i = 0; i < rows; i++)
	{
		for (unsigned int j = 0; j < cols; j++)
		{
			cout << int(1000000 * m[rows * j + i] + .5) / 1000000.0f << "\t";
		}
		cout << endl;
	}
}

matrix &matrix::operator+=(const matrix &m2)
{
	const float *d1 = m, *d2 = m2.m;
	float *dr = m;
	int lim = rows * cols;

	assert(rows == m2.rows && cols == m2.cols);

	for (int i = 0; i < lim; i++, dr++, d1++, d2++)
		(*dr) = (*d1) + (*d2);
	return *this;
}

matrix operator+(const matrix &m1, const matrix &m2)
{
	matrix result(m1);
	result += m2;
	return result;
}

void Add(matrix &result, const matrix &m1, const matrix &m2)
{
	const float *d1 = m1.m, *d2 = m2.m;
	float *dr = result.m;
	int lim = m1.rows * m1.cols;

	assert(m1.rows == m2.rows && m1.cols == m2.cols);

	for (int i = 0; i < lim; i++, dr++, d1++, d2++)
		(*dr) = (*d1) + (*d2);
}

matrix &matrix::operator-=(const matrix &m2)
{
	const float *d1 = m, *d2 = m2.m;
	float *dr = m;
	int lim = rows * cols;

	assert(rows == m2.rows && cols == m2.cols);

	for (int i = 0; i < lim; i++, dr++, d1++, d2++)
		(*dr) = (*d1) - (*d2);
	return *this;
}

matrix operator-(const matrix &m1, const matrix &m2)
{
	matrix result(m1);
	result -= m2;
	return result;
}

void Subtract(matrix &result, const matrix &m1, const matrix &m2)
{
	const float *d1 = m1.m, *d2 = m2.m;
	float *dr = result.m;
	int lim = m1.rows * m1.cols;

	assert(m1.rows == m2.rows && m1.cols == m2.cols);

	for (int i = 0; i < lim; i++, dr++, d1++, d2++)
		(*dr) = (*d1) - (*d2);
}

matrix &matrix::operator*=(const matrix m2)
{
	matrix result(*this);
	Multiply(result, *this, m2);
	*this = result;
	return *this;
}

matrix operator*(const matrix &m1, const matrix &m2)
{
	matrix result(m1.rows, m2.cols);
	Multiply(result, m1, m2);
	return result;
}

void Multiply(matrix &result, const matrix &m1, const matrix &m2)
{
	const float *d1, *d2;
	float *dr;

	assert(m1.cols == m2.rows);
	assert(result.rows == m1.rows);
	assert(result.cols == m2.cols);

	dr = result.m;
	for (unsigned int j = 0; j < m2.cols; j++)
	{
		for (unsigned int i = 0; i < m1.rows; i++)
		{
			d1 = m1.m + i;
			d2 = m2.m + j * m2.rows;
			*dr = 0.0f;
			for (unsigned int k = 0; k < m1.cols; k++)
			{
				*dr += (*d1) * (*d2);
				d1 += m1.rows;
				d2++;
			}
			dr++;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
// Matrix Creation
////////////////////////////////////////////////////////////////////////////

void matrix::Identity(float d) // d defaults to 1.0f
{
	int lim = min(rows, cols);

	memset(m, 0, rows * cols * sizeof(float));
	for (int i = 0; i < lim; i++)
		m[i*rows + i] = d;
}

void matrix::Diagonal(float *v)
{
	int lim = min(rows, cols);

	memset(m, 0, rows * cols * sizeof(float));
	for (int i = 0; i < lim; i++)
		m[i*rows + i] = v[i];
}

////////////////////////////////////////////////////////////////////////////
// Test and Comparison
////////////////////////////////////////////////////////////////////////////

bool IsZero(const matrix &m)
{
	for (int i = 0; i < m.Rows(); i++)
	{
		for (int j = 0; j < m.Cols(); j++)
		{
			if (!IsZero(m(i, j)))
				return false;
		}
	}
	return true;
}

bool IsIdentity(const matrix &m)
{
	for (int i = 0; i < m.Rows(); i++)
	{
		for (int j = 0; j < m.Cols(); j++)
		{
			if (i == j)
			{
				if (!IsEqual(m(i, j), 1.0f))
					return false;
			}
			else
			{
				if (!IsZero(m(i, j)))
					return false;
			}
		}
	}
	return true;
}

bool IsEqual(const matrix &m1, const matrix &m2)
{
	if ((m1.Rows() != m2.Rows()) || (m1.Cols() != m2.Cols()))
		return false;

	for (int i = 0; i < m1.Rows(); i++)
	{
		for (int j = 0; j < m1.Cols(); j++)
		{
			if (!IsEqual(m1(i, j), m2(i, j)))
				return false;
		}
	}
	return true;
}

bool IsNotEqual(const matrix &m1, const matrix &m2)
{
	return !IsEqual(m1, m2);
}

////////////////////////////////////////////////////////////////////////////
// 3D Routines
////////////////////////////////////////////////////////////////////////////

void matrix::MakeRotationX(float alpha)
{
    float a = alpha * PIf / 180.0f;
    float c = cos(a);
    float s = sin(a);

	assert(rows == 4 && cols == 4);

    memset(m, 0, 16*sizeof(float));

    m[0] = 1.0f;
    m[5] = m[10] = c;
    m[6] = s;
    m[9] = -s;
    m[15] = 1.0f;
}

void matrix::MakeRotationY(float alpha)
{
    float a = alpha * PIf / 180.0f;
    float c = cos(a);
    float s = sin(a);

	assert(rows == 4 && cols == 4);

    memset(m, 0, 16*sizeof(float));

    m[5] = 1.0f;
    m[0] = m[10] = c;
    m[2] = -s;
    m[8] = s;
    m[15] = 1.0f;
}

void matrix::MakeRotationZ(float alpha)
{
    float a = alpha * PIf / 180.0f;
    float c = cos(a);
    float s = sin(a);

	assert(rows == 4 && cols == 4);

    memset(m, 0, 16*sizeof(float));

    m[10] = 1.0f;
    m[0] = m[5] = c;
    m[1] = s;
    m[4] = -s;
    m[15] = 1.0f;
}

#define sgn(x) ((x) < 0 ? -1 : 1)

void matrix::MakeRotation(float angle, vector axis)
{
    if (IsZero(axis[0]) && IsZero(axis[1]))
    {
    	MakeRotationZ(sgn(axis[2])*angle);
    	return;
    }

	assert(rows == 4 && cols == 4);

	axis.Normalize();

    float a = angle * PIf / 180.0f;
    float c = cos(a);
    float s = sin(a);
    float D = sqr(axis[0]) + sqr(axis[1]);
    float u0u1 = axis[0]*axis[1];
    float u0u2 = axis[0]*axis[2];
    float u1u2 = axis[1]*axis[2];
    float N = c*u0u1*(sqr(axis[2])-1)/D;

    m[0] = c*(sqr(axis[1])+sqr(u0u2))/D + sqr(axis[0]);
    m[1] = N + s*axis[2] + u0u1;
    m[2] = -axis[1]*s + u0u2*(1-c);
    m[3] = 0.0f;
    m[4] = N - s*axis[2] + u0u1;
    m[5] = c*(sqr(axis[0])+sqr(u1u2))/D + sqr(axis[1]);
    m[6] = axis[0]*s + u1u2*(1-c);
    m[7] = 0.0f;
    m[8] = axis[1]*s + u0u2*(1-c);
    m[9] = -axis[0]*s + u1u2*(1-c);
    m[10]= c*D + sqr(axis[2]);
    m[11]= m[12] = m[13] = m[14] = 0.0f;
    m[15]= 1.0f;
}

void matrix::MakeTranslation(float x, float y, float z)
{
	Identity();
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

void matrix::MakeScale(float x, float y, float z)
{
	Identity();
	m[0] = x;
	m[5] = y;
	m[10] = z;
}

matrix matrix::Identity(int size) { return matrix(size, size, 1.0f); }
matrix matrix::Zero(int size) { return matrix(size, size, 0.0f); }
matrix matrix::RotationX(float alpha) { matrix m(4, 4); m.MakeRotationX(alpha); return m; }
matrix matrix::RotationY(float alpha) { matrix m(4, 4); m.MakeRotationY(alpha); return m; }
matrix matrix::RotationZ(float alpha) { matrix m(4, 4); m.MakeRotationZ(alpha); return m; }
matrix matrix::Rotation(float alpha, const vector &v) { matrix m(4, 4); m.MakeRotation(alpha, v); return m; }
matrix matrix::Translation(float x, float y, float z) { matrix m(4, 4); m.MakeTranslation(x, y, z); return m; }
matrix matrix::Scale(float x, float y, float z) { matrix m(4, 4); m.MakeScale(x, y, z); return m; }

void Multiply(point &result, const matrix &m, const point &p)
{
	assert(m.rows <= 4 && m.rows >= 3 && m.cols <= 4 && m.cols >= 3);

	float X = p[0], Y = p[1], Z = p[2], W = p[3]; // Just in case result == p 
	if (m.rows == 3)
	{
		if (m.cols == 4)
		{
			result[0] = m.m[0]*X + m.m[3]*Y + m.m[6]*Z + m.m[9]*W;
			result[1] = m.m[1]*X + m.m[4]*Y + m.m[7]*Z + m.m[10]*W;
			result[2] = m.m[2]*X + m.m[5]*Y + m.m[8]*Z + m.m[11]*W;
			result[3] = 1.0f;
		}
		else
		{
			result[0] = m.m[0]*X + m.m[3]*Y + m.m[6]*Z;
			result[1] = m.m[1]*X + m.m[4]*Y + m.m[7]*Z;
			result[2] = m.m[2]*X + m.m[5]*Y + m.m[8]*Z;
			result[3] = 1.0f;
		}
	}
	else if (m.rows == 4)
	{
		result[0] = m.m[0]*X + m.m[4]*Y + m.m[8]*Z + m.m[12]*W;
		result[1] = m.m[1]*X + m.m[5]*Y + m.m[9]*Z + m.m[13]*W;
		result[2] = m.m[2]*X + m.m[6]*Y + m.m[10]*Z + m.m[14]*W;
		result[3] = m.m[3]*X + m.m[7]*Y + m.m[11]*Z + m.m[15]*W;
	}
}

point operator*(const matrix &m, const point &p)
{
	point result;
	Multiply(result, m, p);
	return result;
}

void Multiply(vector &result, const matrix &m, const vector &v)
{
	assert(m.rows <= 4 && m.rows >= 3 && m.cols <= 4 && m.cols >= 3);

	float X = v[0], Y = v[1], Z = v[2]; // Just in case result == v
	if (m.rows == 3)
	{
		result[0] = m.m[0]*X + m.m[3]*Y + m.m[6]*Z;
		result[1] = m.m[1]*X + m.m[4]*Y + m.m[7]*Z;
		result[2] = m.m[2]*X + m.m[5]*Y + m.m[8]*Z;
	}
	else if (m.rows == 4)
	{
		result[0] = (m.m[0]*X + m.m[4]*Y + m.m[8]*Z);
		result[1] = (m.m[1]*X + m.m[5]*Y + m.m[9]*Z);
		result[2] = (m.m[2]*X + m.m[6]*Y + m.m[10]*Z);
	}
}

vector operator*(const matrix &m, const vector &v)
{
	vector result;
	Multiply(result, m, v);
	return result;
}