#include "RecyclingScorpionFactory.h"
#include "Scorpion.h"
#include "Mushroomfield.h"

//declare static members used in .h
RecyclingScorpionFactory * RecyclingScorpionFactory::factoryInstance =NULL;

RecyclingScorpionFactory::~RecyclingScorpionFactory(){
	//deletes every Scorpion in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



Scorpion& RecyclingScorpionFactory::CreateScorpion(const sf::Vector2f SpawnLocation,const float direction,Mushroomfield& father){
	
	
	Scorpion* scorp;
		
	//if the recyclebin is empty create a Scorpion
	if ( Instance().recycledItems.empty() )
	{
	
		 
		scorp = new Scorpion(SpawnLocation,direction, father);
			
		// Tell Teal to let us manually manage the Scorpion and send to RecycleFlea()
		scorp->SetExternalManagement( RecycleScorpion);
	}
	else
	{
		//sets Scorpion to the top of the recycle stack (grab a previously used mushroom)
		scorp = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register Scorpion to scene
		scorp->RegisterToCurrentScene();  
	}
	//respawn the Scorpion
	scorp->Initialize(SpawnLocation,direction, father);
	

	return *scorp;
	
}

void RecyclingScorpionFactory::RecycleScorpion(GameObject* scorp)
{
	//adds Scorpion to recycle stack
	Instance().recycledItems.push( (Scorpion*) scorp);
	
}