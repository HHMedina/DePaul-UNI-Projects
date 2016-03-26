#include "RecyclingPlayerExplosion.h"
#include "PlayerExplosion.h"


//declare static members used in .h
RecyclingPlayerExplosionFactory * RecyclingPlayerExplosionFactory::factoryInstance =NULL;

RecyclingPlayerExplosionFactory::~RecyclingPlayerExplosionFactory(){
	//deletes every PlayerExplosion in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}

}



PlayerExplosion& RecyclingPlayerExplosionFactory::CreatePlayerExplosion(const sf::Vector2f SpawnLocation){
	
	
	PlayerExplosion* explosion;
		
	//if the recyclebin is empty create a PlayerExplosion
	if ( Instance().recycledItems.empty() )
	{
	
		 
		explosion = new PlayerExplosion(SpawnLocation);
			
		// Tell Teal to let us manually manage the PlayerExplosion and send to RecyclePlayerExplosion()
		explosion->SetExternalManagement( RecyclePlayerExplosion);
	}
	else
	{
		//sets PlayerExplosion to the top of the recycle stack (grab a previously used PlayerExplosion)
		explosion = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register PlayerExplosion to scene
		explosion->RegisterToCurrentScene();  
	}
	//respawn the EnemyExplosion
	explosion->Initialize(SpawnLocation);
	

	return *explosion;
	
}

void RecyclingPlayerExplosionFactory::RecyclePlayerExplosion(GameObject* explosion)
{
	//adds EnemyExplosion to recycle stack
	Instance().recycledItems.push( (PlayerExplosion*) explosion);
	
}