#ifndef __BASICSHAPES_H__
#define __BASICSHAPES_H__

#include <iostream>
using namespace std;

#include "shape.h"
#include "point.h"
#include "Loader.h"
#include "Mesh.h"

class sphere : public mesh
{
public:

	sphere(point c = point(0, 0, 0), float r = 1.0, int res = 10, char *name = "sphere") 
		{ SetObjectName(name); strcpy(shapeName, "sphere"); SetSphere(c, r, res); }
	~sphere();

	void SetSphere(point c, float r = 1.0, int res = 10);

	virtual point Center(void) { return center; }
	virtual point Size(void) { return point(2*radius, 2*radius, 2*radius); }
	virtual void MinMax(point &Min, point &Max) 
	{ vector v(radius, radius, radius); Min = center - v; Max = center + v; }
	virtual bool Resized(void) { return true; }  // We don't currently track resizing, so always recalculate MinMax.

	virtual void DrawShape(void);

protected:

	virtual void Read(Loader &input);

	void SetupShape(void);

	point center;
	float radius;
	int resolution;
};

class cylinder : public mesh
{
public:

	cylinder(point b = point(0, 0, 0), float r = .1, float h = 1.0, int res = 10, int a = 2, bool capped = true, char *name = "cylinder") 
		{ SetObjectName(name); strcpy(shapeName, "cylinder"); SetCylinder(b, r, h, res); }
	~cylinder();

	void SetCylinder(point b, float r = .1, float h = 1.0, int res = 10, int a = 2, bool capped = true);

	virtual point Center(void) { return base + vector(0, 0, .5) * height; }
	virtual point Size(void) { return point(2*radius, 2*radius, height); }
	virtual void MinMax(point &Min, point &Max) 
	{ vector v(radius, radius, 0.0); Min = base - v; Max = base + v + vector(0, 0, height); }
	virtual bool Resized(void) { return true; }  // We don't currently track resizing, so always recalculate MinMax.

	virtual void DrawShape(void);

protected:

	virtual void Read(Loader &input);

	void SetupShape(void);

	point base;
	float radius, height;
	bool capped;
	int resolution;
	int axis;
};


class cone : public mesh
{
public:

	cone(point b = point(0, 0, 0), float r = .1, float h = 1.0, int res = 10, int a = 2, bool capped = true, char *name = "cone") 
		{ SetObjectName(name); strcpy(shapeName, "cone"); SetCone(b, r, h, a, res); }
	~cone();

	void SetCone(point b, float r = .1, float h = 1.0, int res = 10, int a = 2, bool capped = true);

	virtual point Center(void) { return base + vector(0, 0, .5) * height; }
	virtual point Size(void) { return point(2*radius, 2*radius, height); }
	virtual void MinMax(point &Min, point &Max) 
	{ vector v(radius, radius, 0.0); Min = base - v; Max = base + v + vector(0, 0, height); }
	virtual bool Resized(void) { return true; }  // We don't currently track resizing, so always recalculate MinMax.

	virtual void DrawShape(void);

protected:

	virtual void Read(Loader &input);

	void SetupShape(void);

	point base;
	float radius, height;
	bool capped;
	int resolution;
	int axis;
};

#endif