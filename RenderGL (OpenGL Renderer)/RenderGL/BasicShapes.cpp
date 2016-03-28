
#include <math.h>
#include "BasicShapes.h"
#include "Scene.h"

////////////////////////////////////////////////////////////////////////////
// Methods for the sphere class
////////////////////////////////////////////////////////////////////////////

sphere::~sphere()
{
}

void sphere::SetSphere(point c, float r, int res)
{
	center = c;
	radius = r;
	resolution = res;
	SetupShape();
}
 
void sphere::SetupShape(void)
{
	Erase((2 * resolution + 1) * (resolution + 1), 4 * (resolution + 1) * resolution); 
	// To Do
	// 
	// Setup the data for a sphere

	float pi = 3.14159265358979323846f;

	/*
	Strips refer to the "columns" of the sphere,
	where as, rings, refer to the "rows" of the sphere.
	*/



	int longitudeLines = resolution + 1;
	int latitudeLines = resolution + 1;
	nVerts = longitudeLines*latitudeLines;
	int numTriangleVerts = resolution*resolution * 2 * 3;
	int currentVertexIdx = 0;
	float currentPhi = 0.0f;
	float phiIncrement = (pi / (float)(resolution));
	float currentTheta = 0.0f;
	float thetaIncrement = 2.0f * (pi / (float)(resolution));

	for (int i = 0; i < longitudeLines; i++) //for each strip
	{
		currentPhi = 0.0f; //reset Phi since its a new strip
		for (int j = 0; j < latitudeLines; j++) //for each ring
		{
			//Note: Each face has four vertices...which will be used to make 

			vertices[currentVertexIdx].position = point(cosf(currentTheta)*sinf(currentPhi), sinf(currentTheta)*sinf(currentPhi), cosf(currentPhi)) * radius;
			vertices[currentVertexIdx].normal = vector(cosf(currentTheta)*sinf(currentPhi), sinf(currentTheta)*sinf(currentPhi), cosf(currentPhi));
			if (IsZero(vertices[currentVertexIdx].normal)){
				vertices[currentVertexIdx].normal.Normalize();
			}

			currentVertexIdx++; //skip forward to next idx for new face
			currentPhi += phiIncrement;
		}

		currentTheta += thetaIncrement; //go to next strip
	}

	for (int i = 0; i < resolution; i++) //for each strip
	{

		for (int j = 0; j < resolution; j++) //for each ring
		{
			//Note: Each face has four vertices...which will be used to make 

			faces[nFaces].vertices[0] = j + (i*latitudeLines);
			faces[nFaces].vertices[1] = j + (i*latitudeLines) + 1;
			faces[nFaces++].vertices[2] = j + ((i + 1)*latitudeLines);

			faces[nFaces].vertices[0] = j + ((i + 1)*latitudeLines);
			faces[nFaces].vertices[1] = j + (i*latitudeLines) + 1;
			faces[nFaces++].vertices[2] = j + ((i + 1)*latitudeLines) + 1;

		}
	}
}

void sphere::DrawShape(void)
{
	glCullFace(GL_FRONT);
	mesh::DrawShape();
	glCullFace(GL_BACK);
	mesh::DrawShape();
}

void sphere::Read(Loader &input)
{
	char token[MAX_INPUT_LENGTH];

	bool moreTokens;
	do 
	{
		// We need to "peek" because if the token isn't giving us one
		// of the sphere properties, then it is a shape attribute like
		// color and if we've already read it out of the file, the shape
		// class would not be able to read the attribute afterwords.
		// So we "peek" at the token which does not remove it from the 
		// file stream.  For more information, see the "PeekTokens" 
		// method defined in the Loader class
		input.PeekTokens(token, 1);
		
		moreTokens = true;
		if (strcmp(token, "center") == 0)
		{
			input.ReadToken(token);
			for (int i = 0; i < 3; i++)
				if (!input.ReadFloat(center[i]))
					input.Error("Not enough parameters for sphere's center in sphere '%s'", objectName);
		}
		else if (strcmp(token, "radius") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadFloat(radius))
				input.Error("Error reading radius in sphere '%s'", objectName);
		}
		else if (strcmp(token, "resolution") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadInt(resolution))
				input.Error("Error reading resolution in sphere '%s'", objectName);
		}
		else 
			moreTokens = false; // If we don't recognize it jump out ... it is an attribute
	} while (moreTokens);

	SetupShape();
}

