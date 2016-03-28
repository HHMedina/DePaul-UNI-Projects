
#include "Texture.h"
#include "BitmapFile.h"

scene *texture::curScene = NULL;

///////////////////////////////////////////////////////////////////////
// Texture base class methods
///////////////////////////////////////////////////////////////////////

void texture::RegisterUniformParameters(int tTextureUnit, GLuint glProgramObj)
{
	char texName[256];
	if (strlen(name) == 0)
		sprintf(texName, "texure%d", tTextureUnit);
	else
		sprintf(texName, "%s", name);


	//==========Code Addition================================

	// To Do
	//
	// Use the texName string built above to set the uniform sampler for this
	// texture to the proper texture unit, i.e. tTextureUnit
	glTexParameteri(GL_SAMPLER_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	char transformName[256];
	sprintf(transformName, "%sTransform", texName);
	
	glUniform1i(glGetUniformLocation(glProgramObj, texName), tTextureUnit);

	// To Do
	//
	// Use the transformName built above to set the uniform sampler for this
	// textures transform to the transform stored in this object's 
	// textureTransform member.
	unsigned int loc = glGetUniformLocation(glProgramObj, transformName);
	glUniformMatrix4fv(loc, 1, false, textureTransform.AsArray());

	//==========Code Addition End============================

}

///////////////////////////////////////////////////////////////////////
// ImageTexture Methods
///////////////////////////////////////////////////////////////////////

imageTexture::imageTexture()
{
	tObject = 0;
	for (int i = 0; i < MAX_IMAGES; i++)
		tName[i][0] = '\0';
	tMinFilter = GL_LINEAR_MIPMAP_LINEAR;
	tMagFilter = GL_LINEAR;
	tAnisotropy = 1;
	tHorzWrap = GL_REPEAT;
	tVertWrap = GL_REPEAT;
}

imageTexture::~imageTexture()
{
	glDeleteTextures(1, &tObject);
}

void imageTexture::LoadTexture()
{
	if (tObject == 0)         // We don't yet have an OpenGL texture target
	{
		// This code counts the number of images and if there are none simply 
		// returns without doing anything
		int nImages = 0;
		while (tName[nImages][0] != '\0' && nImages < MAX_IMAGES)
			nImages++;

		if (nImages < 1)
			return;

		// To Do
		//
		// Generate a texture object and place the object's value in the "tObject"
		// member, then bind the object to the 2D texture target

		//==========Code Addition================================

		glGenTextures(1, &tObject);//Generate Texture and place handle in tObject
		glBindTexture(GL_TEXTURE_2D, tObject);

		//==========Code Addition End============================


		for (int nImage = 0; nImage < nImages; nImage++)
		{
			// This code loads the texture using the windows library's "BitmapFile" object
			BitmapFile texture;
			if (!texture.Read(tName[nImage]))
				complain("Couldn't read texture %s", tName);

//			GLuint srcFormat, targFormat;
			// To Do
			// 
			// First decide which format the texture is.  If the texture has 4 bytes
			// per pixel then it should be an RGBA texture, if it is 3 bytes per pixel
			// then it is an RGB image.  Notice though that the byte order for the BitmatFile
			// object is reversed, so you need to take that into account in the "source" format
			//
			// Then you need to set the unpack alignment to tell OpenGL about the structure 
			// of the data in the image and send the data to OpenGL.  If there are multiple files
			// then we are manually creating a mipmap here and you will use the "level" parameter
			// of glTexImage2D to tell OpenGL which mipmap level is being set.  The levels are 
			// set in the same order as they are stored in the image list.

			//==========Code Addition================================

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			if (texture.BytesPerPixel() == 3)//3 byte
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.Width(), texture.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, texture.ImageData());

			}
			else //4 byte
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width(), texture.Height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texture.ImageData());
			}

			//==========Code Addition End============================

		}
		// Finally, if there is only one image, you need to tell OpenGL to generate a mipmap

		//==========Code Addition================================

		if (nImages == 1)
		{
			glGenerateMipmap(GL_TEXTURE_2D);//Generate Mipmap
		}

		//==========Code Addition End============================

	}	

	//==========Code Addition===============================

	// Here you need to bind the texture to the 2D texture target and set the texture parameters
	// You need to set the wrap mode, the minification and magnification filters.
	glBindTexture(GL_TEXTURE_2D, tObject);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tHorzWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tVertWrap);

	// To Do 
	//
	// For advanced antialiasing set the number of anisotropic samples
	if (tAnisotropy > 1)
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)tAnisotropy);//Set Anisotropy samples
	}

	//==========Code Addition End===========================

	GLERR;
}

