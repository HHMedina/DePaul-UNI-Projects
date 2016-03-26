#ifndef ALARMABLE_H
#define ALARMABLE_H

#include <map>
using namespace std;

class TimeManager;
class SceneManager;

	enum AlarmID{
		AlarmID_0,
		AlarmID_1,
		AlarmID_2
	};


class Alarmable{

	//used to allow access to private
	//Timetrigger field member while
	//disallowing access via alarmable
	//through setter and getter methods.
	friend class AlarmMarker;
	friend class AlarmManager;

public:
		
	virtual ~Alarmable(){};
		


	/**
	\ingroup ALARM
	\brief This method sets an Alarm in the referenced Alarmable.
	
	This method sets an alarm for the alarmable by specifiying which alarm to set and how many seconds to set it to.
	If an alarm is already active, setting it again will override the previous alarm.

	\param id id is an AlarmID enum that specifies which alarm to set. 
	\param time time is a float which represents the time in seconds the alarm should go off from when it was called.

	\par Example:

	The following is an example of a bomb setting its alarm 0 upon creation.

	\code

	Bomb::Bomb(){
		
		//setting up initial properties like visual repsentation and collision omitted..

		SetAlarm(AlarmID_0,5); // sets alarm0 to 5 seconds from creation

	}

	void Bomb::Alarm0(){ // will be called after 5 seconds
		
		//Do stuff when alarm 0 is activated (in this case 5 seconds)

	}

	//void Bomb::OnTerminate() omitted
	//void Bomb::Draw() omitted..

	\endcode

	*/
	void SetAlarm(AlarmID id,float time);
	
	/**
	\ingroup ALARM
	\brief This method returns the amount of time left on an Alarm in the Alarmable.
	
	This function will return the amount of time left on an alarm. If the Alarmable is currently inactive,
	the function will return -1.

	\param id id is an AlarmID enum that specifies which alarm to set.

	\par Example:

	The following is an example of a user showing the current time in the console. (This can be used in the same kind of way
	to show the player a countdown via text.)

	\code

	CountDownObject::CountDownObject(){
		
		//setting up initial properties like visual repsentation and collision omitted..

		SetAlarm(AlarmID_0,60); // sets alarm0 to 60 seconds from creation
		RegisterForUpdate();
	}
	
	void CountDownObject::Update(){
		
		cout<< GetTimeRemainingOfAlarm(AlarmID_0);

	}

	void CountDownObject::Alarm0(){ // will be called after 60 seconds
		
		//Do stuff when alarm 0 is activated (in this case 60 seconds)

	}

	//void CountDownObject::OnTerminate() omitted
	//void CountDownObject::Draw() omitted..

	\endcode

	*/
	float GetTimeRemainingOfAlarm(AlarmID id);

	/**
	\ingroup ALARM
	\brief This method adds time to an active alarm.
	
	This method will add to the time remaining on an alarm. If the alarm is not active, then no time will
	be added.

	\param id id is an AlarmID enum that specifies which alarm to set.
	\param timeToAdd is a float that specifies how much time to add to the alarm.

	\par Example:

	The following is an example of a user adding time to a countdown (singleton for clarity) if the player (as a ship) collects a time Buff.

	\code

	TimeBuff::TimeBuff(){
		
		//setting up initial properties like visual repsentation and collision omitted..

		RegisterCollision(this);
	}
	
	void TimeBuff::Collision(Ship* other){
		Timer::AddTimeToAlarm(AlarmID_0,10);//adds 10 seconds to Timer's alarm 0 (which is the countdown alarm) also assuming timer is an Alarmable
		MarkForDestroy();
	}

	void TimeBuff::OnTerminate(){
		DeregisterCollision(this);
	}
	//void TimeBuff::Draw() omitted..

	\endcode

	*/
	void AddTimeToAlarm(AlarmID id,float timeToAdd);

	/**
	\ingroup ALARM
	\brief This method subtracts time to an active alarm.
	
	This method will subtract from the time remaining on an alarm. If the alarm is not active, then no time will
	be subtracted.

	\param id id is an AlarmID enum that specifies which alarm to set.
	\param timeToSubtract is a float that specifies how much time to subtract from the alarm.

	\par Example:

	The following is an example of a user subtracting time from a countdown (singleton for clarity) if the player (ship) collides with an asteroid.

	\code

	Ship::Ship(){
		
		//setting up initial properties like visual repsentation and collision omitted..

		RegisterCollision(this);
	}
	
	void Ship::Collision(Asteroid* other){
		Timer::SubtractTimeFromAlarm(AlarmID_0,2);//subtracts 2 seconds from Timer's alarm 0 (which is the countdown alarm) also assuming timer is an Alarmable
		other->MarkForDestroy();
	}

	void Ship::OnTerminate(){
		DeregisterCollision();
	}
	//void TimeBuff::Draw() omitted..

	\endcode

	*/
	void SubtractTimeFromAlarm(AlarmID id,float timeToSubtract);

