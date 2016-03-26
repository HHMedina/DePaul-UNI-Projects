#ifndef P2ENGINE_H
#define P2ENGINE_H




//P2 Engine

//base
#include <Azul.h>

//managers
#include "AssetManager.h"
#include "InputManager.h"
#include "DrawableManager.h"
#include "AlarmManager.h"
#include "TerminableManager.h"
#include "TerminationDumpster.h"
#include "UpdatableManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "CollisionManager.h"
#include "GraphicsObjectMaker.h"

//Manager subsystems
#include "KeyEventEnum.h"
#include "KeyState.h"
#include "AlarmMarker.h"
#include "CollidableGroup.h"
#include "CollidableGroupBase.h"

//Able Types
#include "Alarmable.h"
#include "Collidable.h"
#include "Drawable.h"
#include "Updatable.h"
#include "Inputable.h"
#include "Terminable.h"

#include "GameObject.h"
#endif // !P2ENGINE_H
