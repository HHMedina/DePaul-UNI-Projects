#include "Scene.h"
#include "Alarmable.h"
#include "UpdatableManager.h"
#include "InputManager.h"
#include "DrawableManager.h"
#include "AlarmManager.h"
#include "CollisionManager.h"
#include <TerminableManager.h>
#include "TerminationDumpster.h"
Scene::Scene(){
	
}

Scene::~Scene(){
	//no need to deallocate (New was never used). Managers will automatially be cleaned

}

void Scene::DrawScene(){
	terrainManager.DrawCurrentTerrain();
	drawableManager.DrawAll();
}

void Scene::UpdateScene(){

	//update all scene elements
	inputManager.Update();
	alarmManager.UpDateAlarms();
	updatableManager.UpdateAll();
	collisionManager.ProcessCollisions();
	terminableManager.processTerminations();
}

void Scene::SetCurrentTerrain(const char* const assetName){
	terrainManager.SetCurrentTerrain(assetName);
}

TerrainManager& Scene::GetTerrainManager(){
	return terrainManager;
}

TerminableManager& Scene::GetTerminableManager(){

	return terminableManager;
	
}

TerminationDumpster& Scene::GetTerminationDumpster(){
	return terminationDumpster;
}

AlarmManager& Scene::GetAlarmManager(){
	return alarmManager;
}

InputManager& Scene::GetInputManager(){
	return inputManager;
}

DrawableManager& Scene::GetDrawableManager(){
	return drawableManager;
}

UpdatableManager& Scene::GetUpdatableManager(){
	return updatableManager;
}

CollisionManager& Scene::GetCollisionManager(){
	return collisionManager;
}

/*
TerrainManager& Scene::GetTerrainManager(){
	return terrainManager;
}
*/