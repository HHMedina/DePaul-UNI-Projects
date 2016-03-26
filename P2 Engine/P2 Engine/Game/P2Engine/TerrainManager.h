#ifndef TERRAINMANAGER_H
#define TERRAINMANAGER_H

class Terrain;

class TerrainManager{

	friend class Scene;
	friend class SceneManager;
private:

	TerrainManager();
	TerrainManager(const TerrainManager&);
	const TerrainManager& operator =(const TerrainManager&);
	~TerrainManager();
	//Set Current Terrain
	void SetCurrentTerrain(const char* const assetName);

	//Get Current Terrain
	Terrain* GetCurrentTerrain() const;

	//Function to draw current scene
	void DrawCurrentTerrain();

	Terrain* currentTerrain;
	Terrain* myNoTerrainInstance;
};
#endif
/*
void TerrainAssetManager::SetCurrentTerrain(const char* const assetName){
	//check if terrain has been loaded
	assert(terrainDataBaseMap.find(assetName)!=terrainDataBaseMap.end()
		&& "TerrainManager::GetTerrain(): Terrain Not Found!");
	currentTerrain = terrainDataBaseMap[assetName];
}

const Terrain* const TerrainAssetManager::GetCurrentTerrain() const{
	return currentTerrain;
}
*/