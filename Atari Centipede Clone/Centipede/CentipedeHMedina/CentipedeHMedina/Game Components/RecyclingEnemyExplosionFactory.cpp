#include "RecyclingEnemyExplosionFactory.h"
#include "EnemyExplosion.h"


//declare static members used in .h
RecyclingEnemyExplosionFactory * RecyclingEnemyExplosionFactory::factoryInstance =NULL;

RecyclingEnemyExplosionFactory::~RecyclingEnemyExplosionFactory(){
	//deletes every EnemyExplosion in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}

}



EnemyExplosion& RecyclingEnemyExplosionFactory::CreateEnemyExplosion(const sf::Vector2f SpawnLocation){
	
	
	EnemyExplosion* explosion;
		
	//if the recyclebin is empty create an EnemyExplosion
	if ( Instance().recycledItems.empty() )
	{
	
		 
		explosion = new EnemyExplosion(SpawnLocation);
			
		// Tell Teal to let us manually manage the EnemyExplosion and send to RecycleEnemyExplosion()
		explosion->SetExternalManagement( RecycleEnemyExplosion);
	}
	else
	{
		//sets EnemyExplosion to the top of the recycle stack (grab a previously used EnemyExplosion)
		explosion = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register EnemyExplosion to scene
		explosion->RegisterToCurrentScene();  
	}
	//respawn the EnemyExplosion
	explosion->Initialize(SpawnLocation);
	

	return *explosion;
	
}

void RecyclingEnemyExplosionFactory::RecycleEnemyExplosion(GameObject* explosion)
{
	//adds EnemyExplosion to recycle stack
	Instance().recycledItems.push( (EnemyExplosion*) explosion);
	
}