#include "Shader.h"
#include "OpenGLWin.h"
#include "BitmapFile.h"
#include "miscio.h"
#include "shape.h"
#include "scene.h"

shader *shader::LoadShader(Loader &input)
{
	char shaderType[256];
	shader *res;

	input.ReadToken(shaderType);
	if (strncmp(shaderType, "shading", 4) == 0) // If we are reading the initial "shading" command
		input.ReadToken(shaderType);

	if (strcmp(shaderType, "gouraud") == 0 || strcmp(shaderType, "faceted") == 0)
		res = new gouraudShader;
	else if (strcmp(shaderType, "phong") == 0)
		res = new phongShader;
	else if (strcmp(shaderType, "simpletexture") == 0)
		res = new simpleTextureShader;
	else if (strcmp(shaderType, "constant") == 0)
		res = new constantShader;
	else if (strcmp(shaderType, "glslshader") == 0)
		res = new glslShader;
	else if (strcmp(shaderType, "multitexture") == 0)
		res = new multiTextureShader;
	else
	{
		input.Error(shaderType, "Unknown shader: %s", shaderType);
		return NULL;
	}
	res->Load(input);
	return res;
}

void glslShader::Load(Loader &input)
{
	char token[MAX_INPUT_LENGTH];

	do
	{
		input.ReadToken(token);

		if (strcmp(token, "vertexshader") == 0)
		{
			char tmp[256];
			if (!input.ReadToken(tmp))
				input.Error("Couldn't find vertex shader name after vertexShader token in glslShader");
			else
				sprintf(vertexShaderName, "%s", tmp);
		}
		else if (strcmp(token, "fragmentshader") == 0)
		{
			char tmp[256];
			if (!input.ReadToken(tmp))
				input.Error("Couldn't find fragment shader name after vertexShader token in glslShader");
			else
				sprintf(fragmentShaderName, "%s", tmp);
		}
		else if (strcmp(token, "uniform") == 0)
		{
			input.ReadToken(uniformParamNames[nUniforms], true, false);  // This token should be upper & lower case
			nUniformComponents[nUniforms] = 0;
			for (int i = 0; i < 4; i++)
			{
				if (!input.ReadFloat(uniformParamValues[nUniforms][i]))
				{
					if (i == 0)	
						input.Error("Incomplete uniform parameter %s\n", uniformParamNames[nUniforms]);
					break;
				}
				else
					nUniformComponents[nUniforms]++;
			}
			nUniforms++;
		}
		else if (strncmp(token, "end", 3) != 0)
			input.Error(token, "Unrecognized glslShader shader command: %s", token);
	} while (strncmp(token, "end", 3) != 0);
}

void simpleTextureShader::Load(Loader &input)
{
	char token[MAX_INPUT_LENGTH];
	char tmp[MAX_INPUT_LENGTH];

	uvTransform = matrix::Identity(4);  // 4x4 identity matrix

	do
	{
		input.ReadToken(token);

		if (strncmp(token, "texture", 3) == 0)
		{
			tex = texture::LoadTexture(input);
			if (tex == NULL)
				input.Error("Unrecognized texture type in simpleTextureShader");
		}
		else if (strcmp(token, "transform") == 0)
		{
			matrix m(4, 4, 1.0);

			input.ReadToken(tmp);
			if (!strcmp(tmp, "translate"))
			{
				float x[3];
				for (int i = 0; i < 3; i++)
					if (!input.ReadFloat(x[i]))
						input.Error("Incomplete translation information for texture");	
				
				m.MakeTranslation(x[0], x[1], x[2]);
				uvTransform = m * uvTransform;
			}
			else if (!strcmp(tmp, "scale"))
			{
				float x[3];
				for (int i = 0; i < 3; i++)
					if (!input.ReadFloat(x[i]))
						input.Error("Incomplete scale information for texture");	

				m.MakeScale(x[0], x[1], x[2]);
				uvTransform = m * uvTransform;
			}
			else if (!strcmp(tmp, "rotate"))
			{
				float x[3];
				for (int i = 0; i < 3; i++)
					if (!input.ReadFloat(x[i]))
						input.Error("Incomplete rotation information for texture");	

				m.MakeRotationX(x[0]);
				uvTransform = m * uvTransform;
				m.MakeRotationY(x[1]);
				uvTransform = m * uvTransform;
				m.MakeRotationZ(x[2]);
				uvTransform = m * uvTransform;
			}
			else if (!strcmp(tmp, "matrix")) // matrix
			{
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						if (!input.ReadFloat(m(i, j)))
							input.Error("Incomplete matrix information for texture");
				uvTransform = m * uvTransform;
			}
			else if (!strcmp(tmp, "homogeneous")) // homogeneous matrix
			{
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						if (!input.ReadFloat(m(i, j)))
							input.Error("Incomplete homogeneous matrix information for texture");
				uvTransform = m * uvTransform;
			}
			else
				input.Error(tmp, "Unrecognized texture transform command: %s", tmp);
		}
		else if (strncmp(token, "end", 3) != 0)
			input.Error(token, "Unrecognized texture shader command: %s", token);
	} while (strncmp(token, "end", 3) != 0);
}

