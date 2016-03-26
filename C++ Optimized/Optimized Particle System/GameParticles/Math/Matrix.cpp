#include <Math.h>
#include <assert.h>
#include "Vect4d.h"
#include "Matrix.h"
#include <string>
#include <iostream>
#include "../DO_NOT_MODIFY/Timer/Timer.h"

Matrix::Matrix()
	:v0(0.0f, 0.0f, 0.0f, 0.0f), v1(0.0f, 0.0f, 0.0f, 0.0f), v2(0.0f, 0.0f, 0.0f, 0.0f), v3(0.0f, 0.0f, 0.0f, 0.0f)

/*
	:m0(0.0f), m1(0.0f), m2(0.0f), m3(0.0f),
	m4(0.0f), m5(0.0f), m6(0.0f), m7(0.0f),
	m8(0.0f), m9(0.0f), m10(0.0f), m11(0.0f),
	m12(0.0f), m13(0.0f), m14(0.0f), m15(0.0f)
*/
{	// constructor for the matrix
}


Matrix::Matrix(const Matrix& t)
	:v0(t.v0), v1(t.v1),v2(t.v2),v3(t.v3)

	/*:m0(t.m0), m1(t.m1), m2(t.m2), m3(t.m3),
	m4(t.m4), m5(t.m5), m6(t.m6), m7(t.m7),
	m8(t.m8), m9(t.m9), m10(t.m10), m11(t.m11),
	m12(t.m12), m13(t.m13), m14(t.m14), m15(t.m15)*/

{ // copy constructor
}

Matrix::Matrix(const __m128& newV0, const __m128& newV1, const __m128& newV2, const __m128& newV3)
	:mm0(newV0),mm1(newV1),mm2(newV2),mm3(newV3)//:v0(newV0), v1(newV1), v2(newV2), v3(newV3)
{

}

Matrix::Matrix(const float f0, const float f1, const float f2, const float f3,
	const float f4, const float f5, const float f6, const float f7,
	const float f8, const float f9, const float f10, const float f11,
	const float f12, const float f13, const float f14, const float f15)
	: m0(f0), m1(f1), m2(f2), m3(f3), m4(f4), m5(f5), m6(f6), m7(f7), m8(f8), m9(f9), m10(f10), m11(f11), m12(f12), m13(f13), m14(f14), m15(f15)
{
}

void Matrix::operator =(const Matrix& rhs)
{
	//memcpy_s(this,sizeof(Matrix),&rhs,1);
	mm0 = rhs.mm0;
	mm1 = rhs.mm1;
	mm2 = rhs.mm2;
	mm3 = rhs.mm3;
}

Matrix::~Matrix()
{
	// nothign to delete
}

void Matrix::setIdentMatrix()
{ // initialize to the identity matrix

	mm0 = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	mm1 = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	mm2 = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	mm3 = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

}

void Matrix::setTransMatrix(const Vect4D *t)
{ // set the translation matrix (note: we are row major)

	mm0 = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
	mm1 = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
	mm2 = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	mm3 = t->m;//_mm_set_ps(1.0f, t->z, t->y, t->x);

}

void Matrix::set(const MatrixRowEnum row,  const Vect4D *t )
{

	m128Arr[row] = t->m;
}

void Matrix::set(const MatrixRowEnum row, const float f0, const float f1, const float f2, const float f3)
{
	m128Arr[row] = _mm_set_ps(f3,f2,f1,f0);
	

}

float &Matrix::operator[](const INDEX_ENUM e)
{
	return mFloatArr[e];
		
}


void Matrix::get( const MatrixRowEnum row, Vect4D *t )
{ // get a row of the matrix
	
	t->m = m128Arr[row];
	
}

void Matrix::operator-=(const Matrix &t)
{
	this->mm0 = _mm_sub_ps(mm0,t.mm0);
	this->mm1 = _mm_sub_ps(mm1, t.mm1);
	this->mm2 = _mm_sub_ps(mm2, t.mm2);
	this->mm3 = _mm_sub_ps(mm3, t.mm3);


}

Matrix Matrix::operator-(const Matrix &t)
{
	//this->mm0 = _mm_sub_ps(mm0, t.mm0);
	//this->mm1 = _mm_sub_ps(mm1, t.mm1);
	//this->mm2 = _mm_sub_ps(mm2, t.mm2);
	///this->mm3 = _mm_sub_ps(mm3, t.mm3);

	return Matrix(_mm_sub_ps(mm0, t.mm0), _mm_sub_ps(mm1, t.mm1), _mm_sub_ps(mm2, t.mm2), _mm_sub_ps(mm3, t.mm3));
}

