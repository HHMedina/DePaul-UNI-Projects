#ifndef RECYCLINGCBODYSEGMENTFACTORY_H
#define RECYCLINGCBODYSEGMENTFACTORY_H

#include <stack>

#include "TEAL\CommonElements.h"
using namespace std;

//class CentipedeBodySegment;
class Mushroomfield;

class RecyclingCBodySegmentFactory{

	friend class Centipede;
	friend class CentipedeBodySegment;
public:
	RecyclingCBodySegmentFactory(){}
	~RecyclingCBodySegmentFactory();
	//responsible for spawning Centipede Body Segments
	static CentipedeBodySegment& CreateCentipedeBodySegment(sf::Vector2f WorldPosition,const int Speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection);
	static void RecycleCentipedeBodySegment(GameObject* b);
private:
	//stack to hold Centipede Body segments
	static stack<CentipedeBodySegment*> recycledItems;
	

};



#endif RECYCLINGCBODYSEGMENTFACTORY_H