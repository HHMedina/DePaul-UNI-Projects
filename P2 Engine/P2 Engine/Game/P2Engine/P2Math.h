#ifndef P2Math_H
#define P2Math_H

class Vect;
class Matrix;
class AABBBoundingBox;
class OOBB;
class BoundingSphereVolume;

class P2Math{

public:
	static  float Min(float a,float b);

	static float Clamp(float value,float minVal, float maxVal);

	static float DegToRad(float);

	static  float Max(float a,float b);

	static Vect MaxVect(Vect&a , Vect&b);

	static Vect MinVect(Vect&a , Vect&b);

	static bool IsWithinInterval(float i1min, float i1max, float i2min , float i2max);

	static bool TestAABBAABBCollision(AABBBoundingBox * a, AABBBoundingBox* b);

	static bool TestAABBAABBCollision(Vect& minA,Vect& maxA,Vect& minB,Vect& maxB);

	static bool TestAABBOBBCollision(AABBBoundingBox * a ,OOBB* o);

	static bool TestOBBOBBCollision(OOBB* a, OOBB* b);

	static bool TestSphereOBBCollision(BoundingSphereVolume* s,OOBB* o );

	static bool TestSphereSphereCollision(Vect& centerA, float radiusA, Vect& centerB, float radiusB);

	static bool TestSphereSphereCollision(BoundingSphereVolume* a, BoundingSphereVolume* b);

	static int ArrayIndexFrom2DTo1D(int row, int column, int numberOfCellsInRow);
	
	static bool TestAABBSphereCollision(AABBBoundingBox* a, BoundingSphereVolume* s);

	static bool TestAABBSphereCollision(Vect& minAABB,Vect& maxAABB, Vect& sphereCenter,float sphereRadius);

	static bool IsWithinSeperatedAxisInterval(const Vect& axis,const Vect aPoints[],const int aPointsSize,const Vect bPoints[],const int bPointsSize);

	static Matrix LeftHandRotX(float amount);

	static Matrix LeftHandRotY(float amount);

	static Matrix LeftHandRotZ(float amount);
};
#endif