void Matrix::operator *=(const Matrix& t)
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

	mm0 = newX;
	mm1 = newY;
	mm2 = newZ;
	mm3 = newW;
}


Matrix Matrix::operator*( const Matrix& t) 
{ // matrix multiplications
	

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

	return Matrix(newX, newY, newZ, newW);
	
}

void Matrix::operator/=(const float rhs)
{ 

	float inv_rhs = 1.0f / rhs;
	__m128 invReg = _mm_load1_ps(&inv_rhs);
	mm0 = _mm_mul_ps(mm0, invReg);
	mm1 = _mm_mul_ps(mm1, invReg);
	mm2 = _mm_mul_ps(mm2, invReg);
	mm3 = _mm_mul_ps(mm3, invReg);
	


}

float Matrix::Determinant() 
{
	
	//if (ta+tb+tc+td+te+tf > .000001)
	//{
	//	int k = 0;
	//}

	
	/*
		//my sse version
		__m128 QPON = _mm_set_ps(m12, m13, m14, m15);


		__m128 MMulQPON = _mm_mul_ps(_mm_load1_ps(&m11), QPON);
		__m128 LMulQPON = _mm_mul_ps(_mm_load1_ps(&m10), QPON);
		__m128 KMulQPON = _mm_mul_ps(_mm_load1_ps(&m9), QPON);
		__m128 JMulQPON = _mm_mul_ps(_mm_load1_ps(&m8), QPON);

		// 0 = Q
		// 1 = P
		// 2 = O
		// 3 = N

		// A = { a,b,c,d / e,f,g,h / j,k, l, m /  n,o, p, q }
		// A = { 0,1,2,3 / 4,5,6,7 / 8,9,10,11 / 12,13,14,15 }

		__m128 Lq_Kq_Kp_Jp = _mm_set_ps(JMulQPON.m128_f32[1], KMulQPON.m128_f32[1], KMulQPON.m128_f32[0], LMulQPON.m128_f32[0]);
		__m128 Jp_Jo_00_00 = _mm_set_ps(0.0f, 0.0f, JMulQPON.m128_f32[2], JMulQPON.m128_f32[1]);

		__m128 Mp_Mo_Lo_Mn = _mm_set_ps(MMulQPON.m128_f32[3], LMulQPON.m128_f32[2], MMulQPON.m128_f32[2], MMulQPON.m128_f32[1]);
		__m128 Ln_Kn_00_00 = _mm_set_ps(0.0f, 0.0f, KMulQPON.m128_f32[3], LMulQPON.m128_f32[3]);


		__m128 LqSubMp_KqSubMo_KpSubLo_JqSubMn = _mm_sub_ps(Lq_Kq_Kp_Jp, Mp_Mo_Lo_Mn);
		__m128 JpSubLn_JoSubKn_00_00 = _mm_sub_ps(Jp_Jo_00_00, Ln_Kn_00_00);

		// S0 = Lq - Mp
		// S1 = Kq - Mo
		// S2 = Kp - Lo
		// S3 = Jq - Mn
		// S4 = Jp - Ln
		// S5 = Jo - Kn


		__m128 ES0_ES1_ES2_00 = _mm_mul_ps(_mm_load1_ps(&m4), _mm_set_ps(0.0f, LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[2], LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[1], LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[0]));
		__m128 FS0_FS3_FS4_00 = _mm_mul_ps(_mm_load1_ps(&m5), _mm_set_ps(0.0f, JpSubLn_JoSubKn_00_00.m128_f32[0], LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[3], LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[0]));
		__m128 GS1_GS3_GS5_00 = _mm_mul_ps(_mm_load1_ps(&m6), _mm_set_ps(0.0f, JpSubLn_JoSubKn_00_00.m128_f32[1], LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[3], LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[1]));
		__m128 HS2_HS4_HS5_00 = _mm_mul_ps(_mm_load1_ps(&m7), _mm_set_ps(0.0f, JpSubLn_JoSubKn_00_00.m128_f32[1], JpSubLn_JoSubKn_00_00.m128_f32[0], LqSubMp_KqSubMo_KpSubLo_JqSubMn.m128_f32[2]));

		//	H(S2)	H(S4)	H(S5)	G(S5) //row1
		//	  +		  +		  +		  +
		//	G(S1)	G(S3)	F(S3)	F(S4) //row2
		//	  -		  -		  -		  -
		//	F(S0)	E(S0)	E(S1)	E(S2) //row3	
		__m128 semiSemiFinal = _mm_sub_ps(_mm_add_ps(_mm_set_ps(GS1_GS3_GS5_00.m128_f32[2], HS2_HS4_HS5_00.m128_f32[2], HS2_HS4_HS5_00.m128_f32[1], HS2_HS4_HS5_00.m128_f32[0]),
			_mm_set_ps(FS0_FS3_FS4_00.m128_f32[2], FS0_FS3_FS4_00.m128_f32[1], GS1_GS3_GS5_00.m128_f32[1], GS1_GS3_GS5_00.m128_f32[0])),
			_mm_set_ps(ES0_ES1_ES2_00.m128_f32[2], ES0_ES1_ES2_00.m128_f32[1], ES0_ES1_ES2_00.m128_f32[0], FS0_FS3_FS4_00.m128_f32[0]));


		__m128 semiFinal = _mm_mul_ps(_mm_set_ps(m3, m2, m1, m0), semiSemiFinal);

		float original = semiFinal.m128_f32[0] - semiFinal.m128_f32[1] + semiFinal.m128_f32[2] - semiFinal.m128_f32[3];
	*/

	// A = { a,b,c,d / e,f,g,h / j,k, l, m /  n,o, p, q }
	// A = { 0,1,2,3 / 4,5,6,7 / 8,9,10,11 / 12,13,14,15 }

	// det(A) = a*det( { f,g,h / k,l,m / o,p,q } )
	//			- b*det( { e,g,h / j,l,m / n,p,q } )
	//			+ c*det( { e,f,h / j,k,m / n,o,q } )
	//			- d*det( { e,f,g / j,k,l / n,o,p } )

	// det(A) = (a (f (lq - mp) - g (kq - mo) + h (kp - lo) ) )
	//			- (b (e (lq - mp) - g (jq - mn) + h (jp - ln) ) )
	//			+ (c (e (kq - mo) - f (jq - mn) + h (jo - kn) ) )
	//			- (d (e (kp - lo) - f (jp - ln) + g (jo - kn) ) )


	
	// ta = (lq - mp)
	float ta = (m10 * m15) - (m11 * m14);
	// tb = (kq - mo)
	float tb = (m9 * m15) - (m11 * m13);
	// tc = (kp - lo)
	float tc = (m9 * m14) - (m10 * m13);
	// td = (jq - mn)
	float td = (m8 * m15) - (m11 * m12);
	// te = (jo - kn)
	float te = (m8 * m13) - (m9 *  m12);
	// tf = (jp - ln)
	float tf = (m8 * m14) - (m10 * m12);
	
	// det(A) = (a (f*ta  - g*tb + h*tc) )
	//			- (b (e*ta - g*td + h*tf) )
	//			+ (c (e*tb - f*td + h*te) )
	//			- (d (e*tc - f*tf + g*te) )
  
		
	
	return  ((m0 * ((m5 * ta) - (m6 * tb) + (m7 * tc)))
	- (m1 * ((m4 * ta) - (m6 * td) + (m7 * tf)))
	+
	(m2 * ((m4 * tb) - (m5 * td) + (m7 * te)))
	- (m3 * ((m4 * tc) - (m5 * tf) + (m6 * te))));


}

