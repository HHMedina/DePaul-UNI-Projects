#include "RecyclingCBodySegmentFactory.h"
#include "CentipedeBodySegment.h"
#include "Mushroomfield.h"
#include "CentipedeFactory.h"

//declare static members used in .h
std::stack<CentipedeBodySegment*> RecyclingCBodySegmentFactory::recycledItems;


RecyclingCBodySegmentFactory::~RecyclingCBodySegmentFactory(){
	//deletes every centipede body segment in the stack
	while(!recycledItems.empty()){
		delete recycledItems.top();
		recycledItems.pop();
	}
	
}



CentipedeBodySegment& RecyclingCBodySegmentFactory::CreateCentipedeBodySegment(sf::Vector2f WorldPosition,const int Speed,Mushroomfield& FatherField,const sf::Vector2f InitialDirection){
	
	
	CentipedeBodySegment* cBody;
		
	//if the recyclebin is empty create a centipede body segment
	if ( recycledItems.empty() )
	{
	
		cBody = new CentipedeBodySegment(WorldPosition,Speed,FatherField,InitialDirection);
			
		// Tell Teal to let us manually manage the centipede body segment and send to Recycle method
		cBody->SetExternalManagement( RecycleCentipedeBodySegment );
	}
	else
	{
		//sets cbody to the top of the recycle stack (grab a previously used centipede body segment)
		cBody = recycledItems.top();
		recycledItems.pop();//remove from stack (since it will be used)

		// register centipede body segment to scene
		cBody->RegisterToCurrentScene();  
	}
	//respawn the centipede body segment
	cBody->Initialize(WorldPosition,Speed,FatherField,InitialDirection);

	//register the cbody into alive segments
	CentipedeFactory::AddAliveSegment(*cBody);
	return *cBody;
	
}

void RecyclingCBodySegmentFactory::RecycleCentipedeBodySegment(GameObject* cBody)
{
	//adds centipede body segment to recycle stack
	recycledItems.push( (CentipedeBodySegment*) cBody);
	
}