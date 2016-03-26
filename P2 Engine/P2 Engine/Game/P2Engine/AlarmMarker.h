#ifndef ALARMMARKER_H
#define ALARMMARKER_H

#include <map>
using namespace std;

class Alarmable;
enum AlarmID;
class AlarmMarker{

	//Only Allow  direct Alarm Marker Access to Alarm Manager since 
	//its the only one who should deal with Markers 
	friend class AlarmManager;

private:

	AlarmMarker(Alarmable*,AlarmID,float);
	AlarmMarker( const AlarmMarker & rhs){ rhs;}
	AlarmMarker& operator = (const AlarmMarker& rhs){ rhs;}
	~AlarmMarker();	

	/**
	\brief Returns a reference to the Alarmable stored in the AlarmMarker (the Alkarmable
	that corresponds to this alarm marker and its trigger time.)
	*/
	Alarmable* GetAlarmable();

	/**
	\brief Returns the AlarmID associated with this AlarmMarker.
	*/
	AlarmID GetAlarmID();

	/**
	\brief Calls Dispatch() on the stored Alarmable and passed in the AlarmMarker's corresponding ID
		essentially, calling the associated Alarmable's callback function
		for the registered Alarm.
	*/
	void ExecuteAlarm();


	/**
	\brief Reference to the Alarmable that has registered an alarm.
	*/
	Alarmable * alarmable;

	/**
	\brief The ID of the alarm that has been registered for the stored Alarmable.
	*/
	AlarmID alarmID;
};
#endif