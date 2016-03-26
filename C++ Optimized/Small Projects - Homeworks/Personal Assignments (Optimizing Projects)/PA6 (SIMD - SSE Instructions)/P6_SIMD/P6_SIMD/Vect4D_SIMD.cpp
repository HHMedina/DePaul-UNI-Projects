#include "Vect4D_SIMD.h"
#include "Matrix_SIMD.h"

#include <xmmintrin.h>
#include <smmintrin.h> 



Vect4D_SIMD Vect4D_SIMD::operator * (const Matrix_SIMD &M)
{

	/*
	x Values =     x*m.m0       x * m.m1     x*m.m2       x*m.m3
			    +			  +				+			+
	y Values =     y*m.m4       y*m.m5       y*m.m6       y*m.m7
			    +			  +				+			+
	z Values =     z*m.m8      z*m.m9        z*m.m10      z*m.m11
			    +			  +				+			+
	W Values =     w * m.m12   w * m.m13    w * m.m14    w * m.m15

         =       C.x      ,   C.y,         C.z,         C.w         (added form top to bottom)
	*/
			
	__m128 xVals = _mm_mul_ps(M.v0.m, _mm_load_ps1(&x));
	__m128 yVals = _mm_mul_ps(M.v1.m, _mm_load_ps1(&y));
	__m128 zVals = _mm_mul_ps(M.v2.m, _mm_load_ps1(&z));
	__m128 wVals = _mm_mul_ps(M.v3.m, _mm_load_ps1(&w));

	return Vect4D_SIMD(_mm_add_ps(	_mm_add_ps(xVals, yVals),
									_mm_add_ps(zVals, wVals)));
};
