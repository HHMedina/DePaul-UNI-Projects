#include <TerminableManager.h>
#include <Terminable.h>
#include "SceneManager.h"

TerminableManager::~TerminableManager(){

	//TerminableManager does not own Terminables
	terminateList.clear();
}

void TerminableManager::Register(Terminable* ptr){
	
	if(!ptr->markedForTermination){//not already marked for termination
		ptr->markedForTermination =true;
		terminateList.push_back(ptr);
	}
}

void TerminableManager::Deregister(Terminable* ptr){
	
	//only remove pointer
	terminateList.remove(ptr);
}

void TerminableManager::processTerminations(){

	while(!terminateList.empty()){
		terminateList.front()->OnTerminate();
		terminateList.front()->DeregisterFromAutomated();

		//check if externally managed. if so... sent to external manager else destroy now
		if(terminateList.front()->HasExternalManager()){
			terminateList.front()->SendToExternalManager();
		}else{
			SceneManager::DestroyNow(terminateList.front());	// remove immediately
		}
		terminateList.pop_front();//remove pointer
	}

}