	/**
	\ingroup ALARM
	\brief This method cancels an Alarm in the referenced Alarmable.
	
	This method cancels an alarm in the alarmable by specifiying which alarm to cancel.
	If the alarm is not active, canceling it will have no effect.

	\param id id Is an AlarmID enum that specifies which alarm to cancel. 


	\par Example:

	The following is an example of a scenario where a player must reach a bomb (to diffuse it) before it explodes.

	\code

	Bomb::Bomb(){
		
		//setting up initial properties like visual repsentation and collision omitted..

		SetAlarm(AlarmID_0,30); // sets alarm0 to 30 seconds from creation (player must reach bomb before 30 seconds)

	}

	void Bomb::Alarm0(){ // will be called after 30 seconds
		
		//Do stuff when alarm 0 is activated (in this case 30 seconds)
		// possibly explode and call GameOver.

	}

	void Bomb::Collision(Player* other){
		this->CancelAlarm(AlarmID_0);
	}

	//void Bomb::OnTerminate() omitted
	//void Bomb::Draw() omitted..

	\endcode

	*/
	void CancelAlarm(AlarmID id);

	void PauseAlarm(AlarmID id);

protected:

	Alarmable(){
		
		for(int i =0;i< MAX_ALARM_EVENTS ;++i){ 
			timeTrigger[i] =-1;//start all alarms as inactive
		}
		/*
		//start as not paused
		for(int i =0;i< MAX_ALARM_EVENTS ;++i){ 
			IsPaused[i] =false;//start all alarms as not paused
		}
		*/
	}
	Alarmable( const Alarmable & rhs){ rhs;}
	Alarmable& operator = (const Alarmable& rhs){ rhs;}
	
	/**
	\brief Method that calls the appropriate callback.

	All Dispatch(AlarmID) does is take in an alarm ID ( 0 - 2 ) and calls the
	correponding callback function. 


	*/
	void Dispatch(AlarmID);

	/**
	\brief The number of alarms.
	*/
	static const int MAX_ALARM_EVENTS = 3;

	/**
	\brief This array holds the times set for each alarm in Alarmable.
	*/
	float timeTrigger[MAX_ALARM_EVENTS];
	
	/**
	\ingroup ALARM
	\brief This is a callBack that will be called when the alarm1 is triggered
	
	This method is a callback that will be called when the time countdown for alarm1 is triggered. This method
	must be reimplemented if the user wants the alarm0 method to do something.

	\par Example:

	The following code will cause a bomb's Alarm0() to be called 5 seconds after bomb is thrown (created.)

	\code

	Bomb::Bomb(){
		
		//setting up initial properties like visual repsentation and collision omitted..

		SetAlarm(AlarmID_0,5); // sets alarm0 to 5 seconds from creation

	}

	void Bomb::Alarm0(){ // will be called after 5 seconds

		new Explosion() //create explosion
		this->MarkForDestroy(); //destroy self

	}

	//void Bomb::OnTerminate() omitted
	//void Bomb::Draw() omitted..

	\endcode

	*/
	virtual void Alarm0(){}

	/**
	\ingroup ALARM
	\brief This is a callBack that will be called when the alarm1 is triggered

	This method is a callback that will be called when the time countdown for alarm1 is triggered.
	This method must be reimplemented if the user wants the alarm1 method to do something.

	\note look at Alarm0() for an example of an Alarm callback.

	*/
	virtual void Alarm1(){}

	/**
	\ingroup ALARM
	\brief This is a callBack that will be called when the alarm2 is triggered

	This method is a callback that will be called when the time countdown for alarm2 is triggered.
	This method	must be reimplemented if the user wants the alarm2 method to do something.
	\note look at Alarm0() for an example of an Alarm callback.

	*/
	virtual void Alarm2(){}

	/*
	bool IsPaused[MAX_ALARM_EVENTS];
	*/
};
#endif