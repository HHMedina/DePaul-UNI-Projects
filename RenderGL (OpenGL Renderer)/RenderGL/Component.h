#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <iostream>
#include <fstream>
using namespace std;

#include "shape.h"
#include "OpenGLWin.h"
#include "Loader.h"

class instance : public shape
{
public:

    instance(const char *masterName = "");

	void SetMaster(const char *masterName);

	virtual point Center(void) { return master->Center(); }
	virtual void MinMax(point &Min, point &Max) { master->MinMax(Min, Max); }
	virtual point Size(void) { return master->Size(); }
	virtual bool Resized(void) { return master->Resized(); }

    virtual void DrawShape(void);
	virtual void ReadObject(Loader &input);

    shape *master;
};

class component : public shape
{
public:

	component(const char *name = "component");
	component(const component& c);
	~component();

	component &operator=(const component &c);

	void AddInstance(instance *c);
	int NInstances(void);

	void DrawShape(void);
	void Erase(void);
	void Read(Loader &input);

	bool FindShapeInHierarchy(shape *s, LinkedList<shape *> sList);

	virtual point Center(void);
	virtual point Size(void);
	virtual void MinMax(point &min, point &max);
	virtual bool Resized(void) { if (resized) return true; for (int i = 0; i < nInst; i++) if (instances[i]->Resized()) { return true; } return false; }

protected:

	point cMin, cMax, cSize, cCenter;
	bool resized;

	void component::RecomputeBounds(void);

	instance **instances;
	int nInst, instSize;
};

#endif