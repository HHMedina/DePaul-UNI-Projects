#ifndef __MATRIX_H__
#define __MATRIX_H__

// This is designed to be an OpenGL compatible matrix class.
// Therefore, the matrix is stored in column major order, i.e.
// with successive elements going down columns instead of going across rows.
//
// Notice that this matrix class is NOT organized internally like a 
// two dimensional array!
//
// Note also that we DON'T access elements of the matrix with [i][j], but
// with the ( ) operator, i.e. m(i, j);   This allows us to control access
// for BOTH indices!

#include "point.h"
#include "vector.h"

#include <assert.h>

class matrix
{
public:

	matrix(int r = 4, int c = 4, float diagVal = 1.0);
	matrix(const matrix &m1);
	~matrix() { delete [] m; }

	void Resize(int r, int c, bool clearNewMem = true);

	int Rows(void) const { return rows; }
	int Cols(void) const { return cols; }

	matrix& operator=(const matrix &m1) { Copy(m1); return *this; }

	float &operator()(unsigned int i, unsigned int j) { assert(i < rows && j < cols); return m[rows * j + i]; }
	float operator()(unsigned int i, unsigned int j) const { assert(i < rows && j < cols); return m[rows * j + i]; }

	operator const float *(void) { return m; }

	// Warning:  This method gives unrestricted access to the internal
	// array and is a possiblitity for memory corruption.
	float *AsArray(void) { return m; }
	const float *AsArray(void) const { return m; }

	const float *AsFloatArray(void) const { for (unsigned int i = 0; i < rows * cols; i++) mFloat[i] = (float)m[i]; return mFloat; }

	void Print(void) const;

	void Copy(const matrix &m1);
	void SwapRows(unsigned int i, unsigned int j);
	void AddRowMultiple(float d, unsigned int i, unsigned int j);
	void AddRowMultiple(float d, unsigned int i, unsigned int j, int startCol);
	void MultiplyRow(float d, unsigned int i);
	void Gauss(void);
	void GaussJordan(void);
	void SolveGaussJordan(matrix &RHS);
	matrix Inverse(void);
	float Invert(void);
	matrix Adjoint3x3(void);
	matrix Transpose(void);

	float Determinant(void);

	void Identity(float d = 1.0);
	void Diagonal(float *v);
	void MakeRotationX(float alpha);
	void MakeRotationY(float alpha);
	void MakeRotationZ(float alpha);
	void MakeRotation(float angle, vector axis);
	void MakeTranslation(float x, float y, float z);
	void MakeScale(float x, float y, float z);

	// These are the static matrix methods that allow you to construct matrices without an explicit object	
	//		e.g. matrix m = matrix::RotationZ(30) * matrix::rotationY(20);
	static matrix Identity(int size);
	static matrix Zero(int size);
	static matrix RotationX(float alpha);
	static matrix RotationY(float alpha);
	static matrix RotationZ(float alpha);
	static matrix Rotation(float alpha, const vector &v);
	static matrix Scale(float x, float y, float z);
	static matrix Translation(float x, float y, float z);

	// These friend functions can be faster in critical situations 
	// because they do not return a matrix or point by value, they
	// just fill memory provided by the caller!
	friend void Add(matrix &result, const matrix &m1, const matrix &m2);
	friend void Subtract(matrix &result, const matrix &m1, const matrix &m2);
	friend void Multiply(matrix &result, const matrix &m1, const matrix &m2);
	friend void Multiply(point &result, const matrix &m, const point &p);
	friend void Multiply(vector &result, const matrix &m, const vector &p);

	friend matrix operator*(const matrix &m1, const matrix &m2);
	friend point operator*(const matrix &m, const point &p);
	friend vector operator*(const matrix &m, const vector &p);
	friend matrix operator+(const matrix &m1, const matrix &m2);
	friend matrix operator-(const matrix &m1, const matrix &m2);
	friend matrix operator*(const matrix &m1, const matrix &m2);
		
	// Note, these are methods, not friends
	matrix &operator+=(const matrix &m2);
	matrix &operator-=(const matrix &m2);
	matrix &operator*=(const matrix m2);

private:

	unsigned int rows, cols;
	float *m;
	float *mFloat;
};

bool IsZero(const matrix &m);
bool IsIdentity(const matrix &m);
bool IsEqual(const matrix &m1, const matrix &m2);
bool IsNotEqual(const matrix &m1, const matrix &m2);

#endif