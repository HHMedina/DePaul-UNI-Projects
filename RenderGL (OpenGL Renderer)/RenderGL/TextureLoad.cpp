
#include "Texture.h"
#include "Shape.h"

texture *texture::LoadTexture(Loader &input)
{
	char textureType[256];
	texture *res;

	input.ReadToken(textureType);
	if (strncmp(textureType, "texture", 4) == 0) // If we are reading the initial "shading" command
		input.ReadToken(textureType);

	if (strcmp(textureType, "imagetexture") == 0)
		res = new imageTexture;
	else if (strcmp(textureType, "imagecubetexture") == 0)
		res = new imageCubeTexture;
	else
	{
		input.Error(textureType, "Unknown texture: %s", textureType);
		return NULL;
	}
	res->Load(input);
	return res;
}

void texture::Load(Loader &input)
{
	ClearProperties();

	char token[MAX_INPUT_LENGTH];
	int oldPos = 0;
	do
	{
		input.ReadToken(token);				// Be an end, so we have to move back so that input function
		if (strncmp(token, "end", 3) != 0)
			if (!ReadAttribute(input, token))
				input.Error(token, "Unrecognized texture command: %s", token);
	} while (strncmp(token, "end", 3) != 0);
}

bool texture::ReadAttribute(Loader &input, char *token)
{
	char tmp[MAX_INPUT_LENGTH];
	if (strcmp(token, "transform") == 0)
	{
		matrix m(4, 4, 1.0);

		input.ReadToken(tmp);
		if (!strcmp(tmp, "translate"))
		{
			float x[3];
			int nComp;
			for (nComp = 0; nComp < 3; nComp++)
				if (!input.ReadFloat(x[nComp]))
					break;
			
			if (nComp < 2)
				input.Error("Incomplete translate information for texture");	
			else if (nComp == 2)
				x[2] = 0.0;

			m.MakeTranslation(x[0], x[1], x[2]);
			textureTransform = m * textureTransform;
		}
		else if (!strcmp(tmp, "scale"))
		{
			float x[3];
			int nComp;
			for (nComp = 0; nComp < 3; nComp++)
				if (!input.ReadFloat(x[nComp]))
					break;
			
			if (nComp < 2)
				input.Error("Incomplete scale information for texture");	
			else if (nComp == 2)
				x[2] = 0.0;

			m.MakeScale(x[0], x[1], x[2]);
			textureTransform = m * textureTransform;
		}
		else if (!strcmp(tmp, "rotate"))
		{
			float x[3];
			int nComp;
			for (nComp = 0; nComp < 3; nComp++)
				if (!input.ReadFloat(x[nComp]))
					break;

			if (nComp == 0)
				input.Error("Incomplete rotation information for texture");
			else if (nComp == 1)  // Assume a rotation about the z-axis
			{
				x[2] = x[0];
				x[0] = x[1] = 0.0;
			}
			else if (nComp == 2)
				x[2] = 0.0;
				

			m.MakeRotationX(x[0]);
			textureTransform = m * textureTransform;
			m.MakeRotationY(x[1]);
			textureTransform = m * textureTransform;
			m.MakeRotationZ(x[2]);
			textureTransform = m * textureTransform;
		}
		else if (!strcmp(tmp, "matrix")) // matrix
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (!input.ReadFloat(m(i, j)))
						input.Error("Incomplete matrix information for texture");
			textureTransform = m * textureTransform;
		}
		else if (!strcmp(tmp, "homogeneous")) // homogeneous matrix
		{
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					if (!input.ReadFloat(m(i, j)))
						input.Error("Incomplete homogeneous matrix information for texture");
			textureTransform = m * textureTransform;
		}
		else
			input.Error(tmp, "Unrecognized texture transform command: %s", tmp);
		return true;
	}
	else if (strcmp(token, "name") == 0)
	{
		if (!input.ReadToken(name, true, false))
			input.Error(name, "Incomplete texture name command in texture shader");
		return true;
	}
	return false;
}

