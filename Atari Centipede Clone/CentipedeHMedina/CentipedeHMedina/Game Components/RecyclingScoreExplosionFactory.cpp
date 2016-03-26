#include "RecyclingScoreExplosionFactory.h"
#include "ScoreExplosion.h"


//declare static members used in .h
RecyclingScoreExplosionFactory * RecyclingScoreExplosionFactory::factoryInstance =NULL;

RecyclingScoreExplosionFactory::~RecyclingScoreExplosionFactory(){
	//deletes every EnemyExplosion in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



ScoreExplosion& RecyclingScoreExplosionFactory::CreateScoreExplosion(const int WhichScore,const sf::Vector2f SpawnLocation){
	
	
	ScoreExplosion* explosion;
		
	//if the recyclebin is empty create an EnemyExplosion
	if ( Instance().recycledItems.empty() )
	{
	
		 
		explosion = new ScoreExplosion(WhichScore,SpawnLocation);
			
		// Tell Teal to let us manually manage the EnemyExplosion and send to RecycleEnemyExplosion()
		explosion->SetExternalManagement( RecycleScoreExplosion);
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
	explosion->Initialize(WhichScore,SpawnLocation);
	

	return *explosion;
	
}

void RecyclingScoreExplosionFactory::RecycleScoreExplosion(GameObject* explosion)
{
	//adds EnemyExplosion to recycle stack
	Instance().recycledItems.push( (ScoreExplosion*) explosion);
	
}