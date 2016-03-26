#ifndef RECYCLINGFLEAFACTORY_H
#define RECYCLINGFLEAFACTORY_H

#include <stack>

#include "TEAL\CommonElements.h"
using namespace std;

class Flea;
class Mushroomfield;

class RecyclingFleaFactory{

	friend class Mushroomfield;

private:
	static RecyclingFleaFactory * factoryInstance; //Factory set up as singleton
	RecyclingFleaFactory(){} //prevent constructor
	RecyclingFleaFactory(RecyclingFleaFactory&){} //prevent copy-construction
	RecyclingFleaFactory& operator = (const RecyclingFleaFactory&){} //prevent assignment
	~RecyclingFleaFactory();

	static RecyclingFleaFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingFleaFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled Flea depository
	stack<Flea*> recycledItems;
	

public:
	//responsible for spawning bullets
	static Flea& CreateFlea(const sf::Vector2f SpawnPosition,const float speed,Mushroomfield&);
	static void RecycleFlea(GameObject* b);

};



#endif RECYCLINGFLEAFACTORY_H