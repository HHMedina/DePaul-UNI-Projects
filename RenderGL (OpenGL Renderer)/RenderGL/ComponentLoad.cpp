#include <math.h>
#include <iostream>
using namespace std;

#include "shape.h"
#include "component.h"
#include <ctype.h>

void instance::ReadObject(Loader &input)
{
	char tmpName[MAX_NAME_LENGTH];

	input.ReadToken(tmpName);

	if (!shape::FindShape(tmpName))
		input.Error(tmpName, "Couldn't find shape %s for instance in component", tmpName);

	SetMaster(tmpName);  // This set's the object name to "masterName"_InstanceCount

	input.EatNumbers();  // Sometimes the master's object count follows ... ignore it
	ReadAttributes(input);
}

void component::Read(Loader &input)
{
	int nComponents;
	instance *inst;
	double count = 0;

	Erase();

	if (!input.ReadInt(nComponents))
		input.Error("Component command must be followed by a component count");

	char type[MAX_INPUT_LENGTH];
	for (int i = 0; i < nComponents; i++)
	{
		int oldPos = input.CurPos();
		input.ReadToken(type);
		if (strcmp(type, "object") == 0)
		{
			inst = new instance;
			inst->ReadObject(input);
			AddInstance(inst);
		}
		else
		{
			input.MoveTo(oldPos);
			shape *s = shape::LoadShape(input);
			if (s != NULL)
			{
				inst = new instance;
				inst->SetMaster(s->ObjectName());
				AddInstance(inst);
			}
			else
				input.Error(type, "Attributes started before all components were found: %s", type);
		}
	}
}