////////////////////////////////////////////////////////////////////////////
// Methods for the cylinder class
////////////////////////////////////////////////////////////////////////////

cylinder::~cylinder()
{
}

void cylinder::SetCylinder(point c, float r, float h, int res, int a, bool capped)
{
	base = c;
	radius = r;
	height = h;
	axis = a;
	resolution = res;
	SetupShape();
}
 
void cylinder::SetupShape(void)
{
	Erase(6 * (resolution + 1), 4 * resolution);  // The vertices need to be doubled for the caps, and the extra 2 sets are for the centers of the caps 
												  // which need to have many copies of the same vertex with different texture coordinates

	float thetaInc = 2 * PIf / resolution;
	float thetaLimit = 2 * PIf + thetaInc / 2.0f;
	int i;
	float theta;
	int bottomCapStart, topCapStart;

	nVerts = 0;

	vector a, v0, v1;

	if (axis == 0)
	{
		a = vector(1, 0, 0);
		v0 = vector(0, 1, 0);
		v1 = vector(0, 0, 1);
	}
	if (axis == 1)
	{
		a = vector(0, 1, 0);
		v0 = vector(0, 0, 1);
		v1 = vector(1, 0, 0);
	}
	else
	{
		a = vector(0, 0, 1);
		v0 = vector(1, 0, 0);
		v1 = vector(0, 1, 0);
	}

	// First we do the body of the cylinder
	for (i = 0, theta = 0.0f; i <= resolution; i++, theta += thetaInc)
	{
		float cTheta = cos(theta), sTheta = sin(theta);

		vertices[nVerts].position = base + radius * (cTheta * v0 + sTheta * v1);
		vertices[nVerts].normal = cTheta * v0 + sTheta * v1;
		vertices[nVerts].uv = vector(theta / (2 * PIf), 0.0);
		vertices[nVerts].dPdu = radius * (-sTheta * v0 + cTheta * v1);
		vertices[nVerts++].dPdv = height * a;

		vertices[nVerts].position = base + radius * (cTheta * v0 + sTheta * v1) + height * a;
		vertices[nVerts].normal = cTheta * v0 + sTheta * v1;
		vertices[nVerts].uv = vector(theta / (2 * PIf), 0.0);
		vertices[nVerts].dPdu = radius * (-sTheta * v0 + cTheta * v1);
		vertices[nVerts++].dPdv = height * a;
	}

	// Now we do the caps
	if (capped)
	{
		bottomCapStart = nVerts;

		// First the bottom cap
		for (i = 0, theta = 2 * PIf; i <= resolution; i++, theta -= thetaInc)  // negative direction to preserve right handedness on the bottom
		{
			float cTheta = cos(theta), sTheta = sin(theta);

			vertices[nVerts].position = base;
			vertices[nVerts].normal = -a;
			vertices[nVerts].uv = vector(theta / (2 * PIf), 1.0);
			vertices[nVerts].dPdu = vector(0, 0, 0);
			vertices[nVerts++].dPdv = radius * (cTheta * v0 + sTheta * v1);

			vertices[nVerts].position = base + radius * cTheta * v0 + radius * sTheta * v1;
			vertices[nVerts].normal = -a;
			vertices[nVerts].uv = vector(theta / (2 * PIf), 0.0);
			vertices[nVerts].dPdu = radius * (-sTheta * v0 + cTheta * v1);
			vertices[nVerts++].dPdv = radius * (cTheta * v0 + sTheta * v1);
		}
		topCapStart = nVerts;

		// Now the top cap
		for (i = 0, theta = 0.0f; i <= resolution; i++, theta += thetaInc)  
		{
			float cTheta = cos(theta), sTheta = sin(theta);

			vertices[nVerts].position = base + height * a;
			vertices[nVerts].normal = a;
			vertices[nVerts].uv = vector(theta / (2 * PIf), 1.0);
			vertices[nVerts].dPdu = vector(0, 0, 0);
			vertices[nVerts++].dPdv = radius * (cTheta * v0 + sTheta * v1);

			vertices[nVerts].position = base + radius * cTheta * v0 + radius * sTheta * v1 + height * a;
			vertices[nVerts].normal = a;
			vertices[nVerts].uv = vector(theta / (2 * PIf), 0.0);
			vertices[nVerts].dPdu = radius * (-sTheta * v0 + cTheta * v1);
			vertices[nVerts++].dPdv = radius * (cTheta * v0 + sTheta * v1);
		}
	}

	nFaces = 0;
	for (int i = 0; i < resolution; i++)
	{
		faces[nFaces].vertices[0] = 2 * i;
		faces[nFaces].vertices[1] = 2 * i + 1;
		faces[nFaces++].vertices[2] = 2 * i + 2;

		faces[nFaces].vertices[0] = 2 * i + 1;
		faces[nFaces].vertices[1] = 2 * i + 3;
		faces[nFaces++].vertices[2] = 2 * i + 2;
	}

	if (capped)
	{
		// First, the bottom cap
		for (int i = 0; i < resolution; i++)
		{
			faces[nFaces].vertices[0] = bottomCapStart + 2 * i;
			faces[nFaces].vertices[1] = bottomCapStart + 2 * i + 1;
			faces[nFaces++].vertices[2] = bottomCapStart + 2 * i + 3;
		}

		// Next, the bottom cap
		for (int i = 0; i < resolution; i++)
		{
			faces[nFaces].vertices[0] = topCapStart + 2 * i;
			faces[nFaces].vertices[1] = topCapStart + 2 * i + 1;
			faces[nFaces++].vertices[2] = topCapStart + 2 * i + 3;
		}
	}
}