void imageTexture::EnableTexture(int tTextureUnit)
{
	// To Do 
	// 
	// Set the active texture unit to the desired texture unit,
	// The LoadTexture call will bind the texture to this texture unit.

	//==========Code Addition===============================

	glActiveTexture(GL_TEXTURE0 + tTextureUnit);
	glEnable(GL_TEXTURE0 + tTextureUnit);

	//==========Code Addition End===========================


	LoadTexture();

}

void imageTexture::DisableTexture(int tTextureUnit)
{
	// To Do
	// 
	// Set the active texture unit and then unbind the texture and disable texturing.
	// How do you "unbind" a tetxture?

	//==========Code Addition===============================

	glActiveTexture(GL_TEXTURE0 + tTextureUnit);
	glBindTexture(GL_TEXTURE0 + tTextureUnit, 0);//Bind active texture to default
	glDisable(GL_TEXTURE0 + tTextureUnit);

	//==========Code Addition End===========================

}

///////////////////////////////////////////////////////////////////////
// ImageCubeTexture Methods
///////////////////////////////////////////////////////////////////////

void imageCubeTexture::EnableTexture(int tTextureUnit)
{
	// To Do
	//
	// Set the active texture unit 
	//==========Code Addition===============================

	glActiveTexture(GL_TEXTURE0 + tTextureUnit);
	glEnable(GL_TEXTURE0 + tTextureUnit);
	//==========Code Addition End===========================
	LoadTexture();
}

void imageCubeTexture::DisableTexture(int tTextureUnit)
{
	// To Do
	// 
	// Set the active texture unit and then unbind the texture.
	//==========Code Addition===============================

	glActiveTexture(GL_TEXTURE0 + tTextureUnit);
	glBindTexture(GL_TEXTURE0 + tTextureUnit, 0);//Bind active texture to default
	glDisable(GL_TEXTURE0 + tTextureUnit);

	//==========Code Addition End===========================

}

void imageCubeTexture::LoadTexture()
{
	if (tObject == 0)         // We don't yet have an OpenGL texture target
	{
		// To Do
		//
		// Generate a texture object and place the object's value in the "tObject"
		// member, then bind the object to the cube map texture target
		
		//==========Code Addition================================

		glGenTextures(1, &tObject);
		glBindTexture(GL_TEXTURE_CUBE_MAP, tObject);

		//==========Code Addition End============================

		for (int nImage = 0; nImage < 6; nImage++)  // Cube maps must have 6 images, see loader!
		{
			BitmapFile texture;
			if (!texture.Read(tName[nImage]))
				complain("Couldn't read texture %s", tName);
			
//			GLuint srcFormat, targFormat;
			// To Do
			// 
			// First decide which format the texture is.  If the texture has 4 bytes
			// per pixel then it should be an RGBA texture, if it is 3 bytes per pixel
			// then it is an RGB image.  Notice though that the byte order for the BitmatFile
			// object is reversed, so you need to take that into account in the "source" format
			//
			// Then you need to set the unpack alignment to tell OpenGL about the structure 
			// of the data in the image and send the data to OpenGL  Notice that here, you 
			// will need to tell OpenGL which face of the cube map is being set.  This is done
			// in the texture target parameter of glTexImage2D.

			//==========Code Addition================================

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			if (texture.BytesPerPixel() == 3)//3 byte
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + nImage, 0, GL_RGB, texture.Width(), texture.Height(), 0, GL_BGR, GL_UNSIGNED_BYTE, texture.ImageData());

			}
			else //4 byte
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + nImage, 0, GL_RGBA, texture.Width(), texture.Height(), 0, GL_BGRA, GL_UNSIGNED_BYTE, texture.ImageData());
			}

			//==========Code Addition End============================

		}

		// We only have one list of textures, so we let OpenGL auto-mipmap them
		// the manual mipmapping was more for demonstration purposes anyway

		//==========Code Addition================================
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);//Generate Mipmap
		//==========Code Addition End============================

	}	

	// Here you need to bind the texture to the 2D texture target and set the texture parameters
	// You need to set the wrap mode, the minification and magnification filters.
	
	//==========Code Addition================================
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, tObject);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, tMagFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, tMinFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, tHorzWrap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, tVertWrap);

	// To Do Advanced
	//
	// For advanced antialiasing set the number of anisotropic samples

	if (tAnisotropy > 1)
	{
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, (GLfloat)tAnisotropy);//Set Anisotropy samples
	}

	//==========Code Addition End============================

}

