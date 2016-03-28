#include <time.h>	
#include <math.h>
#include <iostream>
using namespace std;
#include <stdlib.h>
#include "shape.h"
#include "mesh.h"
#include "point.h"
#include "vector.h"
#include "miscio.h"

// There are two distinct formats for storing a mesh in the dat file format.
//
// First:	Direct polygon definition
//			Here, imediately after the name of the mesh follows a polygon 
//			and a list of "face" commands.  Each face is simply a vertex  followed
//			by a list of vertices and each vertex is a list of coordinates.
//
// Example:	mesh floor 1
//			face 4
//				0.0 0.0 0.0
//				1.0 0.0 0.0
//				1.0 1.0 0.0
//				0.0 1.0 0.0
//			end
//
// To work properly, the vertices should be in er clockwise order to create a 
// proper "outward pointing normal".  As the loader processes each face, it checks
// for vertices that are shared with any other face already defined in the mesh.
//
// This format is the simplest for defining small meshes, but for very large meshes 
// with > 1000 faces, definitions are ineficient and processing takes way too long ... why?
//
// Second:	Vertex Lists
//			Here, the name of the mesh is followed by a vertices command which begins
//			a list of vertex data (position, texture coordinates, color, normal, etc.)
//			Each vertex begins with a "vert" command and is followed by one or more 
//			attribute commands for each vertex.  The current possible attributes are
//
//				coordinate - 3d or 4d coordinates of each vertex -		abbrev: coord
//				texture - 1d, 2d or 3d texture coordinates -			abbrev: tex
//			
//			One or more of these may not have a complete implementation and so may not
//			Work.  coordinate is guaranteed to work.
// 
//			The vertex list ends with the "EndVertices" command.
//
//			After the vertex list comes the face list.  The face list begins with a 
//			"Faces" command which specifies the number of faces to follow.  Each
//			face begins with a face command which specifies the number of vertices
//			in the face.  This is followed by a sequence of attribute commands similar
//			to the vertex data.  The most important of these is the "index" command which
//			lists the indices (in the vertex list) of the vertices in the polygon.  Again
//			they should be in erclockwise order to define an "outward pointing normal"
//

static const int MAX_OLD_MESH_SIZE = 20000;
 
void mesh::ReadFace(Loader &input)
{
	int n;

	if (!input.ReadInt(n))
		input.Error("Syntax error in face command.  # Vertices not found.");

	point *coords = new point[n];
	for (int i = 0; i < n; i++)
	{
		ResizeFaceList();
		for (int j = 0; j < 3; j++)
			if (!input.ReadFloat(coords[i][j]))
				input.Error("Syntax error in face command.  Too few coordinates for Vertex #%d.", nFaces + 1);
	}
	AddFace(coords, n);
}

void mesh::Read(Loader &input)
{
	char token[MAX_INPUT_LENGTH];
	int nmeshs;

	normalsSpecifiedInFile = false;
	if (!input.ReadInt(nmeshs))			
		ReadIndexed(input);
	else
	{
		Erase(MAX_OLD_MESH_SIZE, nmeshs);		// We have currently hardwired in a limit of MAX_OLD_MESH_SIZE vertices
												// for the old format.  Any larger mesh must be in the "new" format
		for (int i = 0; i < nmeshs; i++)
		{
			input.ReadToken(token);

			if (strncmp(token, "separate", 8) == 0)
			{
				separateVertices = true;
				input.ReadToken(token);
			}

			if (strcmp(token, "face") == 0)
				ReadFace(input);
			else 
				input.Error("Insufficient number of faces or objects for mesh: %s\nEncountered '%s' before all faces were complete", objectName, token); 
		}

		point size = Size();
		point min, max;
		MinMax(min, max);

		int smallestCoord = 0;
		double smallestValue = size[0];
		for (int i = 0; i < 3; i++)
		{
			if (size[i] < smallestValue)
			{
				smallestCoord = i;
				smallestValue = size[i];
			}
		}

		for (int i = 0; i < nVerts; i++)
		{
			int index = (smallestCoord + 1) % 3;
			vertices[i].uv[0] = (vertices[i].position[index] - min[index]) / size[index];
			index = (index + 1) % 3;
			vertices[i].uv[1] = (vertices[i].position[index] - min[index]) / size[index];;
		}
	}

	do
	{
		input.PeekTokens(token, 1);
		if (strcmp(token, "breakangle") == 0)
		{
			// Ignore this ... old command
		}
		if (strcmp(token, "end") != 0)
			input.Error("Unknown option for mesh %s", objectName);
	} while (strcmp(token, "end") != 0);

	RecalculateNormals();
}

