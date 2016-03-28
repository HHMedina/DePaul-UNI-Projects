#include <time.h>
#include "jmisc.h"
#include "scene.h"
#include "component.h"
#include "mesh.h"
#include "Application.h"
#include "Texture.h"

void scene::Load(char *newFilename)
{
	shape::ClearShapes();
	SetupDefaults();

	for (int i = 0; i < MAX_INCLUDE_DEPTH; i++)
		input[i].Close();

	strcpy(filename, newFilename);
	if (!input[0].Open(filename))
		complain("Couldn't open file '%s'\n", filename);
	curStream = 0;

	hasViewRef = false;
	hasFov = false;

	nLights = 0;
	NextFrame();
}

void scene::NextFrame(void)
{
	// We keep track of a light count for this frame so that we can
	// use the light commands to modify lights from previous frames
	int newNLights = 0;
	char token[MAX_INPUT_LENGTH];

	if (curStream < 0)
		return;

	do
	{
		input[curStream].EatWhitespace();
		if (input[curStream].eof())
		{
			input[curStream].Close();
			curStream--;
			if (curStream < 0)
			{
				jMessageBox("Last Frame of File %s", filename);
				return;
			}
		}

		int oldPos = input[curStream].CurPos();  // if it's a shape, we'll have to come back to this point
		input[curStream].ReadToken(token);
		if (strncmp(token, "cam", 3) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(Camera[i]))
					input[curStream].Error("Incomplete Camera Data");
			hasCamera = true;
		}
		else if (strncmp(token, "int", 3) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(Interest[i]))
					input[curStream].Error("Incomplete Interest Point Data");
			hasInterest = true;
		}
		else if (strncmp(token, "up", 2) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(Up[i]))
					input[curStream].Error("Incomplete Up Vector Data");
			hasUp = true;
		}
		else if (strncmp(token, "eye", 3) == 0)
		{
			if (!input[curStream].ReadFloat(Eye))
				input[curStream].Error("Incomplete Eye Data");
			hasEye = true;
		}
		else if (strncmp(token, "view", 4) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(ViewRef[i]))
					input[curStream].Error("Incomplete View Reference Point Data");
			hasViewRef = true;
		}
		else if (strncmp(token, "port", 4) == 0)
		{
			for (int i = 0; i <= 3; i++)
				if (!input[curStream].ReadFloat(Viewport[i]))
					input[curStream].Error("Incomplete Viewport Data");
			hasViewport = true;
		}
		else if (strncmp(token, "aspect", 6) == 0)
		{
			if (!input[curStream].ReadFloat(AspectRatio))
				input[curStream].Error("Incomplete Aspect Ratio Data");
			hasAspect = true;
		}
		else if (strncmp(token, "near", 4) == 0)
		{
			if (!input[curStream].ReadFloat(NearZ))
				input[curStream].Error("Incomplete Z-Near Data");
			hasNear = true;
		}
		else if (strncmp(token, "far", 3) == 0)
		{
			if (!input[curStream].ReadFloat(FarZ))
				input[curStream].Error("Incomplete Z-Far Data");
			hasFar = true;
		}
		else if (strncmp(token, "win", 3) == 0)
		{
			for (int i = 0; i <= 3; i++)
				if (!input[curStream].ReadFloat(Window[i]))
					input[curStream].Error("Incomplete Window Data");
			hasWindow = true;			
		}
		else if (strncmp(token, "fov", 3) == 0)
		{
			if (!input[curStream].ReadFloat(Fov))
				input[curStream].Error("Incomplete Field of View Data");
			hasFov = true;
		}
		else if (strncmp(token, "res", 3) == 0)
		{
			if (!input[curStream].ReadInt(Resolution))
				input[curStream].Error("Incomplete Resolution Data");
			input[curStream].EatNumbers(); // Sometimes the y-resolution follows ... ignore it
			hasResolution = true;
		}	
		else if (strncmp(token, "animationlen", 12) == 0)
		{
			float len;
			if (!input[curStream].ReadFloat(len))
				input[curStream].Error("Missing animation length following animationLength command\n");
			App->SetAnimationLength(len);
		}
		else if (strncmp(token, "framespersec", 12) == 0)
		{
			int count;
			if (!input[curStream].ReadInt(count))
				input[curStream].Error("Missing frame rate following framesPerSecond command\n");
			App->SetFramesPerSecond(count);
		}
		else if (strncmp(token, "lightp", 6) == 0)
		{
			if (newNLights >= 7)
				input[curStream].Warning(token, "Only 8 lights supported (0-7), the next light's data will overwrite light #7\n");

			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(lights[newNLights].position[i]))
					input[curStream].Error("Incomplete Light Position Data");

			if (!input[curStream].ReadFloat(lights[newNLights].position[3]))
				lights[newNLights].position[3] = 1.0;

			// The light position command starts a new light in the rgl file format
			// So we initialize the rest of the data for this light to some sensible defaults;
			if (newNLights < 7)
			{
				newNLights++;				
				if (newNLights > nLights)
					nLights = newNLights;

				lights[newNLights].attenuation[0] = 1.0;
				lights[newNLights].attenuation[1] = 0.0;
				lights[newNLights].attenuation[2] = 0.0;
				lights[newNLights].spotCutoff = 180;
				lights[newNLights].spotDirection[0] = 0.0;
				lights[newNLights].spotDirection[1] = 1.0;
				lights[newNLights].spotDirection[2] = 0.0;
				lights[newNLights].spotFalloff = 1;
				lights[newNLights].color = rgb::white;
				lights[newNLights].specularColor = rgb::white;
			}
		}
		else if (strncmp(token, "lightc", 6) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(lights[newNLights].color[i]))
					input[curStream].Error("Incomplete Light Color Data");

			if (!input[curStream].ReadFloat(lights[newNLights].color[3]))
				lights[newNLights].color[3] = 1.0;
		}
		else if (strncmp(token, "lightspec", 9) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(lights[newNLights].specularColor[i]))
					input[curStream].Error("Incomplete Light Specular Color Data");

			if (!input[curStream].ReadFloat(lights[newNLights].specularColor[3]))
				lights[newNLights].color[3] = 1.0;
		}
		else if (strncmp(token, "lightatten", 10) == 0)
		{
			if (!input[curStream].ReadFloat(lights[newNLights].attenuation[0]))
				input[curStream].Error("Incomplete Light Attenuation Data");
			if (!input[curStream].ReadFloat(lights[newNLights].attenuation[1]))
				lights[newNLights].attenuation[1] = 0.0;
			if (!input[curStream].ReadFloat(lights[newNLights].attenuation[2]))
				lights[newNLights].attenuation[2] = 0.0;
		}
		else if (strncmp(token, "spotdir", 7) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(lights[newNLights].spotDirection[i]))
					input[curStream].Error("Incomplete Spot Light Direction Data");
		}
		else if (strncmp(token, "spotcut", 7) == 0)
		{
			if (!input[curStream].ReadFloat(lights[newNLights].spotCutoff))
				input[curStream].Error("Incomplete Spot Light Cutoff Data");
		}
		else if (strncmp(token, "spotfalloff", 7) == 0)
		{
			if (!input[curStream].ReadFloat(lights[newNLights].spotFalloff))
				input[curStream].Error("Incomplete Spot Light Exponent Data");
		}
		else if ((strncmp(token, "lighta", 6) == 0) || 
			     (strncmp(token, "amb", 3) == 0) )
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(GlobalAmbient[i]))
					input[curStream].Error("Incomplete Ambient Light Data");
					
			if (!input[curStream].ReadFloat(GlobalAmbient[3]))
				GlobalAmbient[3] = 1.0;
		}
		else if (strncmp(token, "background", 10) == 0)
		{
			for (int i = 0; i < 3; i++)
				if (!input[curStream].ReadFloat(BackgroundColor[i]))
					input[curStream].Error("Incomplete Ambient Light Data");
					
			if (!input[curStream].ReadFloat(BackgroundColor[3]))
				BackgroundColor[3] = 1.0;
		}
		else if (strncmp(token, "incl", 4) == 0)
		{
			curStream++;
			if (curStream == MAX_INCLUDE_DEPTH)
				complain("Too many nested include files, max = %d", MAX_INCLUDE_DEPTH);
			else
			{
				input[curStream - 1].ReadToken(token);
				if (!input[curStream].Open(token))
					input[curStream - 1].Error(token, "Couldn't open include file: %s", token); 
			}
		}
		else if (strncmp(token, "draw", 4) == 0)
		{
			if (hasViewRef)
			{
				vector v = ViewRef - Interest;
				v.Normalize();
				NearZ = fabs(Eye);
				Camera = ViewRef + NearZ * v; 
				hasViewRef = false;
			}

			char shapeName[MAX_INPUT_LENGTH];
			input[curStream].ReadToken(shapeName);
			sceneShape = shape::FindShape(shapeName);
			if (sceneShape)
			{
				// If no lights were found, set up a default light
				if (nLights == 0)
				{
					lights[0].color = rgb::white;
					lights[0].position[0] = 50.0;
					lights[0].position[1] = 100.0;
					lights[0].position[2] = 50.0;
					lights[0].attenuation[0] = 1.0;					// No Attenuation
					lights[0].attenuation[1] = 0.0;	
					lights[0].attenuation[2] = 0.0;	
					lights[0].spotCutoff = 180;						// No spotlight cutoff
					nLights = 1;
				}
				SetupScene();
			}
			else
				input[curStream].Error(shapeName, "Couldn't find shape '%s' to draw", shapeName);
		}
		else if (strncmp(token, "save", 4) == 0)
		{
			char shapeName[MAX_INPUT_LENGTH], fileName[MAX_INPUT_LENGTH];
			input[curStream].ReadToken(shapeName);
			sceneShape = shape::FindShape(shapeName);
			if (sceneShape)
			{
				input[curStream].ReadToken(fileName);
				if (!isalnum(fileName[0]))
					input[curStream].Error(token, "Couldn't find name of file to save scene '%s'", shapeName);
				else
					SetupScene(false, fileName);
			}
			else
				input[curStream].Error(shapeName, "Couldn't find shape '%s' to draw", shapeName);
		}
		else // It must be a shape 
		{
			input[curStream].MoveTo(oldPos);				// Rewind to before word
			shape *s = shape::LoadShape(input[curStream], this);	// If it's not any of the above
															// It must be a shape
			if (s == NULL)
				input[curStream].Error(token, "Unknown command: %s", token);	
		}
	} while (strcmp(token, "draw") != 0);	
}