void Matrix::MorphIntoAdjugate()
{


	Matrix tmp;

	// load		ABC		(3)		ABC--
	float t1 = (m10*m15) - (m11*m14);
	float t2 = (m9*m15) - (m11*m13);
	float t3 = (m9*m14) - (m10*m13);



	// a = f(ta) - g(tb) + h(tc)
	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	// b = -( b(ta) - c(tb) + d(tc))
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));

	// load		JK		(5)		ABCJK
	float t4 = (m8*m15) - (m11*m12);
	float t5 = (m8*m14) - (m10*m12);
	// e = - ( e(ta) - g(tj) + h(tk))
	tmp.m4 = -((m4*t1) - (m6*t4) + (m7*t5));
	// f = a(ta) - c(tj) + d(tk)
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	AJ		(3)		-BC-K
	// load		P		(4)		PBC-K
	t1 = (m8*m13) - (m9*m12);
	// n = -( e(tc) - f(tk) + g(tp))
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	// o = a(tc) - b(tk) + c(tp)
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);

	// unload	KC		(2)		PB---
	// load		J		(3)		PBJ--
	t3 = (m8*m15) - (m11*m12);

	// j = e(tb) - f(tj) + h(tp)
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	// k = - ( a(tb) - b(tj) + d(tp))
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));

	// unload	BPJ		(0)		-----
	// load		DLM		(3)		DLM--
	t1 = (m6*m15) - (m7*m14);
	t2 = (m4*m15) - (m7*m12);
	t3 = (m4*m14) - (m6*m12);

	// g = - ( a(td) - c(tl) + d(tm))
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));

	// load		FR		(5)		DLMFR
	t4 = (m5*m14) - (m6*m13);
	t5 = (m4*m13) - (m5*m12);

	// p = - ( a(tf) - b(tm) + c(tr))
	tmp.m14 = -((m0*t4) - (m1*t3) + (m3*t5));

	// unload	M		(4)		DL-FR
	// load		E		(5)		DLEFR
	t3 = (m5*m15) - (m7*m13);

	// l = a(te) - b(tl) + d(tr)
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);

	// unload	LR		(3)		D-EF-
	// c = b(td) - c(te) + d(tf)
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);

	// unload	DEF		(0)		-----
	// load		GHI		(3)		GHI--
	t1 = (m6*m11) - (m7*m10);
	t2 = (m5*m11) - (m7*m9);
	t3 = (m5*m10) - (m6*m9);

	// d = -( b(tg) - c(th) + d(ti) )
	tmp.m3 = -((m1*t1) - (m2*t2) + (m3*t3));

	// load		NO		(5)		GHINO
	t4 = (m4*m11) - (m7*m8);
	t5 = (m4*m10) - (m6*m8);

	// h = a(tg) - c(tn) + d(to)
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);

	// unload	G		(4)		-HINO
	// load		Q		(5)		QHINO
	t1 = (m4*m9) - (m5*m8);

	// m = -( a(th) - b(tn) + d(tq))
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));

	// unload	HN		(3)		Q-I-O
	// q = a(ti) - b(to) + c(tq)
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);
	*this = tmp;


}