bool imageTexture::ReadAttribute(Loader &input, char *token)
{
	char tmp[MAX_INPUT_LENGTH];

	if (texture::ReadAttribute(input, token))
		return true;
	else if (strncmp(token, "image", 3) == 0 || strncmp(token, "texture", 3) == 0)
	{
		int index;

		if (!input.ReadToken(tmp))
			input.Error(tmp, "Incomplete texture command in texture shader");

		if (!input.ReadInt(index))
			strcpy(tName[0], tmp);  // It is a single texture command
		else
			strcpy(tName[index], tmp);
		return true;
	}
	else if (strncmp(token, "magfilter", 9) == 0)
	{
		if (!input.ReadToken(tmp))
			input.Error(tmp, "Incomplete Magnification Filter Command");

		if (strncmp(tmp, "nearest", 4) == 0)
			tMagFilter = GL_NEAREST;
		else if (strncmp(tmp, "linear", 4) == 0)
			tMagFilter = GL_LINEAR;
		else
			input.Error(tmp, "Unrecognized texture magnification filter: %s", tmp);
		return true;
	}
	else if (strncmp(token, "minfilter", 9) == 0)
	{
		if (!input.ReadToken(tmp))
			input.Error(tmp, "Incomplete Minification Filter Command");

		if (strcmp(tmp, "nearest") == 0)
			tMinFilter = GL_NEAREST;
		else if (strcmp(tmp, "linear") == 0)
			tMinFilter = GL_LINEAR;
		else if (strcmp(tmp, "nearest_mipmap") == 0)
			tMinFilter = GL_NEAREST_MIPMAP_NEAREST;
		else if (strncmp(tmp, "linear_mipmap", 13) == 0 || strcmp(tmp, "bilinear") == 0)
			tMinFilter = GL_LINEAR_MIPMAP_NEAREST;
		else if (strcmp(tmp, "nearest_mipmap_linear") == 0)
			tMinFilter = GL_NEAREST_MIPMAP_LINEAR;
		else if (strcmp(tmp, "trilinear") == 0)
			tMinFilter = GL_LINEAR_MIPMAP_LINEAR;
		else
			input.Error(tmp, "Unrecognized texture magnification filter: %s", tmp);
		return true;
	}
	else if (strncmp(token, "aniso", 5) == 0)
	{
		if (!input.ReadInt(tAnisotropy))
			input.Error("Texture command 'anisotropy' must be followed with an integer");
		return true;
	}
	else if (strncmp(token, "hwrap", 5) == 0)
	{
		if (!input.ReadToken(tmp))
			input.Error(tmp, "Incomplete Horizontal Wrap Command");

		if (strcmp(tmp, "repeat") == 0)
			tHorzWrap = GL_REPEAT;
		else if (strcmp(tmp, "clamp") == 0)
			tHorzWrap = GL_CLAMP;
		else if (strcmp(tmp, "edge") == 0 || strcmp(tmp, "clampedge") == 0)
			tHorzWrap = GL_CLAMP_TO_EDGE;
		else
			input.Error(tmp, "Unrecognized horizontal wrap mode: %s", tmp);
		return true;
	}
	else if (strncmp(token, "vwrap", 5) == 0)
	{
		if (!input.ReadToken(tmp))
			input.Error(tmp, "Incomplete Vertical Wrap Command");

		if (strcmp(tmp, "repeat") == 0)
			tVertWrap = GL_REPEAT;
		else if (strcmp(tmp, "clamp") == 0)
			tVertWrap = GL_CLAMP;
		else if (strcmp(tmp, "edge") == 0 || strcmp(tmp, "clampedge") == 0)
			tVertWrap = GL_CLAMP_TO_EDGE;
		else
			input.Error(tmp, "Unrecognized vertical wrap mode: %s", tmp);
		return true;
	}
	return false;
}

void imageCubeTexture::Load(Loader &input)
{
	imageTexture::Load(input);

	int nImages = 0;
	while (tName[nImages][0] != '\0' && nImages < MAX_IMAGES)
		nImages++;

	if (nImages != 6)
		input.Error("Cube maps must have six images");
}

