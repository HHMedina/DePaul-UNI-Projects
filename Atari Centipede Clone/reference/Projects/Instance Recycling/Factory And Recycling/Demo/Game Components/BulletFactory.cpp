// BulletFactory
// Andre Berthiaume, April 2013

#include "TEAL\CommonElements.h"
#include "BulletFactory.h"
#include "Bullet.h"

BulletFactory::BulletFactory()
{
}

BulletFactory::~BulletFactory()
{
}

void BulletFactory::CreateBullet( sf::Vector2f pos)
{
	Bullet* b;

	b = new Bullet();
	b->Initialize(pos);
}