void multiTextureShader::Load(Loader &input)
{
	char token[MAX_INPUT_LENGTH];
	char tmp[MAX_INPUT_LENGTH];

	uvTransform = matrix::Identity(4);  // 4x4 identity matrix

	do
	{
		input.ReadToken(token);

		if (strncmp(token, "texture", 3) == 0)
		{
			texture *tex = texture::LoadTexture(input);
			if (tex == NULL)
				input.Error("Unrecognized texture type in multiTextureShader");
			else
				textures.Append(tex);
		}
		else if (strcmp(token, "vertexshader") == 0)
		{
			char tmp[256];
			if (!input.ReadToken(tmp))
				input.Error("Couldn't find vertex shader name after vertexShader token in multiTextureShader");
			else
				sprintf(vertexShaderName, "%s%s", exePath, tmp);
		}
		else if (strcmp(token, "fragmentshader") == 0)
		{
			char tmp[256];
			if (!input.ReadToken(tmp))
				input.Error("Couldn't find fragment shader name after vertexShader token in multiTextureShader");
			else
				sprintf(fragmentShaderName, "%s%s", exePath, tmp);
		}
		else if (strcmp(token, "transform") == 0)
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
				uvTransform = m * uvTransform;
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
				uvTransform = m * uvTransform;
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
				uvTransform = m * uvTransform;
				m.MakeRotationY(x[1]);
				uvTransform = m * uvTransform;
				m.MakeRotationZ(x[2]);
				uvTransform = m * uvTransform;
			}
			else if (!strcmp(tmp, "matrix")) // matrix
			{
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						if (!input.ReadFloat(m(i, j)))
							input.Error("Incomplete matrix information for texture");
				uvTransform = m * uvTransform;
			}
			else if (!strcmp(tmp, "homogeneous")) // homogeneous matrix
			{
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						if (!input.ReadFloat(m(i, j)))
							input.Error("Incomplete homogeneous matrix information for texture");
				uvTransform = m * uvTransform;
			}
			else
				input.Error(tmp, "Unrecognized multiTexture transform command: %s", tmp);
		}
		else if (strcmp(token, "uniform") == 0)
		{
			input.ReadToken(uniformParamNames[nUniforms], true, false);  // This token should be upper & lower case
			nUniformComponents[nUniforms] = 0;
			for (int i = 0; i < 4; i++)
			{
				if (!input.ReadFloat(uniformParamValues[nUniforms][i]))
				{
					if (i == 0)	
						input.Error("Incomplete uniform parameter %s\n", uniformParamNames[nUniforms]);
					break;
				}
				else
					nUniformComponents[nUniforms]++;
			}
			nUniforms++;
		}
		else if (strncmp(token, "end", 3) != 0)
			input.Error(token, "Unrecognized multiTexture shader command: %s", token);
	} while (strncmp(token, "end", 3) != 0);
}

