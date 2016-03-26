#ifndef RECYCLINGSPIDERFACTORY_H
#define RECYCLINGSPIDERFACTORY_H

#include <stack>

#include "TEAL\CommonElements.h"


class Spider;
class Mushroomfield;
class Blaster;

class RecyclingSpiderFactory{

	friend class Mushroomfield;

private:
	static RecyclingSpiderFactory * factoryInstance; //Factory set up as singleton
	RecyclingSpiderFactory(){} //prevent constructor
	RecyclingSpiderFactory(RecyclingSpiderFactory&){} //prevent copy-construction
	RecyclingSpiderFactory& operator = (const RecyclingSpiderFactory&){} //prevent assignment
	~RecyclingSpiderFactory();

	static RecyclingSpiderFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingSpiderFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled Flea depository
	std::stack<Spider*> recycledItems;
	static void RecycleSpider(GameObject* b);

public:
	//responsible for spawning bullets
	static Spider& CreateSpider(const sf::Vector2f SpawnPosition,const float speed,sf::Vector2i InitialDirection,Mushroomfield&);
	

};



#endif RECYCLINGSPIDERFACTORY_H
