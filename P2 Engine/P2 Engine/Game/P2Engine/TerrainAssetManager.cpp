#include "TerrainAssetManager.h"
#include "Terrain.h"
#include <assert.h>
TerrainAssetManager::TerrainAssetManager(){
	
}

TerrainAssetManager::TerrainAssetManager(const TerrainAssetManager&){}

const TerrainAssetManager& TerrainAssetManager::operator =(const TerrainAssetManager&){return *this;}

void TerrainAssetManager::LoadTerrain(const char* const fileName, const char* const assetName, const char* const TextureKey, float Sidelength = 1, float maxheight=1, float zeroHeight=0, int RepeatU =1, int RepeatV =1) const{
	
	if(terrainDataBaseMap.find(assetName)!=terrainDataBaseMap.end()){//already has a terrain then overwrite it (delete it)
		delete terrainDataBaseMap[assetName];
	}

	//Assign Terrain to AssetSpot
	terrainDataBaseMap[assetName] = new Terrain(fileName, assetName, Sidelength,maxheight,zeroHeight,TextureKey,RepeatU,RepeatV);

}

Terrain* TerrainAssetManager::GetTerrain(const char* const assetName) const{
	//check if terrain has been loaded
	assert(terrainDataBaseMap.find(assetName)!=terrainDataBaseMap.end()
		&& "TerrainManager::GetTerrain(): Terrain Not Found!");
	return terrainDataBaseMap[assetName];
}

