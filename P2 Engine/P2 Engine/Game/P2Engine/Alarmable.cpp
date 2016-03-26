#include "Alarmable.h"
#include "SceneManager.h"
#include "TimeManager.h"
#include <iostream>

void Alarmable::SetAlarm(AlarmID id,float time){
	//SceneManager::DeregisterForAlarm(this,id);//remove previous non activated alarm (if any) for this with alarmID id
	SceneManager::RegisterForAlarm(this,id,time);//Register new alarm marker
}
/*
void Alarmable::PauseAlarm(){

}
*/
float Alarmable::GetTimeRemainingOfAlarm(AlarmID id){
	if(timeTrigger[id] == -1){
		return -1; // inactive
	}else{
		return timeTrigger[id] - TimeManager::GetTotalGameTime();
	}
}

void Alarmable::CancelAlarm(AlarmID id){
	
	SceneManager::DeregisterForAlarm(this,id);
}

void Alarmable::Dispatch(AlarmID id){

	switch(id)
	{
		case AlarmID_0:
			Alarm0();
			break;
		case AlarmID_1:
			Alarm1();
			break;
		case AlarmID_2:
			Alarm2();
			break;
		default:
			cout<<"ALARMABLE: NOT AN ALARMID !!";

	}
}


void Alarmable::AddTimeToAlarm(AlarmID id,float timeToAdd){
	SceneManager::AddTimeToAlarm(this,id,timeToAdd);
}

void Alarmable::SubtractTimeFromAlarm(AlarmID id,float timeToSubtract){
	SceneManager::SubtractTimeFromAlarm(this,id,timeToSubtract);
}