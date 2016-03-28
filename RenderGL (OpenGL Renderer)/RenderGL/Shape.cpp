
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "shape.h"
#include "scene.h"
#include "LinkedList.h"
#include "BitmapFile.h"

shape *shape::objectList[MAX_OBJECTS];
scene *shape::curScene;	

shape::shape(const char* newName, const char* newObjName)
{ 
	SetShapeName(newName); 
	SetObjectName(newObjName);

	shade = NULL;
	vertexBuffer = 0;
	faceIndexBuffer = 0;
	vertexArrayObj = 0;

	InitializeAttributes();

	dynamic = false;
}

shape::~shape() 
{ 
	RemoveShape(); 
	ReleaseOpenGLObjects();
}

void shape::ReleaseOpenGLObjects(void)
{
	if (vertexBuffer != 0)
	{
		// To Do
		//
		// Release the vertex buffer and set vertexBuffer to 0
		glDeleteBuffers(1,&vertexBuffer);
		vertexBuffer = 0;
	}

	if (faceIndexBuffer != 0)
	{
		// To Do
		//
		// Release the face index buffer and set vertexBuffer to 0
		glDeleteBuffers(1, &faceIndexBuffer);
		faceIndexBuffer = 0;
	}

	if (shade != NULL)
	{
		delete shade;
	}
}

void shape::InitializeAttributes(void)
{
	arbitraryTrans = matrix(4, 4, 1.0);
	eulerRot[0] = 0; eulerRot[1] = 0; eulerRot[2] = 0; 
	translation[0] = 0; translation[1] = 0; translation[2] = 0;
	scale[0] = 1; scale[1] = 1; scale[2] = 1; 
	shininessSet = false;
	diffuseSet = false;
	specularSet = false;
	alphaDataSet = false;
	transparencySet = false;

	if (!shade)
	{
		delete shade; // we assume that all shaders are created dynamically!!!!
		shade = NULL;
	}

	ReleaseOpenGLObjects();

	diffuse[0] = specular[0] = shininess = -1;	
}

void shape::Draw(void)
{
	if (!drawInScene)
		return;

	glDisable(GL_CULL_FACE);

	PushMatrixStacks();	
	PushMaterialStacks();

	ApplyTransforms();
	ApplyMaterial();

	if (shade)
	{
		curScene->PushShader(shade);
		shade->BeginShading();
	}
	GLERR;
	
	DrawShape();
	
	GLERR;
	if (shade)
	{
		shade->EndShading();
		curScene->PopShader();
		curScene->GetCurrentShader()->BeginShading();
	}

	PopMaterialStacks();
	GLERR;
	PopMatrixStacks();
	GLERR;
}

void shape::PushMatrixStacks(void)
{
	// To Do
	// 
	// Push the ModelTransform in the "curScene"


	curScene->PushModelTransform();
}

void shape::PushMaterialStacks(void)
{
	// To Do
	//
	// Push each of the surface material parameter stacks in the current scene

	curScene->PushSurfaceDiffuse();
	curScene->PushSurfaceShininess();
	curScene->PushSurfaceSpecular();
}

void shape::PopMatrixStacks(void)
{
	// To Do
	//
	// Pop the model transform stack in the current scene
	curScene->PopModelTransform();
}

void shape::PopMaterialStacks(void)
{
	// To Do
	//
	// Pop each of the surface material parameter stacks in the current scene

	curScene->PopSurfaceDiffuse();
	curScene->PopSurfaceShininess();
	curScene->PopSurfaceSpecular();
}

void shape::ApplyTransforms(void)
{
	// To Do
	//
	// Compute the local transform at this object using scale first, then 
	// the rotation in the order x, y and then z followed by the translation.
	// Also multiply in the matrix "arbitraryTrans" in with the rotations 
	// (have this multiply the object just BEFORE the rotations).  
	// Finally, set the model transform in the current scene to be equal
	// to the current model transform multiplied by the local transform 
	// you just calcualted.  Make sure you do this in the correct order.
	//
	// Advanced: Once you've implemented quaternions, convert the quaternion
	// to a matrix and multiply it in here with the other roatations


	/* TRASp*/
	matrix LocalTransform = matrix::Translation(translation[0], translation[1], translation[2]) *
		arbitraryTrans * // is this before or after?
		matrix::RotationZ(eulerRot[2]) *
		matrix::RotationY(eulerRot[1]) *
		matrix::RotationX(eulerRot[0]) *
		matrix::Scale(scale[0], scale[1], scale[2]);

	curScene->SetModelTransform(curScene->GetCurrentModelTransform() * LocalTransform);

}