Matrix Matrix::GetAdjugate() 
{
	// matrix = { a,b,c,d / e,f,g,h / j,k,l,m / n,o,p,q }
	
	// ta = lq - mp
	// tb = kq - mo
	// tc = kp - lo
	// td = gq - hp
	// te = fq - ho
	// tf = fp - go
	// tg = gm - hl
	// th = fm - hk
	// ti = fl - gk
	// tj = jq - mn
	// tk = jp - ln
	// tl = eq - hn
	// tm = ep - gn
	// tn = em - hj
	// to = el - gj
	// tp = jo - kn
	// tq = ek - fj
	// tr = eo - fn
	
	// a = det( { f,g,h / k,l,m / o,p,q } )
	// a = f(lq - mp) - g(kq - mo) + h(kp - lo)
	// a = f(ta) - g(tb) + h(tc)
	
	// b = -det( { b,c,d / k,l,m / o,p,q } )
	// b = -( b(lq - mp) - c(kq - mo) + d(kp - lo))
	// b = -( b(ta) - c(tb) + d(tc))
	
	// c = det( { b,c,d / f,g,h / o,p,q } )
	// c = b(gq - hp) - c(fq - ho) + d(fp - go)
	// c = b(td) - c(te) + d(tf)
	
	// d = -det( { b,c,d / f,g,h / k,l,m } )
	// d = -( b(gm - hl) - c(fm - hk) + d(fl - gk) )
	// d = -( b(tg) - c(th) + d(ti) )

	// e = -det( { e,g,h / j,l,m / n,p,q } )
	// e = - ( e(lq - mp) - g(jq - mn) + h(jp - ln))
	// e = - ( e(ta) - g(tj) + h(tk))
	
	// f = det( { a,c,d / j,l,m / n,p,q } )
	// f = a(lq - mp) - c(jq - mn) + d(jp - ln)
	// f = a(ta) - c(tj) + d(tk)
	
	// g = -det( { a,c,d / e,g,h / n,p,q } )
	// g = - ( a(gq - hp) - c(eq - hn) + d(ep - gn))
	// g = - ( a(td) - c(tl) + d(tm))
	
	// h = det( { a,c,d / e,g,h / j,l,m } )
	// h = a(gm - hl) - c(em - hj) + d(el - gj)
	// h = a(tg) - c(tn) + d(to)
	
	// j = det( { e,f,h / j,k,m / n,o,q } )
	// j = e(kq - mo) - f(jq - mn) + h(jo - kn)
	// j = e(tb) - f(tj) + h(tp)
	
	// k = -det( { a,b,d / j,k,m / n,o,q } )
	// k = - ( a(kq - mo) - b(jq - mn) + d(jo - kn))
	// k = - ( a(tb) - b(tj) + d(tp))
	
	// l = det( { a,b,d / e,f,h / n,o,q } )
	// l = a(fq - ho) - b(eq - hn) + d(eo - fn)
	// l = a(te) - b(tl) + d(tr)
	
	// m = -det( { a,b,d / e,f,h / j,k,m } )
	// m = -( a(fm - hk) - b(em - hj) + d(ek - fj))
	// m = -( a(th) - b(tn) + d(tq))
	
	// n = -det( { e,f,g / j,k,l / n,o,p } )
	// n = -( e(kp - lo) - f(jp - ln) + g(jo - kn))
	// n = -( e(tc) - f(tk) + g(tp))
	
	// o = det( { a,b,c / j,k,l / n,o,p } )
	// o = a(kp - lo) - b(jp - ln) + c(jo - kn)
	// o = a(tc) - b(tk) + c(tp)
	
	// p = -det( { a,b,c / e,f,g / n,o,p } )
	// p = - ( a(fp - go) - b(ep - gn) + c(eo - fn))
	// p = - ( a(tf) - b(tm) + c(tr))
	
	// q = det( { a,b,c / e,f,g / j,k,l } )
	// q = a(fl - gk) - b(el - gj) + c(ek - fj)
	// q = a(ti) - b(to) + c(tq)
	

	//my sse


	Matrix tmp;
	
	// load		ABC		(3)		ABC--
	float t1 = (m10*m15) - (m11*m14);
	float t2 = (m9*m15) - (m11*m13);
	float t3 = (m9*m14) - (m10*m13);
	
	

	// a = f(ta) - g(tb) + h(tc)
	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	// b = -( b(ta) - c(tb) + d(tc))
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));
	
	// load		JK		(5)		ABCJK
	float t4 = (m8*m15) - (m11*m12);
	float t5 = (m8*m14) - (m10*m12);
	// e = - ( e(ta) - g(tj) + h(tk))
	tmp.m4 = -( (m4*t1) - (m6*t4) + (m7*t5));
	// f = a(ta) - c(tj) + d(tk)
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);
	
	// unload	AJ		(3)		-BC-K
	// load		P		(4)		PBC-K
	t1 = (m8*m13) - (m9*m12);
	// n = -( e(tc) - f(tk) + g(tp))
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	// o = a(tc) - b(tk) + c(tp)
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);
	
	// unload	KC		(2)		PB---
	// load		J		(3)		PBJ--
	t3 = (m8*m15) - (m11*m12);
	
	// j = e(tb) - f(tj) + h(tp)
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	// k = - ( a(tb) - b(tj) + d(tp))
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));
	
	// unload	BPJ		(0)		-----
	// load		DLM		(3)		DLM--
	t1 = (m6*m15) - (m7*m14);
	t2 = (m4*m15) - (m7*m12);
	t3 = (m4*m14) - (m6*m12);
	
	// g = - ( a(td) - c(tl) + d(tm))
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));
	
	// load		FR		(5)		DLMFR
	t4 = (m5*m14) - (m6*m13);
	t5 = (m4*m13) - (m5*m12);
	
	// p = - ( a(tf) - b(tm) + c(tr))
	tmp.m14 = -( (m0*t4) - (m1*t3) + (m3*t5));
	
	// unload	M		(4)		DL-FR
	// load		E		(5)		DLEFR
	t3 = (m5*m15) - (m7*m13);
	
	// l = a(te) - b(tl) + d(tr)
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);
	
	// unload	LR		(3)		D-EF-
	// c = b(td) - c(te) + d(tf)
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);
	
	// unload	DEF		(0)		-----
	// load		GHI		(3)		GHI--
	t1 = (m6*m11) - (m7*m10);
	t2 = (m5*m11) - (m7*m9);
	t3 = (m5*m10) - (m6*m9);
	
	// d = -( b(tg) - c(th) + d(ti) )
	tmp.m3 = -( (m1*t1) - (m2*t2) + (m3*t3));
	
	// load		NO		(5)		GHINO
	t4 = (m4*m11) - (m7*m8);
	t5 = (m4*m10) - (m6*m8);
	
	// h = a(tg) - c(tn) + d(to)
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);
	
	// unload	G		(4)		-HINO
	// load		Q		(5)		QHINO
	t1 = (m4*m9) - (m5*m8);
	
	// m = -( a(th) - b(tn) + d(tq))
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));
	
	// unload	HN		(3)		Q-I-O
	// q = a(ti) - b(to) + c(tq)
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);
	
	return tmp;
	
	/*
	return Matrix(Vect4D((m5*t1) - (m6*t2) + (m7*t3),-((m1*t1) - (m2*t2) + (m3*t3)),(m1*t1) - (m2*t3) + (m3*t4),-((m1*t1) - (m2*t2) + (m3*t3))).m,
		Vect4D(-((m4*t1) - (m6*t4) + (m7*t5)),(m0*t1) - (m2*t4) + (m3*t5),-((m0*t1) - (m2*t2) + (m3*t3)), (m0*t1) - (m2*t4) + (m3*t5)).m,
		Vect4D(	(m4*t2) - (m5*t3) + (m7*t1) , -((m0*t2) - (m1*t3) + (m3*t1)) , (m0*t3) - (m1*t2) + (m3*t5) , -((m0*t2) - (m1*t4) + (m3*t1)) ).m ,
		Vect4D( -((m4*t3) - (m5*t5) + (m6*t1)) , (m0*t3) - (m1*t5) + (m2*t1), -((m0*t4) - (m1*t3) + (m3*t5)), (m0*t3) - (m1*t5) + (m2*t1)).m );
	*/

	/*

	tmp.m0 = (m5*t1) - (m6*t2) + (m7*t3);
	tmp.m1 = -((m1*t1) - (m2*t2) + (m3*t3));
	tmp.m2 = (m1*t1) - (m2*t3) + (m3*t4);
	tmp.m3 = -((m1*t1) - (m2*t2) + (m3*t3));
	tmp.m4 = -((m4*t1) - (m6*t4) + (m7*t5));
	tmp.m5 = (m0*t1) - (m2*t4) + (m3*t5);
	tmp.m6 = -((m0*t1) - (m2*t2) + (m3*t3));
	tmp.m7 = (m0*t1) - (m2*t4) + (m3*t5);
	tmp.m8 = (m4*t2) - (m5*t3) + (m7*t1);
	tmp.m9 = -((m0*t2) - (m1*t3) + (m3*t1));
	tmp.m10 = (m0*t3) - (m1*t2) + (m3*t5);
	tmp.m11 = -((m0*t2) - (m1*t4) + (m3*t1));
	tmp.m12 = -((m4*t3) - (m5*t5) + (m6*t1));
	tmp.m13 = (m0*t3) - (m1*t5) + (m2*t1);
	tmp.m14 = -((m0*t4) - (m1*t3) + (m3*t5));
	tmp.m15 = (m0*t3) - (m1*t5) + (m2*t1);

	*/
}

