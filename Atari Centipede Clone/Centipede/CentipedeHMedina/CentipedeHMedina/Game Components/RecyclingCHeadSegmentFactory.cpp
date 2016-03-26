#include "RecyclingCHeadSegmentFactory.h"
#include "CentipedeHeadSegment.h"
#include "Mushroomfield.h"
#include "CentipedeFactory.h"
//declare static members used in .h
RecyclingCHeadSegmentFactory * RecyclingCHeadSegmentFactory::factoryInstance =NULL;

RecyclingCHeadSegmentFactory::~RecyclingCHeadSegmentFactory(){
	//deletes every bullet in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



CentipedeHeadSegment& RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment(const sf::Vector2f WorldPosition,const int speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection){
	
	
	CentipedeHeadSegment* cHead;
		
	//if the recyclebin is empty create a new head segment
	if ( Instance().recycledItems.empty() )
	{
	
		cHead = new CentipedeHeadSegment(WorldPosition,speed,FatherField,InitialDirection);
			
		// Tell Teal to let us manually manage the mushroom and send to RecycleMushroom()
		cHead->SetExternalManagement( RecycleCentipedeHeadSegment );
	}
	else
	{
		//sets mush to the top of the recycle stack (grab a previously used cHead)
		cHead = Instance().recycledItems.top();
		Instance().recycledItems.pop();//remove from stack (since it will be used)

		// register chead to scene
		cHead->RegisterToCurrentScene();  
	}
	//respawn the chead
	cHead->Initialize(WorldPosition,speed,FatherField,InitialDirection);
	
	CentipedeFactory::AddAliveSegment(*cHead);

	return *cHead;
	
}

void RecyclingCHeadSegmentFactory::RecycleCentipedeHeadSegment(GameObject* cHead)
{
	//adds bullet to recycle stack
	Instance().recycledItems.push( (CentipedeHeadSegment*) cHead);
	
}