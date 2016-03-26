#include "TerrainManager.h"
#include <assert.h>
#include "AssetManager.h"
#include "Terrain.h"
#include "NoTerrainInstance.h"

TerrainManager::TerrainManager(){
	myNoTerrainInstance = new NoTerrainInstance();
	currentTerrain=myNoTerrainInstance;
}

TerrainManager::~TerrainManager(){
	delete myNoTerrainInstance;
}

void TerrainManager::SetCurrentTerrain(const char* const assetName){
		
	currentTerrain = AssetManager::GetTerrain(assetName);
}

Terrain* TerrainManager::GetCurrentTerrain() const{
	return currentTerrain;
}

void TerrainManager::DrawCurrentTerrain(){
	currentTerrain->Draw();
}