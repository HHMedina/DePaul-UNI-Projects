
#include "Shader.h"
#include "OpenGLWin.h"
#include "BitmapFile.h"
#include "miscio.h"
#include "shape.h"
#include "scene.h"
#include "time.h"
#include "Mesh.h"

#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////
// Utility functions (local to this file)
////////////////////////////////////////////////////////////////////

static void GetExeFolder(char *path, size_t pathSize)  
{  
    if (!GetModuleFileName(0, path, pathSize))  
        path[0] = '\0';   // insure it's NUL terminated  
	else
	{
		char *lastslash = NULL, *tmpPath = path;  
	    
		do 
		{  
			if (*tmpPath == '\\' || *tmpPath == '/')  
				lastslash = tmpPath; 
			tmpPath++;
		} while (*tmpPath != '\0');

		if (lastslash) 
		{
			*lastslash++ = '\\';
			*lastslash = '\0';
		}
	}	   
}  

///////////////////////////////////////////////////////////////////////
// glslShader class implementation
///////////////////////////////////////////////////////////////////////

char *glslShader::vertexHeader = NULL;
char *glslShader::fragmentHeader = NULL;

glslShader::glslShader()
{
	glProgramObj = 0;
	glVertShader = 0;
	glFragShader = 0;
	glGeomShader = 0;
	vertexShaderName[0] = '\0';
	fragmentShaderName[0] = '\0';
	geometryShaderName[0] = '\0';
	upIndex = -1;
	curShape = NULL;
	curScene = NULL;

	nUniforms = 0;

	GetExeFolder(exePath, MAX_PATH);

	if (vertexHeader == NULL)	// Then load the header files, but do it only once
	{							// Remember, these variables are STATIC
		vertexHeader = LoadShader("header.vert");
		fragmentHeader = LoadShader("header.frag");
	}
}

glslShader::~glslShader()
{
	// To Do
	// 
	// Clean up by delete the shaders and the program object it is attached to.
}

void glslShader::BeginShading(void)
{
	LoadShaders();
}

void glslShader::RegisterUniformParameters(void)
{
	if (curScene)
		curScene->RegisterUniformParameters(glProgramObj);
	
	if (curShape)
	{
		point center, min, max;
		
		// To Do
		//
		// Query the min, max and ceter and send them to GLSL as the uniforms
		// objMin, objMax, and objCenter

		//========================Code Additions===========================

		unsigned int loc = glGetUniformLocation(glProgramObj, "objCenter");
		glUniform4fv(loc, 1,curShape->Center().AsArray());

		//========================Code Additions End=======================
	}

	for (int i = 0; i < nUniforms; i++)
	{
		// To Do
		//
		// Use the i'th uniformParamName to query the location of the 
		// parameter in the GLSL shader, and then use the number of components
		// in the i'th nUniformComponents to decide which glUniform call to make
		// The value is in the i'th uniformParamValues.
	}

	GLERR;
}

char *glslShader::LoadShader(const char *fileName)
{
	int size;

	unsigned char *buffer;
	FILE *shaderFile = fopen(fileName, "rb");
	if (shaderFile == NULL)
	{
		cout << "Unable to open shader file " << fileName << endl;
		return NULL;
	}

	fseek(shaderFile, 0, SEEK_END);
	size = ftell(shaderFile);
	rewind(shaderFile);

	buffer = new unsigned char[size + 1];
	int num = fread(buffer, 1, size, shaderFile);
	buffer[num] = '\0';

	fclose(shaderFile);
	return (char *)buffer;
}

void glslShader::LoadShaders(void)
{
	if (!glProgramObj)
	{
		char *shaderCode[2];
		GLint good;
		char errorLog[4096];

		glVertShader = glCreateShader(GL_VERTEX_SHADER);
		char *vertCode = LoadShader(vertexShaderName);
		if (vertCode == NULL || strlen(vertCode) == 0)
			complain("Couldn't open shader file %s\n", vertexShaderName);

		// The main thing we do here is we add a "header" file so that we 
		// can pre-define some stuff.  The only thing this does is to throw
		// off the numbers for errors, so we will keep these to a minimum
		// to start.  (Some GPUs do not support a method that can correct this :)

		shaderCode[0] = vertexHeader;	// Include the header file
		shaderCode[1] = vertCode;
		glShaderSource(glVertShader, 2, (const char **)shaderCode, NULL);
		glCompileShader(glVertShader);

		glGetObjectParameterivARB(glVertShader, GL_OBJECT_COMPILE_STATUS_ARB, &good);
		if (!good)
		{
			cout << endl << "Vertex Shader '" << vertexShaderName << "' failed to compile" << endl;
			glGetShaderInfoLog(glVertShader, 4096, NULL, errorLog);
			complain(errorLog);
		}

		glFragShader = glCreateShader(GL_FRAGMENT_SHADER);
		char *fragCode = LoadShader(fragmentShaderName);
		if (fragCode == NULL || strlen(fragCode) == 0)
			complain("Couldn't open shader file %s\n", fragmentShaderName);

		shaderCode[0] = fragmentHeader;	// Include the header file
		shaderCode[1] = fragCode;
		glShaderSource(glFragShader, 2, (const char **)shaderCode, NULL);
		glCompileShader(glFragShader);

		glGetObjectParameterivARB(glFragShader, GL_OBJECT_COMPILE_STATUS_ARB, &good);
		if (!good)
		{
			cout << endl << "Fragment Shader '" << fragmentShaderName << "' failed to compile" << endl;
			glGetShaderInfoLog(glFragShader, 4096, NULL, errorLog);
			complain(errorLog);
		}

		glProgramObj = glCreateProgram();
		glAttachShader(glProgramObj, glVertShader);
		glAttachShader(glProgramObj, glFragShader);
		glLinkProgram(glProgramObj);

		glGetObjectParameterivARB(glProgramObj, GL_OBJECT_LINK_STATUS_ARB, &good);
		if (!good)
		{
			cout << "Program and Shaders objects failed to link." << endl;
			glGetProgramInfoLog(glProgramObj, 4096, NULL, errorLog);
			complain(errorLog);
		}

		if (vertCode) delete [] vertCode;
		if (fragCode) delete [] fragCode;
	}
	glUseProgram(glProgramObj);
	
	GLERR;
}

