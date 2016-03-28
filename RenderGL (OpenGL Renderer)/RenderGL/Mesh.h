#ifndef __MESH_H__
#define __MESH_H__

/*Used for debug purposes*/
#define MESHDEBUG 0 //1 = Debug on, 0 = Debug off.
#include <iostream>
#include <fstream>
using namespace std;
#include "shape.h"
#include "OpenGLWin.h"
#include "LinkedList.h"
#include "Loader.h"

class face;

class vert 
{
public:

	struct node
	{
		int f;
		node *next;
	};

	vert() { head = tail = current = NULL; }
	vert(const vert &v);
	~vert();
	
	void SetPosition(float x, float y, float z);
	void SetPosition(float *x);

	float& operator[](int i) { return position[i]; }

	void AppendFace(int f);
	int GetCurrent(void) const { return current->f; }
	void MoveFirst(void) const { current = head; }
	void MoveNext(void) const { if (!AtEnd()) current = current->next; }
	bool AtEnd(void) const { return current == NULL; }
	void ClearFaceList(void);

	point position;
	vector normal;
	vector uv;
	vector dPdu;
	vector dPdv;
	node *head, *tail;
	mutable node *current;
	bool edge, processed;
};

class face
{
public:

	unsigned int vertices[3];
};

class mesh : public shape
{
public:

	mesh(const char *name = "mesh");
	mesh(const mesh& c);
	~mesh();

	// These methods have several ways to add a face to the mesh, either
	// by adding the vertices themselves at the same time or by referring
	// to existing vertices by index
	void AddFace(point *Vertices, int n);
	void AddFace(float *Vertices[], int n);
	void AddFace(int *VertexIndices, int n);

	// Several different methods for setting the position of a vertex in the 
	// list.  The RecalculateNormals parameter determines whether the normals
	// should be recalculated immediately or not.  Sometimes we want to change
	// a lot of vertices and only recalculate at the end.
	void SetVertex(int k, float Coord[3], bool AutoRecalculateNormals = true);
	void SetVertex(int k, point Coord, bool recalculateNormals = true)
		{ SetVertex(k, Coord.AsArray(), recalculateNormals); }
	point GetVertex(int k);
	
	int NVertices(void);
	int NFaces(void);
	int NInstances(void);

	void RecalculateNormals(void);
	void DrawShape(void);
	void Erase(int newVertexSize, int newFaceSize);

 	void Read(Loader &input);

	mesh &operator=(const mesh &c);

	virtual point Center(void);
	virtual point Size(void);
	virtual void MinMax(point &min, point &max);
	virtual bool Resized(void) { return resize; }

protected:

	void BuildVertexBufferObjects(void);
	void BuildVertexBufferObjects_PreComp(void);
	void BindBuffers(void);
	void BindBuffers_PreComp(void);

	void ResizeVertexList(void);
	int AddVertex(const point &Coord);
	void AddFace(face *f);
	void CopyFace(face *f);
	void ResizeFaceList(void);

	void ReadIndexed(Loader &input);
	void ReadFace(Loader &input);

	vert *vertices;
	face *faces;
	vector *faceNormals;
	vector *faceDpDu, *faceDpDv;

	int nVerts, vertSize;
	int nFaces, faceSize;
	point mMin, mMax;
	bool resize;

	bool normalsSpecifiedInFile;
	bool uvsSpecifiedInFile;
	bool derivativesSpecifiedInFile;

	bool separateVertices;
};

#endif