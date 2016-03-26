#ifndef RECYCLINGCHEADSEGMENTFACTORY_H
#define RECYCLINGCHEADSEGMENTFACTORY_H

#include <stack>

#include "TEAL\CommonElements.h"
using namespace std;

class Mushroomfield;

class RecyclingCHeadSegmentFactory{

	friend class Centipede;
	friend class CentipedeHeadSegment;
private:
	static RecyclingCHeadSegmentFactory * factoryInstance; //Factory set up as singleton
	RecyclingCHeadSegmentFactory(){}
	RecyclingCHeadSegmentFactory(RecyclingCHeadSegmentFactory&){} //prevent copy-construction
	RecyclingCHeadSegmentFactory& operator = (const RecyclingCHeadSegmentFactory&){} //prevent assignment
	~RecyclingCHeadSegmentFactory();
	
	static RecyclingCHeadSegmentFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingCHeadSegmentFactory();
		}
		return *factoryInstance;
	}

	static void RecycleCentipedeHeadSegment(GameObject* b);

	//stack to hold Centipede Head segments
	stack<CentipedeHeadSegment*> recycledItems;

public:
	//responsible for spawning Centipede Head Segments
	static CentipedeHeadSegment& CreateCentipedeHeadSegment(const sf::Vector2f WorldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection);
	

};



#endif RECYCLINGCHEADSEGMENTFACTORY_H