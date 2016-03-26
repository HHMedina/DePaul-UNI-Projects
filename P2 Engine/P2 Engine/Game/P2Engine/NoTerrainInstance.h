#ifndef NOTERRAININSTANCE_H
#define NOTERRAININSTANCE_H
#include "Terrain.h"
#include <list>
class NoTerrainInstance:public Terrain{

	friend class TerrainManager;

protected:
	
	NoTerrainInstance(){
		vertsArray =NULL;
		minMaxData =NULL;
	}
	NoTerrainInstance(const NoTerrainInstance&){}
	const NoTerrainInstance& operator =(const NoTerrainInstance&){}
	virtual ~NoTerrainInstance(){}


	virtual void Draw() override{}
	virtual Vect GetCellIndexFromWorldPosition(const Vect&){return Vect(0,0,0);}
	virtual void GetLikelyCandidates(Collidable*,list<Terrain::MaxAndMinPoints>::iterator& startOut,list<Terrain::MaxAndMinPoints>::iterator& endOut) override{startOut = emptyList.begin(); endOut = emptyList.end();}

	std::list<MaxAndMinPoints> emptyList;
};
#endif