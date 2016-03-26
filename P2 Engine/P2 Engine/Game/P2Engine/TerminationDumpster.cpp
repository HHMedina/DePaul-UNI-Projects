#include "TerminationDumpster.h"
#include "Terminable.h"
#include <iostream>
using namespace std;

void TerminationDumpster::DestroyNow(Terminable* target){

	//if terminable is in list, delete, else give error message.
	if(std::find(dumpsterList.begin(), dumpsterList.end(), target) != dumpsterList.end()){

		
		//removes target from list and frees up memory
		dumpsterList.remove(target);

		//delete now or send to factory
		delete target;
	}
	//else{ //for debug
	//	cout<<"TerminationDumpster:" << target<<" was not found in registry."<<endl;
	//}
}

void TerminationDumpster::RegisterToDumpster(Terminable* registrant){
	
	//if terminable is already in list, do not double register
	if(std::find(dumpsterList.begin(), dumpsterList.end(), registrant) == dumpsterList.end()){
		dumpsterList.push_front(registrant);
	}
	
	//else{//for debug
	//	cout<<"TerminationDumpster:" << registrant<<" is already Registered."<<endl;
	//}

}

void TerminationDumpster::DeregisterFromDumpster(Terminable* registrant){
	
	//remove form list
	dumpsterList.remove(registrant);

}

TerminationDumpster::~TerminationDumpster(){

	int counter =0;

	//on scene end ~terminableDumpster() will be called
	for(list<Terminable*>::iterator it = dumpsterList.begin();it != dumpsterList.end();++it){
		(*it)->DeregisterFromAutomated();
		delete (*it);
		++counter;
	}

	dumpsterList.clear();

}