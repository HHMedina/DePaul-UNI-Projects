#ifndef RECYCLINGBOMBFACTORY_H
#define RECYCLINGBOMBFACTORY_H

#include <P2Engine.h>
#include <stack>
class Bomb;
class RecyclingBombFactory{


private:
	static RecyclingBombFactory * factoryInstance; //Factory set up as singleton
	RecyclingBombFactory(){} //prevent constructor
	RecyclingBombFactory(RecyclingBombFactory&){} //prevent copy-construction
	RecyclingBombFactory& operator = (const RecyclingBombFactory&){} //prevent assignment
	~RecyclingBombFactory();

	static RecyclingBombFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingBombFactory();
		}
		return *factoryInstance;
	}


	//stack to hold Recycled bombs
	std::stack<Bomb*> recycledItems;

	//stack to hold active bombs (used for memory clean up)
	std::list<Bomb*> activeInScene;

public:
	//responsible for spawning bullets
	static Bomb& CreateBomb(float,float,float);
	static void RecycleBomb(Terminable* b);
	static void DeleteMe();
};



#endif RECYCLINGENEMYEXPLOSIONFACTORY_H