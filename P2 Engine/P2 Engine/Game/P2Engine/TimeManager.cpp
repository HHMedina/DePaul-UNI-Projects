#include "TimeManager.h"

TimeManager* TimeManager::instance = NULL;

TimeManager::~TimeManager(){
	instance=NULL;
}

void TimeManager::DeleteMe(){
	delete instance;
}

float TimeManager::GetFrameTime(){
	return Instance().currentFrameTime.timeInSeconds();
}

float TimeManager::GetTotalTime(){
	return Instance().totalTime.timeInSeconds();
}

float TimeManager::GetTotalGameTime(){

	return Instance().frameTimeSum;
}

void TimeManager::Update(){

	
	//tocs the time (to get delta time) then resets (to the current time in the timeline)
	Instance().totalTime.toc();
	Instance().currentFrameTime.toc();

	//total time when not freezing
	Instance().frameTimeSum += Instance().currentFrameTime.timeInSeconds();

	
	//check if freeze mode
	Instance().FreezeFrameProcessing();

	
	Instance().currentFrameTime.tic();
	//cout<<Instance().frameTimeSum<<"<===GameTime " << Instance().currentFrameTime.timeInSeconds()<< "<===FrameTime" << Instance().totalTime.timeInSeconds() << "<===TotalTime" <<endl;

}

void TimeManager::FreezeFrameProcessing(){

	static bool isFreezeMode = false;

	if(InputMan::GetKeyboard()->GetKeyState(FREEZE_KEY) || isFreezeMode)
	{

		WaitForRelease(FREEZE_KEY);

		isFreezeMode = true;
		bool single_frame = false;

		while(isFreezeMode && !single_frame)
		{

			if(InputMan::GetKeyboard()->GetKeyState(FREEZE_KEY))
			{
				WaitForRelease(FREEZE_KEY);
				isFreezeMode =false;
				single_frame =false;

			}
			else if(InputMan::GetKeyboard()->GetKeyState(SINGLE_FRAME_KEY))
			{
				WaitForRelease(SINGLE_FRAME_KEY);
				single_frame = true;
			}

			glfwPollEvents();
		}


	}

}

void TimeManager::WaitForRelease(AZUL_KEY k){

	while(InputMan::GetKeyboard()->GetKeyState(k)){
		glfwPollEvents();
	}
}

void TimeManager::InitializeStopWatches(){

	// Initialize timer
	StopWatch::initStopWatch();

	// Start timers
	Instance().totalTime.tic();
	Instance().currentFrameTime.tic();

}