void Matrix::Inverse( Matrix &out ) 
{
	//Matrix tmp;
	
	float det = Determinant();
	if(fabsf(det) < 0.0001f)
	{
		// do nothing, Matrix is not invertable
	}
	else
	{
		out = GetAdjugate();
		//out.MorphIntoAdjugate();
		out /= det;
	}
	
	//out = tmp;
}

void Matrix::setScaleMatrix(const Vect4D *scale)
{
	//	{	sx		0		0		0	}
	//	{	0		sy		0		0	}
	//	{	0		0		sz		0	}
	//	{	0		0		0		1	}
	
	mm0 = _mm_set_ps(0.0f, 0.0f, 0.0f, scale->x);
	mm1 = _mm_set_ps(0.0f, 0.0f, scale->y, 0.0f);
	mm2 = _mm_set_ps(0.0f, scale->z, 0.0f, 0.0f);
	mm3 = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
		
}

void Matrix::setRotZMatrix(const float az)
{
	//	{	cos		-sin	0		0	}
	//	{	sin		cos		0		0	}
	//	{	0		0		1		0	}
	//	{	0		0		0		1	}
	

	float sinRes = sinf(az);
	float cosRes = cosf(az);

	mm0 = _mm_set_ps(0.0f, 0.0f, -sinRes, cosRes);
	mm1 = _mm_set_ps(0.0f, 0.0f, cosRes, sinRes);
	mm2 = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
	mm3 = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

}

// End of file