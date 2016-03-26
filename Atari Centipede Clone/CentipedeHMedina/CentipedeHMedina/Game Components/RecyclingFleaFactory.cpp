#include "RecyclingFleaFactory.h"
#include "Flea.h"
#include "Mushroomfield.h"

//declare static members used in .h
RecyclingFleaFactory * RecyclingFleaFactory::factoryInstance =NULL;

RecyclingFleaFactory::~RecyclingFleaFactory(){
	//deletes every Flea in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



Flea& RecyclingFleaFactory::CreateFlea(const sf::Vector2f SpawnPosition,const float speed,Mushroomfield& FatherMushroomField){
	
	
	Flea* flea;
		
	//if the recyclebin is empty create a flea
	if ( Instance().recycledItems.empty() )
	{
	
		 
		flea = new Flea(SpawnPosition,speed, FatherMushroomField);
			
		// Tell Teal to let us manually manage the flea and send to RecycleFlea()
		flea->SetExternalManagement( RecycleFlea);
	}
	else
	{
		//sets flea to the top of the recycle stack (grab a previously used Flea)
		flea = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register flea to scene
		flea->RegisterToCurrentScene();  
	}
	//respawn the flea
	flea->Initialize(SpawnPosition,speed, FatherMushroomField);
	

	return *flea;
	
}

void RecyclingFleaFactory::RecycleFlea(GameObject* flea)
{
	//adds flea to recycle stack
	Instance().recycledItems.push( (Flea*) flea);
	
}