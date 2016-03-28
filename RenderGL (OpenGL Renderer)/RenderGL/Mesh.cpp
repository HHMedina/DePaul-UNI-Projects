
#include <time.h>	
#include <math.h>
#include <iostream>
using namespace std;

#include "shape.h"
#include "scene.h"
#include "mesh.h"
#include "point.h"
#include "vector.h"

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


vert::vert(const vert &v)
{
	position = v.position;
	normal = v.normal;
	uv = v.uv;

	ClearFaceList();
	for (v.MoveFirst(); !v.AtEnd(); v.MoveNext())
		AppendFace(v.GetCurrent());
}

vert::~vert()
{
	ClearFaceList();
}

void vert::AppendFace(int f)
{
	node *n = new node;
	n->f = f;
	n->next = NULL;
	if (head == NULL)
		{ head = tail = n; }
	else
		{ tail->next = n; tail = n; }
	current = head;
}

void vert::ClearFaceList(void)
{
	node *prev;
	node *tmp = head;

	while (tmp != NULL)
	{
		prev = tmp;
		tmp = tmp->next;
		delete prev;
	}
	head = tail = NULL;
}

void vert::SetPosition(float x, float y, float z)
{
	position[0] = x; position[1] = y; position[2] = z;
}

void vert::SetPosition(float *x)
{
	position[0] = x[0]; position[1] = x[1]; position[2] = x[2];
}

mesh::mesh(const char *name) : shape("mesh")
{
	SetObjectName(name);

	vertexBuffer = faceIndexBuffer = 0;

	vertices = NULL;
	faces = NULL;
	Erase(10, 10);

	separateVertices = false;

	resize = true;
	normalsSpecifiedInFile = false;
	uvsSpecifiedInFile = false;
	derivativesSpecifiedInFile = false;
}

mesh::mesh(const mesh& m) : shape("mesh")
{
	SetObjectName(m.objectName);

	vertexBuffer = faceIndexBuffer = 0;

	vertices = NULL;
	faces = NULL;
	Erase(m.vertSize, m.faceSize);

	separateVertices = m.separateVertices;

	for (int i = 0; i < nVerts; i++)
		vertices[i] = m.vertices[i];

	for (int i = 0; i < m.nFaces; i++)
	{
		faces[i] = m.faces[i];
		faceNormals[i] = m.faceNormals[i];
		faceDpDu[i] = m.faceDpDu[i];
		faceDpDv[i] = m.faceDpDv[i];
	}

	normalsSpecifiedInFile = m.normalsSpecifiedInFile;
	uvsSpecifiedInFile = m.uvsSpecifiedInFile;
	derivativesSpecifiedInFile = m.derivativesSpecifiedInFile;

	resize = true;
	RecalculateNormals();
}

mesh &mesh::operator=(const mesh &m)
{
	SetObjectName(m.objectName);

	Erase(m.nVerts, m.nFaces);

	separateVertices = m.separateVertices;

	for (int i = 0; i < nVerts; i++)
		vertices[i] = m.vertices[i];

	for (int i = 0; i < m.nFaces; i++)
	{
		faces[i] = m.faces[i];
		faceNormals[i] = m.faceNormals[i];
		faceDpDu[i] = m.faceDpDu[i];
		faceDpDv[i] = m.faceDpDv[i];
	}

	normalsSpecifiedInFile = m.normalsSpecifiedInFile;
	uvsSpecifiedInFile = m.uvsSpecifiedInFile;
	derivativesSpecifiedInFile = m.derivativesSpecifiedInFile;

	resize = true;
	RecalculateNormals();

	return *this;
}

mesh::~mesh()
{
	// The shape class destructor releases the buffers!

	delete [] vertices;
	delete [] faces;
	delete [] faceNormals;
	delete [] faceDpDu;
	delete [] faceDpDv;
}

void mesh::Erase(int newVertexSize, int newFaceSize)
{
	ReleaseOpenGLObjects();

	if (vertices != NULL)
		delete [] vertices;
	nVerts = 0;	vertSize = newVertexSize;
	vertices = new vert[vertSize];
	
	if (faces != NULL)
		delete [] faces;
	nFaces = 0;	faceSize = newFaceSize;
	faces = new face[faceSize];
	faceNormals = new vector[faceSize];
	faceDpDu = new vector[faceSize];
	faceDpDv = new vector[faceSize];

	resize = true;
}

