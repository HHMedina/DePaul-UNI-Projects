#ifndef RECYCLINGSCORPIONFACTORY_H
#define RECYCLINGSCORPIONFACTORY_H

#include <stack>

#include "TEAL\CommonElements.h"
using namespace std;

class Scorpion;
class Mushroomfield;

class RecyclingScorpionFactory{

	friend class Mushroomfield;

private:
	static RecyclingScorpionFactory * factoryInstance; //Factory set up as singleton
	RecyclingScorpionFactory(){} //prevent constructor
	RecyclingScorpionFactory(RecyclingScorpionFactory&){} //prevent copy-construction
	RecyclingScorpionFactory& operator = (const RecyclingScorpionFactory&){} //prevent assignment
	~RecyclingScorpionFactory();

	static RecyclingScorpionFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingScorpionFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled Flea depository
	stack<Scorpion*> recycledItems;
	

public:
	//responsible for spawning bullets
	static Scorpion& CreateScorpion(const sf::Vector2f SpawnLocation,const float direction,Mushroomfield& father);
	static void RecycleScorpion(GameObject* b);

};



#endif RECYCLINGSCORPIONFACTORY_H