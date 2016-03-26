#include "SceneManager.h"
#include "Scene.h"
#include "Updatable.h"
#include "Inputable.h"
#include "Drawable.h"
#include "Alarmable.h"
#include "Azul.h"
#include "Terminable.h"
#include "Terrain.h"

SceneManager* SceneManager::instance=NULL;

SceneManager::~SceneManager(){
	instance=NULL;
}

void SceneManager::DeleteMe(){
	Instance().CleanUpCurrentScene();
	delete instance;
}

void SceneManager::UpdateCurrentScene(){

	//check to see if user requested SceneChange
	if(Instance().currentScene!=Instance().nextScene){
		Instance().currentScene->OnSceneEnd();
		delete Instance().currentScene; //clean up current scene
		Instance().currentScene = Instance().nextScene;
		Instance().currentScene->SetUp();//set up next scene
	}


	Instance().currentScene->UpdateScene();
}

void SceneManager::SetCurrentTerrain(const char* const assetName){
	Instance().currentScene->SetCurrentTerrain(assetName);
}

Terrain* SceneManager::GetCurrentTerrain() {

	return Instance().currentScene->GetTerrainManager().GetCurrentTerrain();
}

void SceneManager::SetStartScene(Scene* targetScene){

	Instance().currentScene = targetScene;//set previous to same to avoid scene change
	Instance().nextScene = targetScene;
	Instance().currentScene->SetUp();//set up initial scene
}

void SceneManager::ChangeScene(Scene* nScene){
	Instance().nextScene = nScene;
}

void SceneManager::DrawCurrentScene(){
	Instance().currentScene->DrawScene();
}

void SceneManager::RegisterForUpdating(Updatable* u){
	Instance().currentScene->GetUpdatableManager().Register(u);
}

void SceneManager::DeregisterForUpdating(Updatable* u){
	Instance().currentScene->GetUpdatableManager().Deregister(u);	
}

void SceneManager::RegisterForDrawing(Drawable* d){
	Instance().currentScene->GetDrawableManager().Register(d);
}

void SceneManager::DeregisterForDrawing(Drawable* d){
	Instance().currentScene->GetDrawableManager().Deregister(d);
}

void SceneManager::RegisterForInput(Inputable * in, AZUL_KEY key, KeyEvent keyEvent = KeyEvent::KeyUpAndDown ){
	Instance().currentScene->GetInputManager().RegisterInput( in, key, keyEvent );
}

void SceneManager::DeregisterForInput(Inputable * in, AZUL_KEY key, KeyEvent keyEvent){
	Instance().currentScene->GetInputManager().DeregisterInput( in, key, keyEvent );
}

void SceneManager::DeregisterAllInputKeys(Inputable * in){
	Instance().currentScene->GetInputManager().DeregisterAllInputKeys(in);
}

void SceneManager::RegisterForAlarm(Alarmable* ptr,AlarmID id,float s){
	Instance().currentScene->GetAlarmManager().Register(ptr,id,s);
}

void SceneManager::DeregisterForAlarm(Alarmable* ptr,AlarmID id){
	Instance().currentScene->GetAlarmManager().Deregister(ptr,id);
}

void SceneManager::AddTimeToAlarm(Alarmable* ptr,AlarmID id,float timeToAdd){
	Instance().currentScene->GetAlarmManager().AddTimeToAlarm(ptr,id,timeToAdd);
}

void SceneManager::SubtractTimeFromAlarm(Alarmable* ptr,AlarmID id,float timeToSubtract){
	
	Instance().currentScene->GetAlarmManager().SubtractTimeFromAlarm(ptr,id,timeToSubtract);
}


void SceneManager::RegisterForTermination(Terminable * ptr){
	Instance().currentScene->GetTerminableManager().Register(ptr);
}

void SceneManager::RegisterInTerminationDumpster(Terminable * ptr){

	Instance().currentScene->GetTerminationDumpster().RegisterToDumpster(ptr);
}

void SceneManager::DeregisterFromTerminationDumpster(Terminable * ptr){

	Instance().currentScene->GetTerminationDumpster().DeregisterFromDumpster(ptr);
}

void SceneManager::DestroyNow(Terminable* ptr){
	Instance().currentScene->GetTerminationDumpster().DestroyNow(ptr);
}

void SceneManager::CleanUpCurrentScene(){
	Instance().currentScene->OnSceneEnd();
	delete Instance().currentScene;
}