void mesh::AddFace(float *Vertices[], int n)
{
	int v0, v1, v2;

	v0 = AddVertex(point(Vertices[0][0], Vertices[0][1], Vertices[0][2]));
	v1 = AddVertex(point(Vertices[1][0], Vertices[1][1], Vertices[1][2]));
	for (int i = 2; i < n; i++)
	{
		v2 = AddVertex(point(Vertices[i][0], Vertices[i][1], Vertices[i][2]));

		ResizeFaceList();
		faces[nFaces].vertices[0] = v0;
		faces[nFaces].vertices[1] = v1;
		faces[nFaces].vertices[2] = v2;
		vertices[v0].AppendFace(nFaces);
		vertices[v1].AppendFace(nFaces);
		vertices[v2].AppendFace(nFaces);
		nFaces++;

		v1 = v2;
	}
	resize = true;
}

void mesh::AddFace(point *Vertices, int n)
{
	int v0, v1, v2;

	v0 = AddVertex(Vertices[0]);
	v1 = AddVertex(Vertices[1]);
	for (int i = 2; i < n; i++)
	{
		v2 = AddVertex(Vertices[i]);

		ResizeFaceList();
		faces[nFaces].vertices[0] = v0;
		faces[nFaces].vertices[1] = v1;
		faces[nFaces].vertices[2] = v2;
		vertices[v0].AppendFace(nFaces);
		vertices[v1].AppendFace(nFaces);
		vertices[v2].AppendFace(nFaces);
		nFaces++;

		v1 = v2;
	}
	resize = true;
}

void mesh::AddFace(int *VertexIndices, int n)
{
	int v0, v1, v2;

	for (int i = 2; i < n; i++)
	{
		ResizeFaceList();  // Since we are converting from triangles the 
						   // final face count may not match the face count in the file
		v0 = VertexIndices[0];
		v1 = VertexIndices[i - 1];
		v2 = VertexIndices[i];

		faces[nFaces].vertices[0] = v0;
		faces[nFaces].vertices[1] = v1;
		faces[nFaces].vertices[2] = v2;

		vertices[v0].AppendFace(nFaces);
		vertices[v1].AppendFace(nFaces);
		vertices[v2].AppendFace(nFaces);
		nFaces++;
	}
	resize = true;
}

void mesh::AddFace(face *f)
{
	ResizeFaceList();
	faces[nFaces] = *f;
	nFaces++;
	resize = true;
}

void mesh::ResizeFaceList(void)
{
	if (nFaces == faceSize)
	{
		ReleaseOpenGLObjects();

		faceSize *= 2;

		face *tmp = new face[faceSize];
		vector *tmpNorm = new vector[faceSize];
		vector *tmpDpDu = new vector[faceSize];
		vector *tmpDpDv = new vector[faceSize];

		for (int i = 0; i < nFaces; i++)
		{
			tmp[i] = faces[i];
			tmpNorm[i] = faceNormals[i];
			tmpDpDu[i] = faceDpDu[i];
			tmpDpDv[i] = faceDpDv[i];
		}

		delete [] faces;
		delete [] faceNormals;
		delete [] faceDpDu;
		delete [] faceDpDv;

		faces = tmp;
		faceNormals = tmpNorm;
		faceDpDu = tmpDpDu;
		faceDpDv = tmpDpDv;
	}
}

void mesh::ResizeVertexList(void)
{
	if (nVerts == vertSize)
	{
		ReleaseOpenGLObjects();

		vert *tmpArray = new vert[2 * vertSize];
		vertSize *= 2;
		for (int i = 0; i < nVerts; i++)
			tmpArray[i] = vertices[i];
		delete [] vertices;
		vertices = tmpArray;
	}
}

int mesh::AddVertex(const point &Coord)
{
	if (!separateVertices)
	{
		for (int i = 0; i < nVerts; i++)
		{
			point tmp = vertices[i].position;
			if (IsEqual(tmp, Coord))
				return i;
		} 
	}
	ResizeVertexList();

	vertices[nVerts].ClearFaceList();
	vertices[nVerts].position = Coord;
	nVerts++;

	resize = true;
	return nVerts - 1;
}

void mesh::SetVertex(int k, float Coord[3], bool recalculateNormals)
{
	if (k < nVerts)
	{
		vertices[k].position = point(Coord[0], Coord[1], Coord[2]);
		if (recalculateNormals)
			RecalculateNormals();
		resize = true;
	}
}