void glslShader::EndShading(void)
{
	glUseProgram(0);
}

///////////////////////////////////////////////////////////////////////
// constantShader class implementation
///////////////////////////////////////////////////////////////////////

constantShader::constantShader()
{
	FILE *f;

	GetExeFolder(vertexShaderName, MAX_PATH);
	strcat(vertexShaderName, "..\\constant.vert");
	f = fopen(vertexShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("Constant vertex shader not found in build or exe folder\nScenes will not be able to use the built-in 'constant' shader");

	f = NULL;

	GetExeFolder(fragmentShaderName, MAX_PATH);
	strcat(fragmentShaderName, "..\\constant.frag");
	f = fopen(fragmentShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("Constant fragment shader not found in build or exe folder\nScenes will not be able to use the built-in 'constant' shader");
}


///////////////////////////////////////////////////////////////////////
// gouraudShader class implementation
///////////////////////////////////////////////////////////////////////

gouraudShader::gouraudShader()
{
	FILE *f;

	GetExeFolder(vertexShaderName, MAX_PATH);
	strcat(vertexShaderName, "..\\gouraud.vert");
	f = fopen(vertexShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("Gouraud vertex shader not found in build or exe folder\nScenes will not be able to use the built-in 'gouraud' shader");

	f = NULL;

	GetExeFolder(fragmentShaderName, MAX_PATH);
	strcat(fragmentShaderName, "..\\gouraud.frag");
	f = fopen(fragmentShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("Gouraud fragment shader not found in build or exe folder\nScenes will not be able to use the built-in 'gouraud' shader");
}


///////////////////////////////////////////////////////////////////////
// phongShader class implementation
///////////////////////////////////////////////////////////////////////

phongShader::phongShader()
{
	FILE *f;

	GetExeFolder(vertexShaderName, MAX_PATH);
	strcat(vertexShaderName, "..\\phong.vert");
	f = fopen(vertexShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("Phong vertex shader not found in build or exe folder\nScenes will not be able to use the built-in 'phong' shader");

	f = NULL;

	GetExeFolder(fragmentShaderName, MAX_PATH);
	strcat(fragmentShaderName, "..\\phong.frag");
	f = fopen(fragmentShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("Phong fragment shader not found in build or exe folder\nScenes will not be able to use the built-in 'phong' shader");
}

///////////////////////////////////////////////////////////////////////
// simpleTextureShader class implementation
///////////////////////////////////////////////////////////////////////

simpleTextureShader::simpleTextureShader()
{
	FILE *f;

	GetExeFolder(vertexShaderName, MAX_PATH);
	strcat(vertexShaderName, "..\\simpleTex.vert");
	f = fopen(vertexShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("simpleTex vertex shader not found in build or exe folder\nScenes will not be able to use the built-in 'simpleTex' shader");

	f = NULL;

	GetExeFolder(fragmentShaderName, MAX_PATH);
	strcat(fragmentShaderName, "..\\simpleTex.frag");
	f = fopen(fragmentShaderName, "rb");
	if (f != NULL)
		fclose(f);
	else
		printf("simpleTex fragment shader not found in build or exe folder\nScenes will not be able to use the built-in 'simpleTex' shader");
}

void simpleTextureShader::BeginShading(void)
{
	glslShader::BeginShading();

	// To Do 
	//
	// Set the uniform matrix transformation "uvTransform" 
	
	
	//==========Code Addition================================
	unsigned int loc = glGetUniformLocation(glProgramObj, "uvTransform");
	glUniformMatrix4fv(loc, 1, false, uvTransform.AsArray());
	//==========Code Addition End============================

	curScene->RegisterUniformParameters(glProgramObj);

	tex->EnableTexture(0);
	GLERR;
}

void simpleTextureShader::EndShading(void)
{
	glslShader::EndShading();

	tex->DisableTexture(0);
}

///////////////////////////////////////////////////////////////////////
// multiTextureShader class implementation
///////////////////////////////////////////////////////////////////////

multiTextureShader::multiTextureShader()
{
	textures.Clear();
}

void multiTextureShader::BeginShading(void)
{
	glslShader::BeginShading();

	// To Do 
	//
	// Set the uniform matrix transformation "uvTransform" 

	//==========Code Addition================================
	unsigned int loc = glGetUniformLocation(glProgramObj, "uvTransform");
	glUniformMatrix4fv(loc, 1, false, uvTransform.AsArray());
	//==========Code Addition End============================


	curScene->RegisterUniformParameters(glProgramObj);

	int i = 0;
	for (textures.MoveFirst(); !textures.AtEnd(); textures.MoveNext(), i++)
	{
		texture *tex = textures.GetCurrent();
		tex->EnableTexture(i);
		tex->RegisterUniformParameters(i, glProgramObj);;
	}
}

void multiTextureShader::EndShading(void)
{
	glslShader::EndShading();

	int i = 0;
	for (textures.MoveFirst(); !textures.AtEnd(); textures.MoveNext(), i++)
		textures.GetCurrent()->DisableTexture(i);
}

