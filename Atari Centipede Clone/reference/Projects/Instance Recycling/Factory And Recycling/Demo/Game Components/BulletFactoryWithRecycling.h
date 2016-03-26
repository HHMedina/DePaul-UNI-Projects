// BulletFactoryWithRecycling
// Andre Berthiaume, April 2013

#ifndef _BulletFactoryWithRecycling
#define _BulletFactoryWithRecycling

#include <stack>
#include "Bullet.h"

class BulletFactoryWithRecycling
{
private:
	static std::stack<Bullet*> recycledItems;

public:
	BulletFactoryWithRecycling();
	~BulletFactoryWithRecycling();


	static void CreateBullet(sf::Vector2f pos);

	static void RecycleBullet(GameObject* b);  // Arguments must be GameObject since it will be called by the base class
};


#endif _BulletFactoryWithRecycling