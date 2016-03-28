#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "OpenGLWin.h"
#include "Point.h"
#include "Vector.h"
#include "Loader.h"
#include "MiscIO.h"
#include "Color.h"
#include "Matrix.h"

class scene;
class shape;

class texture 
{
public:

	texture() { name[0] = '\0'; }

	virtual void SetShape(shape *s) { curShape = s; }

	virtual void LoadTexture(void) = 0;
	virtual void EnableTexture(int tTextureUnit) = 0;
	virtual void DisableTexture(int tTextureUnit) = 0;

	virtual void RegisterUniformParameters(int tTextureUnit, GLuint glProgramObj);

	virtual void Load(Loader &input); 

	static texture *LoadTexture(Loader &input);

	static void SetCurrentScene(scene *newCur) { curScene = newCur; }
	static scene *GetCurrentScene(void) { return curScene; }

protected:
	
	virtual void ClearProperties(void) { name[0] = '\0'; textureTransform = matrix(4, 4, 1); }
	virtual bool ReadAttribute(Loader &input, char *token);

	char name[256];
	matrix textureTransform;  // Transform will have the name "name"+Transform
	shape *curShape;

	static scene *curScene;
};

class imageTexture : public texture
{
public:

	imageTexture();
	~imageTexture();

	virtual void LoadTexture(void);
	virtual void EnableTexture(int tTextureUnit);
	virtual void DisableTexture(int tTextureUnit);

	enum { MAX_IMAGES = 20, IMAGE_NAME_LENGTH = 256 };

protected:

	virtual bool ReadAttribute(Loader &input, char *token);

	unsigned int tObject;						// The OpenGL texture object
	char tName[MAX_IMAGES][IMAGE_NAME_LENGTH];	// The texture file names, allows for mipmapping and OpenGL box mapping
	int tMinFilter;			// The minimization filter used by the texture
	int tMagFilter;			// The magnification filter used by the texture
	int tAnisotropy;		// The level of anisotropic filtering
	int tHorzWrap;			// The horizontal wrap mode
	int tVertWrap;			// The vertical wrap mode
};

class imageCubeTexture : public imageTexture
{
public:

	virtual void LoadTexture(void);
	virtual void EnableTexture(int tTextureUnit);
	virtual void DisableTexture(int tTextureUnit);

	virtual void Load(Loader &input);
};

#endif