void mesh::ReadIndexed(Loader &input)
{
	char token[MAX_INPUT_LENGTH];

	input.ReadToken(token);
	if (strncmp(token, "vertices", 4) != 0)
		input.Error(token, "An indexed mesh must begin with a Vertices command");
	
	if (!input.ReadInt(nVerts))
		input.Error("Vertices command must be followed by a vertex count");

	delete [] vertices;
	vertices = new vert[nVerts];

	if (nVerts > 0)
	{
		input.ReadToken(token);
		if (strncmp(token, "vert", 4) != 0)
			input.Error("A vertex must begin with a 'vert' command");
	}
	else 
		input.ReadToken(token);

	for (int i = 0; i < nVerts; i++)
	{
		vertices[i].position = point(0, 0, 0);
		vertices[i].uv = vector(9.9999e25f, 0, 0);
		vertices[i].ClearFaceList();

		do
		{
			input.ReadToken(token);
			if (strncmp(token, "coord", 5) == 0)
			{
				int j;
				for (j = 0; j < 3; j++)
					if (!input.ReadFloat(vertices[i].position[j]))
						input.Error("Incomplete coordinate data for vertex %d, must have at least 3", i);

				if (!input.ReadFloat(vertices[i].position[3]))
					vertices[i].position[j] = 1.0;
			}
			else if (strncmp(token, "uv", 3) == 0)
			{
				vertices[i].uv = vector(0, 0, 0);
				for (int j = 0; j < 2; j++)
					if (!input.ReadFloat(vertices[i].uv[j]))
						input.Error("Incomplete uv data for vertex %d, must have 2", i);
				uvsSpecifiedInFile = true;
			}
			else if (strncmp(token, "norm", 4) == 0)
			{
				vertices[i].normal = vector(0, 0, 0);
				for (int j = 0; j < 3; j++)
					if (!input.ReadFloat(vertices[i].normal[j]))
						input.Error("Incomplete normal data for vertex %d, must have 3", i);
				normalsSpecifiedInFile = true;
			}
			else if (strncmp(token, "dpdu", 4) == 0)
			{
				vertices[i].dPdu = vector(0, 0, 0);
				for (int j = 0; j < 3; j++)
					if (!input.ReadFloat(vertices[i].dPdu[j]))
						input.Error("Incomplete dpdu data for vertex %d, must have 3", i);
				vertices[i].dPdu[3] = 0.0;

				derivativesSpecifiedInFile = true;
			}
			else if (strncmp(token, "dpdv", 4) == 0)
			{
				vertices[i].dPdv = vector(0, 0, 0);
				for (int j = 0; j < 3; j++)
					if (!input.ReadFloat(vertices[i].dPdv[j]))
						input.Error("Incomplete dpdv data for vertex %d, must have 3", i);
				vertices[i].dPdu[3] = 0.0;

				derivativesSpecifiedInFile = true;
			}
			else if (strncmp(token, "endvertices", 6) == 0) 
			{
				if (i != nVerts - 1)
					complain("Vertex list ended before %d vertices were found", nVerts);
				else
					break; // Continue to process the faces
			}
			else if (strncmp(token, "vert", 4) != 0)
				input.Error(token, "Unknown Vertex Command Encountered: %s", token);
		} while(strncmp(token, "vert", 4) != 0);  // If you hit a vert command, go on to next vertex

		if (vertices[i].uv[0] == 9.9999e25f)
			vertices[i].uv = vertices[i].position - point::origin;
	}

	input.ReadToken(token);
	if (strncmp(token, "faces", 5) != 0)
		input.Error(token, "Vertex list must be followed by face list");

	int faceCount;
	if (!input.ReadInt(faceCount))
		input.Error("Faces command must be followed by a face count");

	delete [] faces;
	delete [] faceNormals;
	delete [] faceDpDu;
	delete [] faceDpDv;
	faces = new face[faceCount];
	faceNormals = new vector[faceCount];
	faceDpDu = new vector[faceCount];
	faceDpDv = new vector[faceCount];
	faceSize = faceCount;

	if (nFaces > 0)
	{
		if (!input.ReadToken(token))
			input.Error("A face must begin with a 'face' command");
	}
	else 
		input.ReadToken(token);

	for (int i = 0; i < faceCount; i++)
	{
		int vertCount;
		if (!input.ReadInt(vertCount))
			input.Error("face command must be followed by a vertex count");

		int *indices = new int[vertCount];
		do
		{
			input.ReadToken(token);
			if (strncmp(token, "index", 5) == 0)
			{
				for (int j = 0; j < vertCount; j++)
				{
					if (!input.ReadInt(indices[j]))
						input.Error("Incomplete vertex data for face %d, must have %d", i, vertCount);
					else if (indices[j] >= nVerts)
						input.Error("Badly formatted data: vertex index %d in mesh %s\n", indices[j], objectName);
				}
				AddFace(indices, vertCount);
			}
			else if (strncmp(token, "face", 4) == 0)
			{
				continue;
			}
			else if (strncmp(token, "endfaces", 4) == 0)
			{
				if (i != faceCount - 1)
					input.Error(token, "End reached before %d faces were found", nFaces);
				else
					break; // Continue to process the faces
			}
		} while(strncmp(token, "face", 4) != 0);  // If hit a new face command, then continue on to next face
	}
}


