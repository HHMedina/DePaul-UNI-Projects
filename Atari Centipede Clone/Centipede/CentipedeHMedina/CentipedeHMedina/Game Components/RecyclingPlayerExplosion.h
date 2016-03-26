#ifndef RECYCLINGPLAYEREXPLOSIONFACTORY_H
#define RECYCLINGPLAYEREXPLOSIONFACTORY_H

#include <stack>
#include "TEAL\CommonElements.h"
class PlayerExplosion;
class RecyclingPlayerExplosionFactory{


private:
	static RecyclingPlayerExplosionFactory * factoryInstance; //Factory set up as singleton
	RecyclingPlayerExplosionFactory(){} //prevent constructor
	RecyclingPlayerExplosionFactory(const RecyclingPlayerExplosionFactory&){} //prevent copy-construction
	RecyclingPlayerExplosionFactory& operator = (const RecyclingPlayerExplosionFactory&){} //prevent assignment
	~RecyclingPlayerExplosionFactory();

	static RecyclingPlayerExplosionFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingPlayerExplosionFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled Enemy Explosions
	std::stack<PlayerExplosion*> recycledItems;
	

public:
	//responsible for spawning bullets
	static PlayerExplosion& CreatePlayerExplosion(const sf::Vector2f SpawnLocation);
	static void RecyclePlayerExplosion(GameObject* b);

};



#endif RECYCLINGPLAYEREXPLOSIONFACTORY_H