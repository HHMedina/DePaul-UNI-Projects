#include "DrawableManager.h"
#include "Drawable.h"
#include <iostream>
using namespace std;
DrawableManager::DrawableManager(){
	activeDrawablesList = list<Drawable*>();
}

DrawableManager::~DrawableManager(){

	activeDrawablesList.clear();
}

void DrawableManager::Register(Drawable* registrant){

	//if drawable is in not in list, add to list
	if(std::find( activeDrawablesList.begin(), activeDrawablesList.end(), registrant) == activeDrawablesList.end()){


		//Add Drawable
		activeDrawablesList.push_back(registrant);
	}
	//else do nothing

}

void DrawableManager::Deregister(Drawable* target){

	//if drawable is in list, delete
	if(std::find( activeDrawablesList.begin(), activeDrawablesList.end(), target) != activeDrawablesList.end()){


		//removes target from list and frees up memory
		activeDrawablesList.remove(target);
	}
	//else do nothing
	
}

void DrawableManager::DrawAll(){

	//iterate through list and draw each registered drawable
	for(iterate = activeDrawablesList.begin();iterate!=activeDrawablesList.end();++iterate){
		(*iterate)->Draw();
	}

}