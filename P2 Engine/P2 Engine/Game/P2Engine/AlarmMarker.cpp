#include "AlarmMarker.h"
#include "Alarmable.h"
#include "TimeManager.h"
AlarmMarker::AlarmMarker(Alarmable* ptr, AlarmID id, float t){
	alarmID = id;
	alarmable = ptr;
	alarmable->timeTrigger[alarmID] = t + TimeManager::GetTotalGameTime();
}

Alarmable* AlarmMarker::GetAlarmable(){
	return alarmable;
}

AlarmID AlarmMarker::GetAlarmID(){
	return alarmID;
}

void AlarmMarker::ExecuteAlarm(){
	alarmable->timeTrigger[alarmID] =-1;//mark as finished
	alarmable->Dispatch(alarmID);
}

AlarmMarker::~AlarmMarker(){

}