#ifndef CENTIPEDEFACTORY_H
#define CENTIPEDEFACTORY_H

#include "TEAL\CommonElements.h"

#include <list>

class Mushroomfield;
class CentipedeHeadSegment;
class CentipedeNodySegment;
class CentipedeSegment;
class CentipedeFactory{
public:

	static void CreateCentipede(const sf::Vector2f SpawnPosition,const int Speed, const int Length, Mushroomfield&,const sf::Vector2f InitialDirection);
	static void RemoveCentipedeSegmentFromAliveList(CentipedeSegment&);
	static void AddAliveSegment(CentipedeSegment&);
	static void RecallAllSegments();
	static int GetNumberOfAliveSegments();
private:
	static CentipedeFactory * factoryInstance; 
	CentipedeFactory(){};
	CentipedeFactory(const CentipedeFactory&){}
	CentipedeFactory& operator =(const CentipedeFactory&){}
	~CentipedeFactory(){};

	static CentipedeFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new CentipedeFactory();
		}
		return *factoryInstance;
	}

	std::list<CentipedeSegment*> AliveSegments;//list that holds the centipeds currently alive (in scene)

	static const int CENTIPEDESEGMENT = 16;
};

#endif CENTIPEDEFACTORY_H