void mesh::BuildVertexBufferObjects(void)
{

	// To Do
	//
	// Check if they already exist, and if they do, destroy them
	//
	// Then allocate the buffers for both the vertex data and the vertex indices for the faces
	// The ints that will hold the buffer ID's have already been created for you as member data
	// for the mesh.
	// 
	// Then, set up the vertex buffer, bind it and send the vertex data from the "vertices" array.
	// You can just send all the data from the vertex list to the graphics card (for the moment, 
	// we will not worry about the pointers that are at the end of each vertex.  Yes, they are 
	// taking up space on the graphics card, but the graphics card doesn't care as long as we don't
	// try to tell the card that these pointers are valid vertex data :)
	//
	// Then do the same for the faceIndexBuffer.

	/* Vert info to fill
	point position;
	vector normal;
	vector uv;
	vector dPdu;
	vector dPdv;
	node *head, *tail;
	mutable node *current;
	bool edge, processed;
	*/

	/*
		Deletes already allocated Buffers
	*/
	
	if (vertexBuffer != 0) // vBuffer Already Allocated
	{
		glDeleteBuffers(1,&vertexBuffer);

		vertexBuffer = 0;
	}

	if (faceIndexBuffer != 0) // faceIndexBuffer Already Allocated
	{
		glDeleteBuffers(1, &faceIndexBuffer);
		faceIndexBuffer = 0;

	}


	/*
		Allocate buffers on GPU
	*/
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, nVerts * sizeof(vert), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &faceIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nFaces * 3 * sizeof(int), faces, GL_STATIC_DRAW);


}

// This #define is boiler-plate from OpenGL for getting the "offset" into a 
// struct for one of the struct elements.  It is used when setting up the 
// vertex array structure for the VertexArrayObject
#define BUFFER_OFFSET(obj) ((GLchar *)((int)(&(obj)) - (int)(vertices)))

void mesh::BindBuffers(void)
{
	// To Do
	//
	// Bind BOTH the buffers so that we know what the offsets in the AttribPointer calls refer to

	// These are provided as convenient aliases here for you to use. 
	shader *s = curScene->GetCurrentShader();
	GLuint glProgramObj = s->GLSLProgramObj();

	// Then we need to get the attribute indices for each of the attributes.  You can start with 
	// just surfacePosition, and surfaceNormal, but you will also eventually need to do surfaceUV
	// surfaceDpDu and surfaceDpDv

	// For each of the attributes, check to see if the position index is >= 0 (if not then the 
	// attribute is not used in the shader so we needn't bother to set the pointer).  Remember though,
	// OpenGL will not mind if you do set it even if the attribute is not used.  The one thing that this test
	// makes a little easier is error trapping when you've misspelled an attribute in a shader
	// 
	// You will need to set the pointers for each of the attributes above eventually, but again, you can
	// start with surfacePosition and surfaceNormal.  I've provided you with a #define above that can help 
	// with setting that offset in general, but to start with, JUST USE A CONSTANT offset for each since we 
	// know exacly how many bytes down the object position, normal, etc. start.

	/* 
	int vertPosLoc = glGetAttribLocation(shaderProgram, "vertexPos");
	int vertColorLoc = glGetAttribLocation(shaderProgram, "vertexColor");
	int vertNormalLoc = glGetAttribLocation(shaderProgram, "vertexNormal");

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glVertexAttribPointer(vertPosLoc, 4, GL_FLOAT, false, sizeof(vert), (void *)0);
	glEnableVertexAttribArray(vertPosLoc);

	glVertexAttribPointer(vertColorLoc, 4, GL_FLOAT, false, sizeof(vert), (void *)(sizeof(point)));
	glEnableVertexAttribArray(vertColorLoc);

	glVertexAttribPointer(vertNormalLoc, 4, GL_FLOAT, false, sizeof(vert), (void *)(2 * sizeof(point)));
	glEnableVertexAttribArray(vertNormalLoc);
	*/
	/*
	point position;
	vector normal;
	vector uv;
	vector dPdu;
	vector dPdv;
	node *head, *tail;
	mutable node *current;
	bool edge, processed;
	*/

	glGenVertexArrays(1, &vertexArrayObj);
	glBindVertexArray(vertexArrayObj);

	glUseProgram(glProgramObj);
	int vertSurfacePosLoc = glGetAttribLocation(glProgramObj, "surfacePosition");
	int vertSurfaceNormLoc = glGetAttribLocation(glProgramObj, "surfaceNormal");
	int vertSurfaceUVLoc = glGetAttribLocation(glProgramObj, "surfaceUV");

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);


	if (vertSurfacePosLoc >= 0){
 
		glVertexAttribPointer(vertSurfacePosLoc, 4, GL_FLOAT, false, sizeof(vert), (void *)0);
		glEnableVertexAttribArray(vertSurfacePosLoc);

	}
#if MESHDEBUG
	else
	{
		cout << "\nERROR: MESH.cpp: surfacePosition was not found in specified shader!";
	}
