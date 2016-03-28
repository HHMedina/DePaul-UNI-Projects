	
#include <math.h>
#include <iostream>
using namespace std;

#include "shape.h"
#include "component.h"
#include "Scene.h"
#include <ctype.h>

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


instance::instance(const char *masterName) : shape("instance")
{
    master = NULL;
    SetMaster(masterName);
}

void instance::DrawShape(void)
{
	if (master)
		master->Draw();
}
 
void instance::SetMaster(const char *masterName)
{
	char tmpName[MAX_NAME_LENGTH];

	if ((masterName != NULL) && (masterName[0] != '\0'))
	{
		int i = -1; // So that the list will start at -1 + 1 = 0
		do 
		{
			i++;
			sprintf(tmpName, "%s_%03d", masterName, i);
		} while (FindShape(tmpName) != NULL);
	    master = shape::FindShape(masterName);
		if (master == NULL)
			complain("Cannot find master object %s", masterName);
	}
	else
		sprintf(tmpName, "instance");
	SetObjectName(tmpName);
}

component::component(const char *name) : shape("component")
{
	SetObjectName(name);
	instances = new instance*[10];
	nInst = 0;
	instSize = 10;
	resized = true;
}

component::component(const component& c) : shape("component")
{
	SetObjectName(c.objectName);

	instances = new instance*[10];
	nInst = 0;
	instSize = 10;
	resized = true;

	for (int i = 0; i < c.nInst; i++)
		AddInstance(c.instances[i]);
}

component &component::operator=(const component &c)
{
	Erase();

	SetObjectName(c.objectName);

	instances = new instance*[10];
	nInst = 0;
	instSize = 10;
	resized = true;

	for (int i = 0; i < c.nInst; i++)
		AddInstance(c.instances[i]);
	return *this;
}

component::~component()
{
	Erase();
	delete [] instances;
}

void component::Erase(void)
{
	for (int i = 0; i < nInst; i++)
	{
		delete instances[i];
		instances[i] = NULL;
	}
	delete [] instances;
	nInst = 0;
	instances = new instance*[10];
	instSize = 10;
	resized = true;
}

void component::AddInstance(instance *c)
{
	if (nInst == instSize)
	{
		instance **tmp = new instance*[2 * instSize];
		instSize *= 2;
		for (int i = 0; i < nInst; i++)
			tmp[i] = instances[i];
		delete instances;
		instances = tmp;
	}
	instances[nInst] = c;
	nInst++;
	resized = true;
}

void component::DrawShape(void)
{
	for (int i = 0; i < nInst; i++)
		instances[i]->Draw();
}

bool component::FindShapeInHierarchy(shape *s, LinkedList<shape *> sList)
{
	sList.Append(this);
	if (s == this)
		return true;

	for (int i = 0; i < nInst; i++)
		if (instances[i]->FindShapeInHierarchy(s, sList))
			return true;
	
	sList.RemoveLast();
	return false;
}

void component::RecomputeBounds(void)
{
	if (Resized())
	{
		instance *inst;
		point iMin, iMax;

		if (nInst < 1)
			cMin = cMax = point(0, 0, 0);

		instances[0]->master->MinMax(iMin, iMax);
		cMin[0] = iMin[0];
		cMin[1] = iMin[1];
		cMin[2] = iMin[2];

		cMax[0] = iMax[0];
		cMax[1] = iMax[1];
		cMax[2] = iMax[2];
		for (int i = 1; i < nInst; i++)
		{
			inst = instances[i];
			instances[i]->master->MinMax(iMin, iMax);

			cMin[0] = min(cMin[0], iMin[0]);
			cMin[1] = min(cMin[1], iMin[1]);
			cMin[2] = min(cMin[2], iMin[2]);

			cMax[0] = max(cMax[0], iMax[0]);
			cMax[1] = max(cMax[1], iMax[1]);
			cMax[2] = max(cMax[2], iMax[2]);
		}
		cCenter = Average(cMin, cMax);
		cSize = point(0, 0, 0) + (cMax - cMin);
		resized = false;
	}
}

point component::Center(void) 
{
	RecomputeBounds();
	return cCenter;
}

point component::Size(void) 
{ 
	RecomputeBounds();
	return cSize;
}

void component::MinMax(point &Min, point &Max) 
{ 
	RecomputeBounds();
	Min = cMin;
	Max = cMax;
}

int component::NInstances(void)
{
	return nInst;
}

