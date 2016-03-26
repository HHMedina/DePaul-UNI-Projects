
#include "RecyclingSpiderFactory.h"
#include "Spider.h"
#include "Mushroomfield.h"
#include "Blaster.h"

//declare static members used in .h
RecyclingSpiderFactory * RecyclingSpiderFactory::factoryInstance =NULL;

RecyclingSpiderFactory::~RecyclingSpiderFactory(){
	//deletes every Flea in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



Spider& RecyclingSpiderFactory::CreateSpider(const sf::Vector2f SpawnPosition,const float speed,const sf::Vector2i InitialDirection, Mushroomfield& FatherMushroomField){
	
	
	Spider* spider;
		
	//if the recyclebin is empty create a Spider
	if ( Instance().recycledItems.empty() )
	{
	
		 
		spider = new Spider(SpawnPosition,speed,InitialDirection, FatherMushroomField);
			
		// Tell Teal to let us manually manage the Spider and send to Recycle method
		spider->SetExternalManagement( RecycleSpider);
	}
	else
	{
		//sets spider to the top of the recycle stack (grab a previously used spider)
		spider = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register flea to scene
		spider->RegisterToCurrentScene();  
	}
	//respawn the flea
	spider->Initialize(SpawnPosition,speed,InitialDirection, FatherMushroomField);
	

	return *spider;
	
}

void RecyclingSpiderFactory::RecycleSpider(GameObject* spider)
{
	//adds flea to recycle stack
	Instance().recycledItems.push( (Spider*) spider);
	
}
