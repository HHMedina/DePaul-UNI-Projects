// BulletFactory
// Andre Berthiaume, April 2013

#ifndef _BulletFactory
#define _BulletFactory

#include <stack>

class BulletFactory
{
public:
	BulletFactory();
	~BulletFactory();

	static void CreateBullet(sf::Vector2f pos);
};


#endif _BulletFactory