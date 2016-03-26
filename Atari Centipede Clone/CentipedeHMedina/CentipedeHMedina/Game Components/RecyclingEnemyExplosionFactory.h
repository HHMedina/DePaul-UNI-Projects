#ifndef RECYCLINGENEMYEXPLOSIONFACTORY_H
#define RECYCLINGENEMYEXPLOSIONFACTORY_H

#include <stack>
#include "TEAL\CommonElements.h"
class EnemyExplosion;
class RecyclingEnemyExplosionFactory{


private:
	static RecyclingEnemyExplosionFactory * factoryInstance; //Factory set up as singleton
	RecyclingEnemyExplosionFactory(){} //prevent constructor
	RecyclingEnemyExplosionFactory(RecyclingEnemyExplosionFactory&){} //prevent copy-construction
	RecyclingEnemyExplosionFactory& operator = (const RecyclingEnemyExplosionFactory&){} //prevent assignment
	~RecyclingEnemyExplosionFactory();

	static RecyclingEnemyExplosionFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingEnemyExplosionFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled Enemy Explosions
	std::stack<EnemyExplosion*> recycledItems;
	

public:
	//responsible for spawning bullets
	static EnemyExplosion& CreateEnemyExplosion(const sf::Vector2f SpawnLocation);
	static void RecycleEnemyExplosion(GameObject* b);

};



#endif RECYCLINGENEMYEXPLOSIONFACTORY_H