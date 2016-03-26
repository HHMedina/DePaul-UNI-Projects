#ifndef RECYCLINGBLASTERFACTORY_H
#define RECYCLINGBLASTERFACTORY_H

#include "TEAL\CommonElements.h"
#include <stack>

class Blaster;

class RecyclingBlasterFactory{

private:
	static RecyclingBlasterFactory * factoryInstance; //Factory set up as singleton
	RecyclingBlasterFactory(){}
	RecyclingBlasterFactory(RecyclingBlasterFactory&){} //prevent copy-construction
	RecyclingBlasterFactory& operator = (const RecyclingBlasterFactory&){} //prevent assignment
	~RecyclingBlasterFactory();
	
	static RecyclingBlasterFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingBlasterFactory();
		}
		return *factoryInstance;
	}

	static void RecycleBlaster(GameObject* b);

	//stack to hold Centipede Body segments
	std::stack<Blaster*> recycledItems;

public:
	//responsible for spawning Centipede Body Segments
	static Blaster& CreateBlaster(const int type);
	

};



#endif RECYCLINGBLASTERFACTORY_H