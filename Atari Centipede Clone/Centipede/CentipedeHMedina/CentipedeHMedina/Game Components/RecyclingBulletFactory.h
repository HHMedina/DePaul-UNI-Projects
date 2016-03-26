#ifndef RECYCLINGBULLETFACTORY_H
#define RECYCLINGBULLETFACTORY_H

#include <stack>

#include "TEAL\CommonElements.h"
using namespace std;

class BlasterBullet;
class Blaster;

class RecyclingBulletFactory{

	friend class BlasterBullet;
	friend class Playercontroller;
private:
	static RecyclingBulletFactory * factoryInstance;//set up singleton
	RecyclingBulletFactory(){}//prevent constructor
	RecyclingBulletFactory(RecyclingBulletFactory&){}//prevent copy constructor
	RecyclingBulletFactory& operator = (const RecyclingBulletFactory&){}//prevent assignment operator
	~RecyclingBulletFactory();

	static RecyclingBulletFactory& Instance()
	{
		if(!factoryInstance){
			factoryInstance = new RecyclingBulletFactory();
		}
		return *factoryInstance;
	}

	stack<BlasterBullet*> recycledItems;
	static void RecycleBullet(GameObject* b);
public:
	//responsible for spawning bullets
	static BlasterBullet& CreateBullet(const sf::Vector2f pos,Blaster&);
	
};



#endif RECYCLINGBULLETFACTORY_H