#endif 


	if (vertSurfaceNormLoc >= 0){
 
		glVertexAttribPointer(vertSurfaceNormLoc, 3, GL_FLOAT, false, sizeof(vert), (void *)(sizeof(point)));
		glEnableVertexAttribArray(vertSurfaceNormLoc);

	}
#if MESHDEBUG
	else
	{
		cout << "\nERROR: MESH.cpp: surfaceNormal was not found in specified shader!";
	}
#endif 


	if (vertSurfaceUVLoc >= 0){
		glVertexAttribPointer(vertSurfaceUVLoc, 2, GL_FLOAT, false, sizeof(vert), (void *)(sizeof(point)*2)); // point and vect have same size
		glEnableVertexAttribArray(vertSurfaceUVLoc);
	}
#if MESHDEBUG
	else
	{
		cout << "\nERROR: MESH.cpp: surfaceNormal was not found in specified shader!";
	}
#endif 

}

void mesh::DrawShape(void)
{
	// Make sure that our OpenGL buffers and array objects are set up. 
	//
	// To Do Important:  Once you have implemented BuildVertexBufferObjects, replace the call to the pre-compiled 
	// version below to call yours!
	if (vertexBuffer == 0)
		BuildVertexBufferObjects();
	
	// This method binds the buffers and makes sure that the attribute pointers are set.
	//
	// To Do Important:  Once you have implemented BindBuffers, replace the call to the pre-compiled 
	// version below to call yours!
	BindBuffers();

	// This call makes sure that the uniform parameters are setup.
	curScene->GetCurrentShader()->RegisterUniformParameters();

	// To Do
	//
	// Make the call to draw the triangles with glDrawElements here.  How many vertices do we need to call when drawing?
	glBindVertexArray(vertexArrayObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceIndexBuffer);

	glDrawElements(GL_TRIANGLES,nFaces * 3, GL_UNSIGNED_INT,0);
	// Finally you will need to unbind the buffers here.  You can do this by binding buffer #0 to each target	
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	GLERR;
}

void mesh::RecalculateNormals(void)
{	
	point p0, p1, p2;
	vector v0, v1;
	vector uv[3];
	//int i, j;

	if (!normalsSpecifiedInFile)
	{
		for (int i = 0; i < nFaces; i++)
		{
			point &p0 = vertices[faces[i].vertices[0]].position;
			point &p1 = vertices[faces[i].vertices[1]].position;
			point &p2 = vertices[faces[i].vertices[2]].position;
			faceNormals[i] = Cross(p1 - p0, p2 - p1);
			if (IsNotZero(faceNormals[i]))
				faceNormals[i].Normalize();
		}

		for (int i = 0; i < nVerts; i++)
		{
			vertices[i].normal = vector(0.0f, 0.0f, 0.0f);
			for (vertices[i].MoveFirst(); !vertices[i].AtEnd(); vertices[i].MoveNext())
				vertices[i].normal += faceNormals[vertices[i].GetCurrent()];
			if (IsNotZero(vertices[i].normal))
				vertices[i].normal.Normalize();
		}
	}
}

point mesh::Center(void) 
{
	MinMax(mMin, mMax);
	return Average(mMin, mMax);
}

point mesh::Size(void) 
{ 
	MinMax(mMin, mMax);
	return point(mMax[0] - mMin[0], mMax[1] - mMin[1], mMax[2] - mMin[2]);
}

void mesh::MinMax(point &Min, point &Max) 
{ 
	vert *v;
	if (resize)
	{
		if (nVerts < 1)
		{
			Min = Max = point(0, 0, 0);
		}
		else
		{
			v = &vertices[0];
			mMin[0] = mMax[0] = v->position[0];
			mMin[1] = mMax[1] = v->position[1];
			mMin[2] = mMax[2] = v->position[2];
			for (int i = 1; i < nVerts; i++)
			{
				v = &vertices[i];
				mMin[0] = min(mMin[0], v->position[0]);
				mMin[1] = min(mMin[1], v->position[1]);
				mMin[2] = min(mMin[2], v->position[2]);

				mMax[0] = max(mMax[0], v->position[0]);
				mMax[1] = max(mMax[1], v->position[1]);
				mMax[2] = max(mMax[2], v->position[2]);
			}
		}
		resize = false;
	}
	Min = mMin; Max = mMax;
}

point mesh::GetVertex(int k)
{
	if (k < nVerts)
	{
		vert *v = &vertices[k];
		return point(v->position[0], v->position[1], v->position[2]);
	}
	else
		return point(0, 0, 0);
}

int mesh::NVertices(void)
{
	return nVerts;
}

int mesh::NFaces(void)
{
	return nFaces;
}
