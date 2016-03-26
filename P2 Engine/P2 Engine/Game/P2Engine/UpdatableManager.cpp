#include "UpdatableManager.h"
#include "Updatable.h"
UpdatableManager::~UpdatableManager(){
	//does not own updatables
	activeUpdatablesList.clear();
}

UpdatableManager::UpdatableManager(){
	activeUpdatablesList = list<Updatable*>();
}

void UpdatableManager::Register(Updatable* registrant){

	bool found =false;
	//check if not in list
	for(list<Updatable*>::iterator it = activeUpdatablesList.begin();!found &&it != activeUpdatablesList.end();++it){
		if(registrant==*it){
			//cout<< "UpdateManager:: Updatable Already In List"<<endl;
			found =true;
		}
	}
	if(!found){
		activeUpdatablesList.push_back(registrant);
	}
}

void UpdatableManager::Deregister(Updatable* target){

	bool found =false;
	//check if not in list
	for(list<Updatable*>::iterator it = activeUpdatablesList.begin();!found &&it != activeUpdatablesList.end();++it){
		if(target==*it){
			found =true;
		}
	}
	if(found){
		activeUpdatablesList.remove(target);
	}
	
	//else{//for debugging
	//	cout<< "UpdateManager:: Updatable not in List"<<endl;
	//}
}

void UpdatableManager::UpdateAll(){

	//iterate through list and draw each registered drawable
	for(iterate = activeUpdatablesList.begin();iterate!=activeUpdatablesList.end();++iterate){
		(*iterate)->Update();
	}

}