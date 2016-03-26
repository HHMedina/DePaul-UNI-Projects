#ifndef TERRAIN_H
#define TERRAIN_H
#include "Drawable.h"
#include "Azul.h"
#include "P2Math.h"
using namespace std;//for strings

class Collidable;

class Terrain {

	//friends
	friend class TerrainAssetManager;
	friend class TerrainManager;

	template <class T>
	friend class CollisionTerrainProcessor;
public:

	struct MaxAndMinPoints
	{
		MaxAndMinPoints(){

		}
		
		MaxAndMinPoints(const MaxAndMinPoints& rhs){
			this->MinPoint = rhs.MinPoint;
			this->MaxPoint = rhs.MaxPoint;
		}

		const MaxAndMinPoints &operator=(const MaxAndMinPoints &rhs){
			this->MaxPoint = rhs.MaxPoint;
			this->MinPoint = rhs.MinPoint;
			return *this;
		}

		MaxAndMinPoints(const Vect& min,const Vect& max){
			MinPoint =min;
			MaxPoint = max;
		}
		~MaxAndMinPoints(){}
		Vect MaxPoint;
		Vect MinPoint;
	};
	Terrain( const char* const heightmapFile, const char* const assetName, const float Sidelength, const float maxheight, const float zeroHeight, const char* const TextureKey, const int RepeatU, const int RepeatV);
	Terrain(){}
	
	virtual Vect GetCellIndexFromWorldPosition(const Vect& pos);
protected:

	
	Terrain(const Terrain& );
	const Terrain& operator =(const Terrain&);
	virtual ~Terrain();	

	virtual void Draw();
	void CreateTerrainModel( const char* const heightmapFile , const char* const assetName , const float Sidelength,const float zeroHeight,const float maxheight, const int RepeatU, const int RepeatV);
	void SaveTerrainModel( const char* const assetName, VertexStride_VUN* pVerts, int num_verts, TriangleIndex* tlist, int num_tri);

	//Vect GetIndexFromWorldPosition(const Vect& pos);

	virtual void GetLikelyCandidates(Collidable*,list<MaxAndMinPoints>::iterator& startOut,list<MaxAndMinPoints>::iterator& endOut);

	bool IsWithinTerrain(const Vect&);

	int imgWidth, imgHeigth,numVerts,numTris,numCells,numVertsPerSide,numCellsPerSide;
	float cellDimension,sideLength;
		
	VertexStride_VUN* vertsArray;	//Dynamic array holding the vertices
	MaxAndMinPoints* minMaxData;	//Dynamic array holding the cells/minMax data

	GraphicsObject* terrainGrphObject;

	list<MaxAndMinPoints> likeyCandidates;

};

#endif