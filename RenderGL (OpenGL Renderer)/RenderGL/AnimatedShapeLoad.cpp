
#include "AnimatedShape.h"

Interpolator *Interpolator::LoadInterpolator(Loader &input)
{
	char type[256];
	Interpolator *result = NULL;

	input.ReadToken(type);
	if (strcmp(type, "linear") == 0)
		result = new LinearInterp;
	else 
		input.Error(type, "Unknown interpolator type");

	result->Load(input);
	return result;
}

void Interpolator::Load(Loader &input)
{
	char token[256];

	if (!input.ReadInt(nKeys))
		input.Error("Interpolator object must begin with a key count");

	input.ReadToken(token);
	for (int i = 0; i < nKeys; i++)
	{
		if (strcmp(token, "key") != 0)
			input.Error("Each key must begin with the keyword 'key'");

		if (!input.ReadFloat(keys[i].t))
			input.Error("Key %d must begin with a time", i);

		keys[i].vOut = keys[i].vOut = vector(0, 0, 0);
		keys[i].tcb = vector(0, 0, 0);
		keys[i].constant = false;
		keys[i].speedIn = 0.0;
		keys[i].speedOut = 0.0;

		do 
		{
			input.ReadToken(token);

			if (strncmp(token, "val", 3) == 0 || strncmp(token, "pos", 3) == 0)
			{
				for (int j = 0; j < 3; j++)
					if (!input.ReadFloat(keys[i].p[j]))
						input.Error("Not enough coordinates for key %d", i);
				
				// See if there is a homogeneous coordinate, if not, then 1.0
				if (!input.ReadFloat(keys[i].p[3]))
					keys[i].p[3] = 1.0;
		
				keys[i].p.Normalize(); // Make its homogeneous coordinate 1.0
			}
			else if (strcmp(token, "constant") == 0)
				keys[i].constant = true;
			else if (strcmp(token, "easein") == 0)
			{
				if (!input.ReadFloat(keys[i].speedIn))
					input.Error("The value for a key's ease in must be a floating point value (preferrably between -1 and 1");
			}
			else if (strcmp(token, "easeout") == 0)
			{
				if (!input.ReadFloat(keys[i].speedOut))
					input.Error("The value for a key's ease out must be a floating point value (preferrably between -1 and 1");
			}
			else if (strcmp(token, "key") == 0)
			{
				continue;
			}
			else
			{
				// Try to see if the derived class recognizes the 
				// key's attribute.
				if (!ReadKeyAttribute(input, i, token))
				{
					if (i == nKeys - 1)
						break; // Go on to process attributes
					else
						input.Error(token, "Unknown token '%s' for Interpolator key %d", token, i);
				}
			}
		} while (strcmp(token, "key") != 0 && strcmp(token, "end") != 0);

		if (strcmp(token, "end") == 0 && i != nKeys - 1)
			input.Error("End reached before all keys were found");
	}

	if (strcmp(token, "end") != 0)
	{
		do
		{
			if (!ReadAttribute(input, token))
				input.Error(token, "Invalid attribute '%s' for interpolator object", token);
			input.ReadToken(token);
		} while (strcmp(token, "end") != 0);
	}

	RebuildParamDistanceTable();
}

void animatedShape::Read(Loader &input)
{
	char token[256], name[256];

	s = NULL;
	do
	{
		input.PeekTokens(token, 1);

		if (strcmp(token, "object") == 0)
		{
			input.ReadToken(token);

			if (s != NULL)
				input.Error("Only one shape allowed in an AnimatedShape object");

			input.ReadToken(name);
			s = FindShape(name);
			if (s == NULL)
				input.Error("Couldn't find shape '%s' referenced in AnimatedShape '%s'", name, objectName);
		}
		else if (strncmp(token, "pos", 3) == 0)
		{
			input.ReadToken(token);
			iPosition = Interpolator::LoadInterpolator(input);
			if (iPosition->NKeys() == 0)
			{
				delete iPosition;
				iPosition = NULL;
			}
		}
		else if (strncmp(token, "sc", 2) == 0)
		{
			input.ReadToken(token);
			iScale = Interpolator::LoadInterpolator(input);
			if (iScale->NKeys() == 0)
			{
				delete iScale;
				iScale = NULL;
			}
		}
		else if (strncmp(token, "rot", 3) == 0)
		{
			input.ReadToken(token);
			iRotation = Interpolator::LoadInterpolator(input);
			if (iRotation->NKeys() == 0)
			{
				delete iRotation;
				iRotation = NULL;
			}
		}
		else if (strcmp(token, "end") == 0)
			break;
		else
			input.Error(token, "Unknown command '%s' in AnimatedShape '%s'", token, objectName);
	} while (true);
}
