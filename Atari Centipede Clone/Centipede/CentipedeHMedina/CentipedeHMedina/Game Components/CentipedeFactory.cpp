#include "CentipedeFactory.h"
#include "Mushroomfield.h"
#include "RecyclingCHeadSegmentFactory.h"
#include "RecyclingCBodySegmentFactory.h"
#include "CentipedeHeadSegment.h"
#include "CentipedeBodySegment.h"

CentipedeFactory * CentipedeFactory::factoryInstance =NULL;

void CentipedeFactory::CreateCentipede(const sf::Vector2f StartLocation,const int Speed,const int Length, Mushroomfield& Field,const sf::Vector2f InitialDirection){
	//creates the head of the centipede
	int size =Length;
	int UpdateCounter =0;
	CentipedeSegment* head = &RecyclingCHeadSegmentFactory::CreateCentipedeHeadSegment( StartLocation,Speed,Field,InitialDirection );
	CentipedeSegment* iterator =head;
	
	//adds the bodies after the head, while linking the previous and next pointers properly
	for(int i =0;i<size-1;i++){
		iterator->Next=	&RecyclingCBodySegmentFactory::CreateCentipedeBodySegment(StartLocation-sf::Vector2f((CENTIPEDESEGMENT*(i+1))*InitialDirection.x,0),Speed,Field,InitialDirection);
		iterator->Next->Previous= iterator;
		iterator= iterator->Next;
	}

	//this block of code sets up the update order so that the head is updated last (fixes the jittering issue and the offset created on splitting)
	iterator =head;
	while(iterator!=NULL){
		
		iterator->SetUpdateOrder(size-UpdateCounter);
		iterator= iterator->Next;
		++UpdateCounter;
	}
}

void CentipedeFactory::RemoveCentipedeSegmentFromAliveList(CentipedeSegment& toBeDestroyed){

	bool HasBeenFound =false;
 	std::list<CentipedeSegment*>::iterator it=Instance().AliveSegments.begin();
	for(it=Instance().AliveSegments.begin();!HasBeenFound&&it!=Instance().AliveSegments.end();){ //while object to be destroyed is not found and not at end of list

		if(*it==&toBeDestroyed){//if the pointers are equal (point to same object)
			(*it)->MarkForDestroy();//destroy the object no need to delete becaue factory takes care of it
			it=Instance().AliveSegments.erase(it);//remove the spot from list
			HasBeenFound=true;
		}else{
			++it; //continue searching
		}

	}
}

void CentipedeFactory::AddAliveSegment(CentipedeSegment& toBeAdded){

	Instance().AliveSegments.push_back(&toBeAdded);
}

void CentipedeFactory::RecallAllSegments(){

	std::list<CentipedeSegment*>::iterator it=Instance().AliveSegments.begin();
	while(Instance().AliveSegments.size()>0){

		Instance().AliveSegments.front()->MarkForDestroy(); //destroy object.. sending it back to factory (no need to delete since factory owns them)
		

	}
	Instance().AliveSegments.clear();//clear the list (doesnt delete objects only the pointers to the objects, which is ok because factory owns all segments)
	
}

int CentipedeFactory::GetNumberOfAliveSegments(){

	return Instance().AliveSegments.size();
}