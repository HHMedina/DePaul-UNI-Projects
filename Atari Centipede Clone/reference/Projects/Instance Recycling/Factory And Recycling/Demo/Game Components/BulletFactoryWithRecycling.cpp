// BulletFactoryWithRecycling
// Andre Berthiaume, April 2013

#include "TEAL\CommonElements.h"
#include "BulletFactoryWithRecycling.h"
#include "Bullet.h"

std::stack<Bullet*> BulletFactoryWithRecycling::recycledItems;

BulletFactoryWithRecycling::BulletFactoryWithRecycling()
{
}

void BulletFactoryWithRecycling::CreateBullet(sf::Vector2f pos)
{
	Bullet* b;

	if ( recycledItems.empty() )
	{
		ConsoleMsg::WriteLine("New Bullet"); // For illustration purposes

		b = new Bullet();

		// Declares that this object should be returned here (rather than deleted) when destroyed
		b->SetExternalManagement( RecycleBullet );
	}
	else
	{
		ConsoleMsg::WriteLine("Recycled Bullet"); // For illustration purposes

		b = recycledItems.top();
		recycledItems.pop();	// Remember: top doesn't pop and pop returns void...

		// Tell the object to register itself to the scene
		b->RegisterToCurrentScene();  
	}

	b->Initialize(pos);
}

void BulletFactoryWithRecycling::RecycleBullet(GameObject* b)
{
	recycledItems.push( (Bullet*) b);

	// For illustration purposes
	ConsoleMsg::WriteLine("Recycled Stack Size: " + Tools::ToString( recycledItems.size() ) );
}


// Why is this necessary?
BulletFactoryWithRecycling::~BulletFactoryWithRecycling()
{
	// forcefully delele all recycled gameobjects
	while ( ! recycledItems.empty() ) 
	{
		delete recycledItems.top();
		recycledItems.pop();
	}
}