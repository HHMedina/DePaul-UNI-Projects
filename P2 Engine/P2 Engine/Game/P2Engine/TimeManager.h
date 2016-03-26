#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <Azul.h>
#include <list>
#include "Scene.h"


using namespace std;

class TimeManager{

	//gives private access to Game
	//for deleteMe(), Update(), and InitializeStopWatches().
	friend class Game;

public:


	/**
	\ingroup TIMEMANAGER
	\brief This function returns the time for a single frame.

	\par The following prints out the frametime as part of a Ship's Update(). 

	\code
	#include "Ship.h"
	#include <iostream>
	#include "TimeManager.h"

	Ship::Update(){
		cout<< TimeManager::GetFrameTime();
	}

	\endcode
	*/
	static float GetFrameTime();

	/**
	\brief This function is mainly for debugging purposes. This function returns
	the total time of the game including debug-purpose pauses.

	*/
	static float GetTotalTime();

	/**
	\ingroup TIMEMANAGER
	\brief This function returns the game time since the start of the game.

	\par The following prints out the total time as part of a Ship's Update(). 

	\code
	#include "Ship.h"
	#include <iostream>
	#include "TimeManager.h"

	Ship::Update(){
		cout<< TimeManager::GetTotalGameTime();
	}

	\endcode
	*/
	static float GetTotalGameTime();
private:

	TimeManager(){frameTimeSum =0;}
	TimeManager( const TimeManager & rhs){ rhs;}
	TimeManager& operator = (const TimeManager& rhs){rhs;}
	~TimeManager();

	static TimeManager& Instance(){
		if(!instance){
			instance = new TimeManager();
		}
		return *instance;
	}


	/**
	\brief This method is called at everyframe and increments the total time with the frame time as well as storing the frame time.
	*/
	static void Update();

	/**
	\brief This method is called at the start of the game to initialize the stop watches.
	*/
	static void InitializeStopWatches();
	
	/**
	\brief This method called delete on the singleton.
	*/
	static void DeleteMe();

	/**
	\brief This method called on a freeze frame
	*/
	void FreezeFrameProcessing();

	/**
	\brief This method checks for the release of a key

	\param k k is the key to wait for release

	*/
	void WaitForRelease(AZUL_KEY k);

	/**
	\brief This is a singleton.
	*/
	static TimeManager* instance;//this is a singleton

	/**
	\brief this stop watch holds the frametime
	*/
	StopWatch currentFrameTime;

	/**
	\brief this stopwatch holds the time since the start of the game.
	*/
	StopWatch totalTime;

	/**
	\brief float that holds the total game time without pauses
	*/
	float frameTimeSum; 

	
	/**
	\brief Specifies the freeze key.
	*/
	static const AZUL_KEY FREEZE_KEY = AZUL_KEY::KEY_KP_SUBTRACT;

	/**
	\brief Specifies the single frame progression key.
	*/
	static const AZUL_KEY SINGLE_FRAME_KEY = AZUL_KEY::KEY_KP_ADD;

};




#endif