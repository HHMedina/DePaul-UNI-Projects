#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <list>
#include "AlarmMarker.h"
using namespace std;

class Alarmable;
class AlarmMarker;

class AlarmManager{

	friend class Scene;
public:

	AlarmManager(){}
	AlarmManager( const AlarmManager & rhs){ rhs;}
	AlarmManager& operator = (const AlarmManager&){return *this;}
	~AlarmManager();


	/**
	\brief Register an alarm in an Alarmable for a given time.

	This function creates an AlarmMarker with the passed in alarmable,
	which alarm was set, and the time the alarm is set to go off. If an
	AlarmMarker is already set, then the function will override it.

	\param registrant registrant is the alarmable that will have an alarm set.
	\param id id is the number id of the alarm that will be set.
	\param time time is the amount of time the alarm should be set to.
	*/
	void Register(Alarmable* registrant,AlarmID id,float time);
	
	/**
	\brief Deregister an alarm in an Alarmable.
	
	This function removes an alarm in an Alarmable from consideration
	in the AlarmManager. Essentially this cancels an alarm from going
	off.

	\param ptr ptr is the alarmable that will have an alarm deregistered.
	\param id id is the number of the alarm that will be deregistered.
	*/
	void Deregister(Alarmable* ptr,AlarmID id);

	/**
	\brief This method adds time to an alarm.

	\param ptr ptr is the alarmable that will have its alarm altered.
	\param id id is the number of the alarm that will have time added.
	\param timeToAdd is the time to be added to the alarm.
	*/
	void AddTimeToAlarm(Alarmable* ptr,AlarmID id,float timeToAdd);

	/**
	\brief This method subtracts time from an alarm.

	\param ptr ptr is the alarmable that will have its alarm altered.
	\param id id is the number of the alarm that will have time subtracted.
	\param timeToSubtract is the time to be subtracted from the alarm.
	*/
	void SubtractTimeFromAlarm(Alarmable* ptr,AlarmID id,float timeToSubtract);

	
private:
	
	/**
	\brief Checks for alarms to be triggered at each frame.
	
	This function checks to see if the current time surpasses AlarmMarkers
	by checking the timeLineMap.If the top (the most AlarmMarker with the most
	immediate time) is more than the current time, its Alarmable's reference
	will have its callback called and is then removed from the list. This process
	will continue until it finds an AlarmMarker that is in the future.
	

	*/
	void UpDateAlarms();
	
	/**
	\brief multimap that stores AlarmMarkers in ascending time trigger time.
	*/
	multimap<float,AlarmMarker*> timeLineMap;
};

#endif