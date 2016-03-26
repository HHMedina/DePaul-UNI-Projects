#ifndef RECYCLINGSCOREEXPLOSIONFACTORY_H
#define RECYCLINGSCOREEXPLOSIONFACTORY_H

#include <stack>
#include "TEAL\CommonElements.h"
class ScoreExplosion;
class RecyclingScoreExplosionFactory{


private:
	static RecyclingScoreExplosionFactory * factoryInstance; //Factory set up as singleton
	RecyclingScoreExplosionFactory(){} //prevent constructor
	RecyclingScoreExplosionFactory(RecyclingScoreExplosionFactory&){} //prevent copy-construction
	RecyclingScoreExplosionFactory& operator = (const RecyclingScoreExplosionFactory&){} //prevent assignment
	~RecyclingScoreExplosionFactory();

	static RecyclingScoreExplosionFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingScoreExplosionFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled Enemy Explosions
	std::stack<ScoreExplosion*> recycledItems;
	

public:
	//responsible for spawning Enemy Explosions
	static ScoreExplosion& CreateScoreExplosion(const int WhichScore,const sf::Vector2f SpawnLocation);
	static void RecycleScoreExplosion(GameObject* b);

};



#endif RECYCLINGSCOREEXPLOSIONFACTORY_H