#include "AlarmManager.h"
#include "Alarmable.h"
#include <iostream>
#include "TimeManager.h"

AlarmManager::~AlarmManager(){

	//clean up alarm marker
	for(map<float,AlarmMarker*>::iterator it = timeLineMap.begin();it!=timeLineMap.end();){

		delete it->second;
		it = timeLineMap.erase(it);

	}

	timeLineMap.clear();
}

void AlarmManager::Register(Alarmable*  registrant, AlarmID id, float time){
	
	Deregister(registrant,id);//remove a previous registrant if already there
	AlarmMarker* temp = new AlarmMarker(registrant,id,time ); // will be inserted regardless of condition
	
	//will replace alarm marker if already registered with new time
	//If there was not an alarm marker already registered, then it will register it
	timeLineMap.insert(std::pair<float,AlarmMarker*>(time + TimeManager::GetTotalGameTime(),temp));
}

void AlarmManager::Deregister(Alarmable* ptr,AlarmID id){

	bool found=false;
	multimap<float,AlarmMarker*>::iterator itLower,itUpper,it;
	itLower = timeLineMap.lower_bound(ptr->timeTrigger[id]);
	itUpper = timeLineMap.upper_bound(ptr->timeTrigger[id]);

	
	//search through all elements with the same time as ptr's alarm's time
	for(it=itLower;it!= timeLineMap.end() && it!=itUpper&&!found;){//search all markers with same time as alarmable
		if(it->second->GetAlarmable() == ptr&& it->second->GetAlarmID() == id){//if it is the target Alarmable, and matches the alarm id
			it->second->alarmable->timeTrigger[id] = -1;//mark as inactive
			delete it->second;
			it = timeLineMap.erase(it);
			found =true; // end search
		}else{
			++it;
		}
	}

	if(!found){
		//cout<<"ALARM MANAGER: \""<<ptr<<"\" has not been found in registry with id of \""<<id<<"\"."<<endl;
	}

}

void AlarmManager::UpDateAlarms(){

	multimap<float,AlarmMarker*>::iterator it= timeLineMap.begin();
	while(it!=timeLineMap.end() && it->first < TimeManager::GetTotalGameTime()){ //while first element's time is in the past
		it->second->ExecuteAlarm(); //execute callback
		delete it->second; //release memory
		it = timeLineMap.erase(it);//remove from timeline and validate pointer
		
	}

}

void AlarmManager::AddTimeToAlarm(Alarmable* ptr,AlarmID id,float timeToAdd){
	//adding time in this case means removing the old time and reregistering into the timeline with the new time.
	float timeUntilAlarm =0;
	if(ptr->timeTrigger[id]!=-1){//if the alarm is active
		timeUntilAlarm = ptr->timeTrigger[id]-TimeManager::GetTotalGameTime();//returns time until alarm goes off
		//timeUntilAlarm+timeToAdd because register is registering from current time.
		Deregister(ptr,id);
		Register(ptr,id,timeUntilAlarm+timeToAdd);
	}
	
}

void AlarmManager::SubtractTimeFromAlarm(Alarmable* ptr,AlarmID id,float timeToSubtract){
	//subtractinging time in this case means removing the old time and reregistering into the timeline with the new time.
	float timeUntilAlarm =0;
	if(ptr->timeTrigger[id]!=-1){//if the alarm is active
		timeUntilAlarm = ptr->timeTrigger[id]-TimeManager::GetTotalGameTime();//returns time until alarm goes off
		Deregister(ptr,id);
		Register(ptr,id,timeUntilAlarm-timeToSubtract);//timeUntilAlarm+timeToAdd because register is registering from current time.
	}
}