void cylinder::DrawShape(void)
{
	glCullFace(GL_FRONT);
	mesh::DrawShape();
	glCullFace(GL_BACK);
	mesh::DrawShape();
}

void cylinder::Read(Loader &input)
{
	char token[MAX_INPUT_LENGTH];

	bool moreTokens;
	do 
	{
		input.PeekTokens(token, 1);
		
		moreTokens = true;
		if (strcmp(token, "base") == 0)
		{
			input.ReadToken(token);
			for (int i = 0; i < 3; i++)
				if (!input.ReadFloat(base[i]))
					input.Error("Not enough parameters for cylinder's base in cylinder '%s'", objectName);
		}
		else if (strcmp(token, "radius") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadFloat(radius))
				input.Error("Error reading radius in cylinder '%s'", objectName);
		}
		else if (strcmp(token, "height") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadFloat(height))
				input.Error("Error reading height in cylinder '%s'", objectName);
		}
		else if (strcmp(token, "capped") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadBool(capped))
				input.Error("Error reading height in cylinder '%s'", objectName);
		}
		else if (strcmp(token, "axis") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadInt(axis))
				input.Error("Error reading axis number in cylinder '%s'", objectName);
		}
		else if (strcmp(token, "resolution") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadInt(resolution))
				input.Error("Error reading resolution in cylinder '%s'", objectName);
		}
		else 
			moreTokens = false; // If we don't recognize it jump out ... it is an attribute
	} while (moreTokens);

	SetupShape();
}

////////////////////////////////////////////////////////////////////////////
// Methods for the cone class
////////////////////////////////////////////////////////////////////////////

cone::~cone()
{
}

void cone::SetCone(point c, float r, float h, int res, int a, bool capped)
{
	base = c;
	radius = r;
	height = h;
	resolution = res;
	axis = a;
	SetupShape();
}
 
