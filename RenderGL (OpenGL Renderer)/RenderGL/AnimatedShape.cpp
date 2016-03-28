
#include "AnimatedShape.h"

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


int Interpolator::FindInterval(float t)
{
	if (nKeys == 0 || nKeys == 1)
		return 0;
	else if (t < keys[0].t)
		return 0;
	else if (t > keys[nKeys - 1].t)
		return nKeys - 2;

//  Binary Serach
	int a = 0, b = nKeys - 1;
	while (b - a > 1)
	{
		int tmp = (b + a) / 2;
		if (keys[tmp].t > t)
			b = tmp;
		else a = tmp;
	}
	if (a > nKeys - 2)
		a = nKeys - 2;
	if (a < 0)
		a = 0;

	return a;
}

int Interpolator::FindKey(float T, float tolerance)
{
	int i = FindInterval(T);

	// If there is a key then it is either i or i+1 because T is on the interval between keys i and i+1
	if (fabs(keys[i].t - T) < tolerance)
		return i;
	else if (fabs(keys[i+1].t - T) < tolerance)
		return i + 1;
	else
		return -1;
}

// Simple reverse bubble sort for sorting keys
// Reverse it to make it more efficient for inserting
// keys on the end.
void Interpolator::SortKeys(void)
{
	bool found;

	do 
	{
		found = false;
		for (int i = nKeys - 1; i > 0; i--)
		{
			if (keys[i-1].t > keys[i].t)
			{
				InterpKey tmp = keys[i];
				keys[i] = keys[i-1];
				keys[i-1] = tmp;
				found = true;
			}
		}
	} while (found == true);
}

void Interpolator::SetValue(int index, const point &p)
{
	if (index >= nKeys)
		cout << "Tried to set key position above nKeys = " << nKeys << "; Command ignored.\n";

	keys[index].p = p;

	RebuildParamDistanceTable();
}

void Interpolator::SetTime(int index, float t)
{
	if (index >= nKeys)
		cout << "Tried to set key position above nKeys = " << nKeys << "; Command ignored.\n";

	keys[index].t = t;

	SortKeys();
	RebuildParamDistanceTable();
}

void Interpolator::SetKey(int index, float t, const point &p)
{
	if (index >= nKeys)
		cout << "Tried to set key position above nKeys = " << nKeys << "; Command ignored.\n";

	keys[index].t = t;
	keys[index].p = p;

	SortKeys();
	RebuildParamDistanceTable();
}

int Interpolator::InsertKey(float t, const point &p)
{
	if (nKeys == MAX_KEYS)
		cout << "Cannot add more than " << MAX_KEYS << " to interpolator; Command ignored.\n";

	keys[nKeys].t = t;
	keys[nKeys].p = p;
	nKeys++;

	SortKeys();
	RebuildParamDistanceTable();

	return FindKey(t);
}

// Deletes the keys it encounters whose time is within
// "tolerance" of t.  If "all" = false then it will only
// delete the first it finds.
void Interpolator::DeleteKey(float t, float tolerance, bool all)
{
	for (int i = 0; i < nKeys; i++)
	{
		if (fabs(keys[i].t - t) < tolerance)
		{
			for (int j = i + 1; j < nKeys; j++)
				keys[j - 1] = keys[j];
			nKeys--;
			if (all == false)
				break;
		}
	}
	RebuildParamDistanceTable();
}

void Interpolator::DeleteKey(int index)
{
	if (index < nKeys)
	{
		for (int i = index + 1; i < nKeys; i++)
			keys[i - 1] = keys[i];
		nKeys--;
	}
	RebuildParamDistanceTable();
}

void Interpolator::RebuildParamDistanceTable(void)
{
	// Not implemented
}

#include "stdio.h"

bool record = false;

point Interpolator::Evaluate(float t)   // This evaluation method lets us step in and do constant velocity movement if we want to
{
	int i = FindInterval(t);

	float s = (t - keys[i].t) / (keys[i+1].t - keys[i].t);
	s = min(max(s, 0.0f), 1.0f);
	return pointOnCurve(s);
}

point LinearInterp::pointOnCurve(float s)
{
	if (nKeys == 0)
		return point(0, 0, 0);
	else if (nKeys == 1)
		return keys[0].p;

	int i = int(s);
	i = min(max(i, 0), nKeys - 2);
	s -= i;
	s = min(max(s, 0.0f), 1.0f);

	return Average(keys[i].p, keys[i+1].p, s);
}

animatedShape::animatedShape() : shape("animatedshape")
{
	iPosition = NULL;
	iScale = NULL;
	iRotation = NULL;
}

#include "Scene.h"

void animatedShape::ApplyTransforms(void)
{
	point pos, sc, rot;
	float time = curScene->GetTime();

	matrix curTransform = curScene->GetCurrentModelTransform();

	if (iPosition)
	{
		pos = iPosition->Evaluate(time);
		curTransform *= matrix::Translation(pos[0], pos[1], pos[2]);
	}

	if (iRotation)
	{
		rot = iRotation->Evaluate(time);
		curTransform *= matrix::Rotation(rot[2], vector(0, 0, 1));
		curTransform *= matrix::Rotation(rot[1], vector(0, 1, 0));
		curTransform *= matrix::Rotation(rot[0], vector(1, 0, 0));
	}

	if (iScale)
	{
		sc = iScale->Evaluate(time);
		curTransform *= matrix::Scale(sc[0], sc[1], sc[2]);
	}

	curScene->SetModelTransform(curTransform);

	// Apply the base constant transform for the shape
	shape::ApplyTransforms();
}

void animatedShape::DrawShape(void)
{
	s->DrawShape();
}

