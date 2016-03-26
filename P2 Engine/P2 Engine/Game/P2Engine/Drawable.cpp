#include "Drawable.h"
#include "SceneManager.h"

void Drawable::RegisterToDraw(){
	SceneManager::RegisterForDrawing(this);
}

void Drawable::DeregisterToDraw(){
	SceneManager::DeregisterForDrawing(this);
}