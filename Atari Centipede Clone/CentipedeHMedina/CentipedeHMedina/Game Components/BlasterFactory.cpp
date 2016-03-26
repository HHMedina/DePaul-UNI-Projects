#include "BlasterFactory.h"
#include "Blaster.h"

//declare static members used in .h
RecyclingBlasterFactory * RecyclingBlasterFactory::factoryInstance =NULL;


RecyclingBlasterFactory::~RecyclingBlasterFactory(){
	//deletes every blaster in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



Blaster& RecyclingBlasterFactory::CreateBlaster(const int type){
	

	Blaster* shooter;
		
	//if the recyclebin is empty create a blaster
	if ( Instance().recycledItems.empty() )
	{
	
		 
		shooter = new Blaster(type);
			
		// Tell Teal to let us manually manage the Blaster and send to RecycleBlaster()
		shooter->SetExternalManagement( RecycleBlaster );
	}
	else
	{
		//sets blaster to the top of the recycle stack (grab a previously used blaster)
		shooter = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register blaster to scene
		shooter->RegisterToCurrentScene();  
	}
	//respawn the blaster
	shooter->Initialize(type);
	

	return *shooter;
	
}


void RecyclingBlasterFactory::RecycleBlaster(GameObject* shooter)
{
	//adds blaster to recycle stack
	Instance().recycledItems.push( (Blaster*) shooter);
	
}