void shape::ApplyMaterial(void)
{
	// To Do
	//
	// Apply the material properties to the scene's stack.  The RGB color class 
	// has several static members for common colors, e.g. rgb::black, rgb::white.  
	// Black can be used to turn specularity off if requested by the object, and 
	// white is the default specular color and should be used if specularity is 
	// requested (with a shininess setting) but no specular color is specified.
	//
	// The color class "rgb" in the Color.cpp is compatible with the
	// OpenGL material functions.  Just use the "AsArray()" member
	// to pass the underlying array of floats.
	//
	// Remember that we have Four different properties to set.  
	//
	// Ambient reflectance, Diffuse reflectance, 
	// Secular reflectance, and Shininess coefficient
	//
	// Make sure that the specular highlight is correct on your object
	// Use the "propertySet" members to tell which material parameters 
	// you should change.  There is one kind of tricky aspect to the shininess
	// however.  In the rgl file format, specularity is turned off with a shininess
	// set explicitly to 0 (i.e. shininess = 0 && shininessSet == true).  If 
	// This is the case, then you should set the specular color to black.
	//
	// Also, scenes may turn shininess on and not set specular.  If this is 
	// the case (i.e. shininess > 0 && specularSet == false) then you should
	// set the specualr color to white.
	//
	// Finally if transparency is set to true on the object turn on alpha 
	// blending in OpenGL, and if it is false disable alpha blending.
	
	/*Diffuse*/
	if (diffuseSet)
	{
		curScene->SetSurfaceDiffuse(diffuse);
	}

	
	if (shininess>=0)
	{
		curScene->SetSurfaceShininess(shininess);
		if (specularSet)
		{
			curScene->SetSurfaceSpecular(specular);
		}
		else
		{
			curScene->SetSurfaceSpecular(rgb::white);
		}
	}
	else if (shininessSet)
	{
		curScene->SetSurfaceSpecular(rgb::black);
	}
	

	/*Alpha Not Done Yet*/

	if (transparencySet)
	{
		if (alphaTransparency)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

}

bool shape::FindShapeInHierarchy(shape *s, LinkedList<shape *> &sList)
{
	if (s == this)
	{
		sList.Append(this);
		return true;
	}
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
// The following functions manage the internal hash table of loaded shapes
// They allow for searching for shapes by name, which will retrieve a pointer
// to the shape.
///////////////////////////////////////////////////////////////////////////////

// The following function calculates a position in the object array
// Based on a shapes "object name".  It does so by adding up all the
// ascii codes of the characters in the name and taking the remainder
// of this count by MAX_OBJECTS.  Note that this only works efficiently
// if MAX_OBJECTS is a prime number.  For information on why this is
// true see any introductory text on number theory.
int shape::HashFunction(const char *name)
{
	int i = 0;
	uint count = 0;

	while (name[i] != '\0')
		count = count + name[i++];
	return count % MAX_OBJECTS;	
}

void shape::ClearShapes(void)
{
	for (int i = 0; i < MAX_OBJECTS; i++)
	{
		shape *s = objectList[i];
		if (s != NULL && s->dynamic == true)
			delete s;
		objectList[i] = NULL;
	}
}

void shape::RemoveShape(void)
{
	int index = ShapeIndex();
	if (index >= 0)
		objectList[index] = NULL;
}

shape* shape::FindShape(const char *objName)
{
	char tmpName[MAX_INPUT_LENGTH];

	strcpy(tmpName, objName);

	LCase(tmpName);
	int index, startIndex = HashFunction(tmpName);

	index = startIndex;
	while (objectList[index] != NULL)
	{
		if (strcmp(tmpName, objectList[index]->objectName) == 0)
			return objectList[index];
		index++;
		if (index >= MAX_OBJECTS)
			index = 0;
		if (index == startIndex)
			break;
	}
	return NULL;  // Not Found!
}

const char* shape::SetObjectName(const char *newObjName)
{
	char Name[MAX_NAME_LENGTH];

	if (strlen(newObjName) == 0)
		strcpy(Name, shapeName); 
	else 
		strcpy(Name, newObjName);

	int index, startIndex = HashFunction(Name);
	index = startIndex;

	while (objectList[index] != NULL)
	{
		index++;
		if (index >= MAX_OBJECTS)
			index = 0;
		if (index == startIndex)
			complain("Object List Full:  Too many shapes!");
	}
	
	RemoveShape();

	strcpy(objectName, Name);
	objectList[index] = this;
	return objectName;
}

int shape::ShapeIndex(void)
{
	int startIndex = HashFunction(objectName);
	int index = startIndex;

	while (objectList[index] != this)
	{
		index++;
		if (index >= MAX_OBJECTS)
			index = 0;
		if (index == startIndex)
		{
			index = -1; // Not found
			break;
		}
	}
	return index;
}

