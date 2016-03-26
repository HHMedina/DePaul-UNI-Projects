#include "Matrix_SIMD.h"	


Matrix_SIMD Matrix_SIMD::operator * (const Matrix_SIMD &t)
{

	__m128 multCol0 = _mm_mul_ps(_mm_load1_ps(&m0), t.v0.m);
	__m128 multCol1 = _mm_mul_ps(_mm_load1_ps(&m1), t.v1.m);
	__m128 multCol2 = _mm_mul_ps(_mm_load1_ps(&m2), t.v2.m);
	__m128 multCol3 = _mm_mul_ps(_mm_load1_ps(&m3), t.v3.m);

	__m128 newX = _mm_add_ps(_mm_add_ps(multCol0, multCol1), _mm_add_ps(multCol2, multCol3));

	__m128 multCol4 = _mm_mul_ps(_mm_load1_ps(&m4), t.v0.m);
	__m128 multCol5 = _mm_mul_ps(_mm_load1_ps(&m5), t.v1.m);
	__m128 multCol6 = _mm_mul_ps(_mm_load1_ps(&m6), t.v2.m);
	__m128 multCol7 = _mm_mul_ps(_mm_load1_ps(&m7), t.v3.m);

	__m128 newY = _mm_add_ps(_mm_add_ps(multCol4, multCol5), _mm_add_ps(multCol6, multCol7));

	__m128 multCol8 = _mm_mul_ps(_mm_load1_ps(&m8), t.v0.m);
	__m128 multCol9 = _mm_mul_ps(_mm_load1_ps(&m9), t.v1.m);
	__m128 multCol10 = _mm_mul_ps(_mm_load1_ps(&m10), t.v2.m);
	__m128 multCol11 = _mm_mul_ps(_mm_load1_ps(&m11), t.v3.m);

	__m128 newZ = _mm_add_ps(_mm_add_ps(multCol8, multCol9), _mm_add_ps(multCol10, multCol11));

	__m128 multCol12 = _mm_mul_ps(_mm_load1_ps(&m12), t.v0.m);
	__m128 multCol13 = _mm_mul_ps(_mm_load1_ps(&m13), t.v1.m);
	__m128 multCol14 = _mm_mul_ps(_mm_load1_ps(&m14), t.v2.m);
	__m128 multCol15 = _mm_mul_ps(_mm_load1_ps(&m15), t.v3.m);

	__m128 newW = _mm_add_ps(_mm_add_ps(multCol12, multCol13), _mm_add_ps(multCol14, multCol15));

	return Matrix_SIMD(newX, newY, newZ, newW);
}