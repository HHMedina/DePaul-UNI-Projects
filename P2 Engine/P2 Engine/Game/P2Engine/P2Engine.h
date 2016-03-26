#ifndef P2ENGINE_H
#define P2ENGINE_H




//P2 Engine

//base
#include <Azul.h>

//managers
#include "SoundSystem.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "Sound.h"
#include "DrawableManager.h"
#include "AlarmManager.h"
#include "TerminableManager.h"
#include "TerminationDumpster.h"
#include "UpdatableManager.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include "CollisionManager.h"
#include "GraphicsObjectMaker.h"
#include "TerrainAssetManager.h"
#include "TerrainManager.h"

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

//object types
#include "GameObject.h"
#include "Terrain.h"

#include "P2Math.h"

//collision types
#include "CollisionVolume.h"
#include "BoundingSphereVolume.h"
#include "AABBBoundingBox.h"

//debugger
#include "DebugVisualizer.h"


//for aound debug
#include "irrKlang.h"

#endif // !P2ENGINE_H
