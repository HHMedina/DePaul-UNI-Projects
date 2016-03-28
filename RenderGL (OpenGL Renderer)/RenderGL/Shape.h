
#ifndef __SHAPE_H__
#define __SHAPE_H__


#include <iostream>
using namespace std;

#include <stdarg.h>
#include "OpenGLWin.h"
#include "point.h"
#include "vector.h"
#include "shader.h"
#include "miscio.h"
#include "LinkedList.h"
#include "Loader.h"
#include "Quat.h"
#include "Color.h"

class scene;

class shape
{
public:
	shape(const char *newName, const char *newObjName = "");
	virtual ~shape();

	const char *SetObjectName(const char* newObjName);
	char *ObjectName(void) { return objectName; }
	const char* GetShapeName(void) { return shapeName; }

	void SetDrawInScene(bool flag) { drawInScene = flag; }
	bool GetDrawInScene(void) { return drawInScene; }

	// A derived class must override the next four methods
	virtual point Center(void) = 0;
	virtual point Size(void) = 0;
	virtual void MinMax(point &min, point &max) = 0;
	virtual bool Resized(void) = 0;

	virtual void DrawShape(void) = 0;
	virtual void Draw(void);

	virtual bool FindShapeInHierarchy(shape *s, LinkedList<shape *> &sList);

	void Translate(float x, float y, float z)
	{ translation[0] += x; translation[1] += y; translation[2] += z; }

	int ShapeIndex(void);

	// The following static methods are for loading and managing 
	// shapes and connecting shape objects to an OpenGLWin that is 
	// current for a scene
	static shape* FindShape(const char* objName);
	static shape* LoadShape(Loader &input, scene *CurScene = NULL);
	static void ClearShapes(void);
	static void SetCurrentScene(scene *newCur) { curScene = newCur; }
	static scene *GetCurrentScene(void) { return curScene; }

	// These constants are important for managing shapes
	// 9973 is the largest prime number < 10000
	// Must use a prime to make hashing efficient
	enum { MAX_OBJECTS = 9973, MAX_MESSAGE_LENGTH = 1024 };

protected:

	void SetShapeName(const char *newName) { strcpy(shapeName, newName); }
	void RemoveShape(void);
	void ReleaseOpenGLObjects(void);

	virtual bool ReadSegment(Loader& input, char *token);
	virtual void ReadObject(Loader& input);
	virtual void ReadAttributes(Loader& input);
	virtual void Read(Loader& input) { }
	virtual void InitializeAttributes(void);

	virtual void PushMatrixStacks(void);
	virtual void PushMaterialStacks(void);
	virtual void PopMatrixStacks(void);
	virtual void PopMaterialStacks(void);
    virtual void ApplyTransforms(void);
	virtual void ApplyMaterial(void);

	void PlanarMap(float x, float y, float z, float &u, float &v);
	void CylindricalMap(float x, float y, float z, float &u, float &v);
	void SphericalMap(float x, float y, float z, float &u, float &v);
	void BoxMap(float x, float y, float z, float &u, float &v);
	
	char shapeName[MAX_NAME_LENGTH];
	char objectName[MAX_NAME_LENGTH];

	bool dynamic;
	
	// Transform members
	GLfloat	eulerRot[3];	// Euler angle rotations (Rx, Ry, Rx)
	matrix	arbitraryTrans;		// General transformation matrix for more flexibility
	quat	quatRotation;
	GLfloat	translation[3];	 
	GLfloat	scale[3];		

	// Object's material properties
	rgb			diffuse;
	rgb			specular;
	GLfloat		shininess;

	// Data for controlling alpha transparency
	GLenum		alphaFunc;		// Function and value for alpha testing
	GLfloat		alphaValue;		

	// Flags to tell whether the data above has been set
	bool		shininessSet;
	bool		specularSet;
	bool		diffuseSet;
	bool		alphaDataSet;
	bool		alphaTransparency;
	bool		transparencySet;

	// Flag to tell whether to draw the object 
	bool		drawInScene;

	// OpenGL Data
	GLuint vertexBuffer, faceIndexBuffer;
	GLuint vertexArrayObj;

	// Pointer to a shader for the object.  If it is null, then the object
	// will use the shader from its parent.  See the "Component" and "Instance" 
	// objects for more information about parents.
	shader			*shade;
	static scene	*curScene;	// Current scene for reflection mapping

	// These members keep track of all of the shape objects that have been 
	// loaded by the system so that we can search for them by name. 
	// See the "FindShape" method above.
	static shape* objectList[MAX_OBJECTS];
	static int HashFunction(const char *name);

	friend class scene;
};
 
#endif