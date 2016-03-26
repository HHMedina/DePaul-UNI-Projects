#ifndef TERRAINASSETMANAGER_H
#define TERRAINASSETMANAGER_H

#include <map>

//class forward declarations
class Terrain;

using namespace std;//For Strings

class TerrainAssetManager {

	friend class AssetManager;

	static struct comparer
	{
		public:
		bool operator()(const std::string x, const std::string y)
		{
			return x.compare(y)>0;
		}
	};

	
	TerrainAssetManager();
	TerrainAssetManager(const TerrainAssetManager&);
	const TerrainAssetManager& operator =(const TerrainAssetManager&);

	//Load Terrain Method
	void LoadTerrain(const char* const, const char* const,const char* const, float, float, float, int, int) const;

	//Get Terrain Pointer
	Terrain* GetTerrain(const char* const) const;
		
	//Used To Store All Instances of Terrains
	mutable map<string,Terrain*, comparer> terrainDataBaseMap;
};
#endif