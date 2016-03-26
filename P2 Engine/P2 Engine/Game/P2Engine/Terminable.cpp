#include "Terminable.h"
#include "SceneManager.h"

Terminable::Terminable(){
	hasExternalManagement=false;
	markedForTermination=false;
	extFunc = NULL;
	SceneManager::RegisterInTerminationDumpster(this);
}

bool Terminable::HasExternalManager(){

	return hasExternalManagement;
}

void Terminable::SetExternalManagement(ExternalManagerFunction exFunc){
	
	//Set the External Management function
	hasExternalManagement = true;
	extFunc= exFunc;
	SceneManager::DeregisterFromTerminationDumpster(this);
		
}

void Terminable::SendToExternalManager(){
	this->markedForTermination =false;
	//note: send is only called if there is an external manager
	extFunc(this);
}

void Terminable::MarkForDestroy(){
	
	SceneManager::RegisterForTermination(this);
	
}