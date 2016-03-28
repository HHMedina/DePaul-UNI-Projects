#ifndef __ANIMATEDSHAPE_H__
#define __ANIMATEDSHAPE_H__

#include "shape.h"
#include "quat.h"

// We could make this a lot more flexible by deriving new 
// key types from a basic InterpKey, but the structure of the
// base class interpolator would become a lot more complex
// We would certainly do this, though, in production code.
//
// For now, we just include the data for all the types of 
// interpolators that we will be exploring, linear, hermite,
// catmull-rom and tcb.
struct InterpKey
{
	InterpKey() { constant = false; vIn = vOut = vector(0, 0, 0); dist = 0; tcb = vector(0, 0, 0); p = point(0, 0, 0); speedIn = speedOut = 0; }

	// Curve Data
	point p;
	vector vIn, vOut;
	vector tcb;
	float speedIn, speedOut;

	// Timing Data
	float t;
	float dist;
	bool constant; // Should the object move with a constant speed along the segment
};

struct ParamDistEntry
{
	float s;
	float d;
};

class Interpolator
{
public:

	Interpolator() { nKeys = 0; distTable = NULL; }

	virtual point Evaluate(float t);

	virtual void SetValue(int index, const point &p);
	virtual void SetTime(int index, float t);
	virtual void SetConstant(int index, bool value) { keys[index].constant = value; RebuildParamDistanceTable(); }
	virtual void SetKey(int index, float t, const point &p);
	virtual int  InsertKey(float t, const point &p);
	virtual void DeleteKey(float t, float tolerance, bool all);
	virtual void DeleteKey(int index);
	virtual void SetEaseIn(int index, float in) { keys[index].speedIn = in; }
	virtual void setEaseOut(int index, float out) { keys[index].speedOut = out; }

	virtual float GetTime(int index) { return keys[index].t; }
	virtual point GetValue(int index) { return keys[index].p; }
	virtual bool GetConstant(int index) { return keys[index].constant; }
	int FindInterval(float T);
	int FindKey(float T, float tolerance = .01);		
	int NKeys(void) { return nKeys; }

	virtual void Load(Loader &input);
	virtual bool ReadKeyAttribute(Loader &input, int nKey, char *attribute) { return false; }
	virtual bool ReadAttribute(Loader &input, char *attribute) { return false; }

	static Interpolator *LoadInterpolator(Loader &input);

protected:

	enum { MAX_KEYS = 1000 };

	void SortKeys(void);

	virtual void RebuildParamDistanceTable(void);

	virtual point pointOnCurve(float s) = 0;

	InterpKey keys[MAX_KEYS];	// Could use a dynamic array here, 
	int nKeys;					// but 1000 should be enough for now!

	// DistTable will be 100 * nKeys long.  
	ParamDistEntry *distTable;
	float averageDT;
};

class LinearInterp : public Interpolator
{
public:

	virtual point pointOnCurve(float s);
};

class animatedShape : public shape
{
public:
	
	animatedShape();

	void SetShape(shape *S) { s = S; }
	void SetPositionInterp(Interpolator *interp) { iPosition = interp; }
	void SetScaleInterp(Interpolator *interp) { iScale = interp; }
	void SetRotationInterp(Interpolator *interp) { iRotation = interp; }

	Interpolator *GetPositionInterp(void) { return iPosition; }
	Interpolator *GetScaleInterp(void) { return iScale; }
	Interpolator *GetRotationInterp(void) { return iRotation; }

	virtual void Read(Loader &input);
    virtual void ApplyTransforms(void);
	virtual void DrawShape(void);

	virtual point Center(void) { return s->Center(); }
	virtual point Size(void) { return s->Size(); }
	virtual void MinMax(point &Min, point &Max) { s->MinMax(Min, Max); }
	virtual bool Resized(void) { return s->Resized(); }

private:

	shape *s;

	Interpolator *iPosition;
	Interpolator *iScale;
	Interpolator *iRotation;  // Interpolator for Euler Angle Rotation
};

#endif