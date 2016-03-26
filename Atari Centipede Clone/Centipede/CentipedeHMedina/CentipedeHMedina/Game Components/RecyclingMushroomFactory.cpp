#include "RecyclingMushroomFactory.h"
#include "Mushroom.h"
#include "MushroomField.h"

//define static members used in .h
RecyclingMushroomFactory * RecyclingMushroomFactory::factoryInstance =NULL;


RecyclingMushroomFactory::~RecyclingMushroomFactory(){
	//deletes every mushroom in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



Mushroom& RecyclingMushroomFactory::CreateMushroom(sf::Vector2f WorldPosition,sf::Vector2i GridPosition, Mushroomfield& MushField){
	//checks to see if there are less than the maximum amount of bullets in existance
	
	Mushroom* mush;
		
	//if the recyclebin is empty create a bullet
	if ( Instance().recycledItems.empty() )
	{
	
		 
		mush = new Mushroom(WorldPosition.x,WorldPosition.y, GridPosition, MushField);
			
		// Tell Teal to let us manually manage the mushroom and send to RecycleMushroom()
		mush->SetExternalManagement( RecycleMushroom );
	}
	else
	{
		//sets mush to the top of the recycle stack (grab a previously used mushroom)
		mush = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register mush to scene
		mush->RegisterToCurrentScene();  
	}
	//respawn the mushroom
	mush->Initialize(WorldPosition,GridPosition,MushField);
	

	return *mush;
	
}



void RecyclingMushroomFactory::RecycleMushroom(GameObject* mushroom)
{
	//adds bullet to recycle stack
	Instance().recycledItems.push( (Mushroom*) mushroom);
	
}