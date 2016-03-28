#ifndef __SHADER_H__
#define __SHADER_H__


#include <iostream>
using namespace std;

#include "point.h"
#include "vector.h"
#include "LinkedList.h"
#include "matrix.h"
#include "Loader.h"
#include "BitmapFile.h"
#include "OpenGLWin.h"
#include "Texture.h"

class scene;
class shape;
class vert;
class face;

class shader
{
public:

	virtual ~shader() { }

	virtual GLuint GLSLProgramObj() = 0;

	virtual void BeginShading(void) = 0;
	virtual void EndShading(void) = 0;

	virtual void RegisterUniformParameters(void) { }

	// The default loader loads nothing after the name of the shader
	virtual void Load(Loader &input) { } 
	static shader *LoadShader(Loader &input);

	virtual void SetScene(scene *newScene) { curScene = newScene; }
	virtual void SetShape(shape *newShape) { curShape = newShape; }

protected:

	scene *curScene;		// For reflection mapping
	shape *curShape;		// To retrieve size and other info
};

class glslShader : public shader
{
public:

	glslShader();
	~glslShader();

	virtual GLuint GLSLProgramObj() { return glProgramObj; }

	virtual void BeginShading(void);
	virtual void EndShading(void);

	virtual void RegisterUniformParameters(void);

	virtual void Load(Loader &input);

	virtual char* LoadShader(const char* filename);
	virtual void LoadShaders(void);
	virtual void LoadShaders_PreComp(void);

protected:

	GLuint glProgramObj;
	GLuint glVertShader;
	GLuint glFragShader;
	GLuint glGeomShader;

	char exePath[MAX_PATH];
	char vertexShaderName[MAX_PATH];
	char fragmentShaderName[MAX_PATH];
	char geometryShaderName[MAX_PATH];
	int upIndex;

	char uniformParamNames[50][50]; // Fifty uniform parameters, at 50 characters per name
	point uniformParamValues[50];
	int nUniformComponents[50];
	int nUniforms;

	static char *vertexHeader;
	static char *fragmentHeader;
};

class constantShader : public glslShader
{
public:

	constantShader();

	virtual void Load(Loader &input) { } 
};

class gouraudShader : public glslShader
{
public:

	gouraudShader();

	virtual void Load(Loader &input) { } 
};

class phongShader : public glslShader
{
public:

	phongShader();

	virtual void Load(Loader &input) { } 
};

class simpleTextureShader : public glslShader
{
public:

	simpleTextureShader();
	~simpleTextureShader() { if (tex != NULL) delete tex; }

	virtual void BeginShading(void);
	virtual void EndShading(void);

	virtual void Load(Loader &input);

	virtual void SetScene(scene *newScene) { curScene = newScene; if (tex) tex->SetCurrentScene(newScene); }
	virtual void SetShape(shape *newShape) { curShape = newShape; if (tex) tex->SetShape(newShape); }

protected:

	texture *tex;
	matrix uvTransform;
};

class multiTextureShader : public glslShader
{
public:

	multiTextureShader();
	~multiTextureShader() { for (textures.MoveFirst(); !textures.AtEnd(); textures.MoveNext()) { delete textures.GetCurrent(); } }

	virtual void BeginShading(void);
	virtual void EndShading(void);

	virtual void Load(Loader &input);

	virtual void SetScene(scene *newScene) 
		{ curScene = newScene; for (textures.MoveFirst(); !textures.AtEnd(); textures.MoveNext()) textures.GetCurrent()->SetCurrentScene(newScene); }
	
	virtual void SetShape(shape *newShape) 
		{ curShape = newShape; for (textures.MoveFirst(); !textures.AtEnd(); textures.MoveNext()) textures.GetCurrent()->SetShape(newShape); }

protected:

	LinkedList<texture *> textures;
	matrix uvTransform;
};

#endif