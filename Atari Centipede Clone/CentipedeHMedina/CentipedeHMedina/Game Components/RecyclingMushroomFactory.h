#ifndef RECYCLINGMUSHROOMFACTORY_H
#define RECYCLINGMUSHROOMFACTORY_H

#include <stack>

#include "TEAL\CommonElements.h"
using namespace std;

class Mushroom;
class Mushroomfield;

class RecyclingMushroomFactory{

	friend class Mushroomfield;
private:
	static RecyclingMushroomFactory * factoryInstance; //Factory set up as singleton
	RecyclingMushroomFactory(){}//prevent constructor
	RecyclingMushroomFactory(const RecyclingMushroomFactory&){} //prevent copy-construction
	RecyclingMushroomFactory& operator = (const RecyclingMushroomFactory&){} //prevent assignment
	~RecyclingMushroomFactory();
	
	static RecyclingMushroomFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingMushroomFactory();
		}
		return *factoryInstance;
	}



	//stack to hold bullets
	stack<Mushroom*> recycledItems;
	static void RecycleMushroom(GameObject* b);

public:
	//responsible for spawning bullets
	static Mushroom& CreateMushroom(sf::Vector2f WorldPosition,sf::Vector2i GridPosition,Mushroomfield& MushField);
};



#endif RECYCLINGMUSHROOMFACTORY_H