void cone::SetupShape(void)
{
	Erase(4 * (resolution + 1), 2 * resolution);  // The vertices need to be doubled for the caps, and the extra 2 sets are for the centers of the caps 
												  // which need to have many copies of the same vertex with different texture coordinates

	float thetaInc =  2 * PIf / resolution;
	float thetaLimit = 2 * PIf + thetaInc / 2.0f;
	int i;
	float theta;
	int bottomCapStart;

	nVerts = 0;

	vector a, v0, v1;

	if (axis == 0)
	{
		a = vector(1, 0, 0);
		v0 = vector(0, 1, 0);
		v1 = vector(0, 0, 1);
	}
	if (axis == 1)
	{
		a = vector(0, 1, 0);
		v0 = vector(0, 0, 1);
		v1 = vector(1, 0, 0);
	}
	else
	{
		a = vector(0, 0, 1);
		v0 = vector(1, 0, 0);
		v1 = vector(0, 1, 0);
	}

	// First we do the body of the cone
	for (i = 0, theta = 0.0f; i <= resolution; i++, theta += thetaInc)
	{
		float cTheta = cos(theta), sTheta = sin(theta);

		vertices[nVerts].position = base + radius * (cTheta * v0 + sTheta * v1);
		vertices[nVerts].normal = height * (cTheta * v0 + sTheta * v1) + radius * a;
		vertices[nVerts].uv = vector(theta / (2 * PIf), 0.0);
		vertices[nVerts].dPdu = radius * (-sTheta * v0 + cTheta * v1);
		vertices[nVerts++].dPdv = -radius * (cTheta * v0 + sTheta * v1) + height * a;

		vertices[nVerts].position = base + height * a;
		vertices[nVerts].normal = height * (cTheta * v0 + sTheta * v1) + radius * a;
		vertices[nVerts].uv = vector(theta / (2 * PIf), 0.0);
		vertices[nVerts].dPdu = radius * (-sTheta * v0 + cTheta * v1);
		vertices[nVerts++].dPdv = -radius * (cTheta * v0 + sTheta * v1) + height * a;
	}

	// Now we do the cap
	if (capped)
	{
		bottomCapStart = nVerts;
		for (i = 0, theta = 2 * PIf; i <= resolution; i++, theta -= thetaInc)  // negative direction to preserve right handedness on the bottom
		{
			float cTheta = cos(theta), sTheta = sin(theta);

			vertices[nVerts].position = base;
			vertices[nVerts].normal = -a;
			vertices[nVerts].uv = vector(theta / (2 * PIf), 1.0);
			vertices[nVerts].dPdu = vector(0, 0, 0);
			vertices[nVerts++].dPdv = radius * (cTheta * v0 + sTheta * v1);

			vertices[nVerts].position = base + radius * cTheta * v0 + radius * sTheta * v1;
			vertices[nVerts].normal = -a;
			vertices[nVerts].uv = vector(theta / (2 * PIf), 0.0);
			vertices[nVerts].dPdu = radius * (-sTheta * v0 + cTheta * v1);
			vertices[nVerts++].dPdv = radius * (cTheta * v0 + sTheta * v1);
		}
	}

	nFaces = 0;
	for (int i = 0; i < resolution; i++)
	{
		faces[nFaces].vertices[0] = 2 * i;
		faces[nFaces].vertices[1] = 2 * i + 1;
		faces[nFaces++].vertices[2] = 2 * i + 2;
	}

	if (capped)
	{
		for (int i = 0; i < resolution; i++)
		{
			faces[nFaces].vertices[0] = bottomCapStart + 2 * i;
			faces[nFaces].vertices[1] = bottomCapStart + 2 * i + 1;
			faces[nFaces++].vertices[2] = bottomCapStart + 2 * i + 3;
		}
	}
}

void cone::DrawShape(void)
{
	glCullFace(GL_FRONT);
	mesh::DrawShape();
	glCullFace(GL_BACK);
	mesh::DrawShape();
}

void cone::Read(Loader &input)
{
	char token[MAX_INPUT_LENGTH];

	bool moreTokens;
	do 
	{
		input.PeekTokens(token, 1);
		
		moreTokens = true;
		if (strcmp(token, "base") == 0)
		{
			input.ReadToken(token);
			for (int i = 0; i < 3; i++)
				if (!input.ReadFloat(base[i]))
					input.Error("Not enough parameters for cone's base in cone '%s'", objectName);
		}
		else if (strcmp(token, "radius") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadFloat(radius))
				input.Error("Error reading radius in cone '%s'", objectName);
		}
		else if (strcmp(token, "height") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadFloat(height))
				input.Error("Error reading height in cone '%s'", objectName);
		}
		else if (strcmp(token, "axis") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadInt(axis))
				input.Error("Error reading axis number in cone '%s'", objectName);
		}
		else if (strcmp(token, "capped") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadBool(capped))
				input.Error("Error reading height in cone '%s'", objectName);
		}
		else if (strcmp(token, "resolution") == 0)
		{
			input.ReadToken(token);
			if (!input.ReadInt(resolution))
				input.Error("Error reading resolution in cone '%s'", objectName);
		}
		else 
			moreTokens = false; // If we don't recognize it jump out ... it is an attribute
	} while (moreTokens);

	SetupShape();
}

